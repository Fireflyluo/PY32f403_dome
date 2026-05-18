# RTT 生态组件接入示例：DFS + SFUD + W25Q128

## 组件源文件管理模式

RTT 生态组件（DFS、SFUD、FAL、MQTT 等）的源代码在 RTT 官方源码树中。
项目引用这些组件有两种模式：

### 模式 A：外部引用（推荐个人项目）

项目依赖本地一份完整的 RTT 源码树，通过路径或 env 工具引用。

```
d:/rtt/rt-thread-v5.1.0/         # 完整 RTT 源码树（git clone + pkgs --update）
  ├── components/                 # 所有组件源文件都在这里
  ├── packages/                   # pkgs --update 下载的在线包
  └── ...

你的工程/
  └── xmake.lua                   # add_files("d:/rtt/.../components/dfs/src/dfs.c")
```

**优点**：一份 RTT 树供多个项目共享、env 工具管理在线包、不重复拷贝
**缺点**：项目移动需要改路径、依赖外部环境

### 模式 B：自包含（本项目采用）

将用到的组件源文件拷贝到项目内的 `os/rt-thread/` 下。

```
你的工程/
  └── os/rt-thread/
      └── components/
          ├── dfs/                # 已拷贝
          ├── finsh/              # 已拷贝
          └── drivers/spi/sfud/   # 已拷贝
```

**优点**：零外部依赖、`git clone` 即编译
**缺点**：每个项目都要手动拷组件、在线包需自行下载后拷入

### 选择建议

| 场景 | 推荐模式 |
|------|---------|
| 个人日常开发，有 RTT env 工具 | **A** — 用 env 统一管理 |
| 多人协作出差/离线 | **A** — RTT 树固定版本后一起分发 |
| 发布 BSP 示例、demo | **B** — 用户 clone 即用，无需装 RTT |
| 嵌入式竞赛/课程作业 | **B** — 减少环境配置成本 |

本项目作为 BSP 模板示例使用，故选择 **模式 B（自包含）**。

---

### 模式 A 快速示例

以外部引用 DFS 为例，`xmake.lua` 中这样写：

```lua
-- 指向你本地的 RTT 源码树
RTT_ROOT = "D:/MCU/6.OS/RTT/rt-thread-v5.1.0"

target("rtthread")
    add_includedirs(
        path.join(RTT_ROOT, "components/dfs/include"),
        path.join(RTT_ROOT, "components/finsh")
    )
    add_files(
        path.join(RTT_ROOT, "components/dfs/src/dfs.c"),
        path.join(RTT_ROOT, "components/dfs/src/dfs_file.c"),
        path.join(RTT_ROOT, "components/dfs/src/dfs_fs.c")
    )
```

然后就可以直接用 env 工具的 `menuconfig + pkgs --update` 管理组件和在线包，
xmake.lua 里通过 `RTT_ROOT` 路径引用即可。此时 `os/rt-thread/` 目录不需要存在。

---

## 目标

在 PY32F403 上通过 RTT 生态组件实现：
1. SFUD 驱动识别 W25Q128FW SPI Flash
2. DFS 虚拟文件系统挂载 FAT（elmfat）
3. 从 C 代码中创建、写入、读取文件

## 完整流程

### 1. 通过 menuconfig 启用组件

```powershell
cd demo\rtthread
xmake menuconfig
```

GUI 窗口中操作：

```
RT-Thread Components
  └── [*] DFS: device virtual file system
       └── [*] Enable elm-chan fatfs

Hardware Drivers Config
  └── [*] Enable SPI Flash SFUD
       └── [*] Enable SFDP auto-detect
       └── [*] Enable static flash table fallback
```

Save 退出后，menuconfig 自动：
- 更新 `os/rtconfig.h` → 添加 `RT_USING_DFS`、`RT_USING_DFS_ELMFAT` 等
- 生成 `os/.config` → 保存所有选择的配置

### 2. 将组件源文件加入构建

打开 `xmake.lua`，在 `add_files` 块中添加 DFS 源文件：

```lua
-- DFS 核心
path.join(RTT_ROOT, "components/dfs/src/dfs.c"),
path.join(RTT_ROOT, "components/dfs/src/dfs_file.c"),
path.join(RTT_ROOT, "components/dfs/src/dfs_fs.c"),
path.join(RTT_ROOT, "components/dfs/src/dfs_posix.c"),

-- elmfat（FAT32 文件系统）
path.join(RTT_ROOT, "components/dfs/filesystems/elmfat/dfs_elm.c"),
path.join(RTT_ROOT, "components/dfs/filesystems/elmfat/ff.c"),
path.join(RTT_ROOT, "components/dfs/filesystems/elmfat/ffunicode.c"),

-- devfs（设备文件系统）
path.join(RTT_ROOT, "components/dfs/filesystems/devfs/devfs.c"),

-- msh 脚本执行支持（finsh 在 DFS 下执行脚本文件）
path.join(RTT_ROOT, "components/finsh/msh_file.c"),

-- SFUD（SPI Flash 通用驱动）
path.join(RTT_ROOT, "components/drivers/spi/spi_flash_sfud.c"),
path.join(RTT_ROOT, "components/drivers/spi/sfud/src/sfud.c"),
path.join(RTT_ROOT, "components/drivers/spi/sfud/src/sfud_sfdp.c"),
```

同时添加 include 路径：

```lua
add_includedirs(
    ...
    path.join(RTT_ROOT, "components/dfs/include"),
    path.join(RTT_ROOT, "components/dfs/filesystems/elmfat"),
    "os/posix_override",     -- POSIX 头覆盖（dirent.h, sys/statfs.h）
)
```

以及 newlib syscall stub（DFS 的 POSIX 层需要）：

```lua
add_files("lib/drivers/syscalls_stub.c")
```

### 3. 实现应用代码

在 `app/main.c` 中添加文件操作命令：

```c
#include <rtthread.h>
#include <board.h>

#ifdef RT_USING_SFUD
#include "spi_flash_sfud.h"
#endif
#ifdef RT_USING_DFS
#include <dfs_fs.h>
#endif

#define LED_PIN GET_PIN(B, 2)

/* ── SFUD 探测 ── */
#ifdef RT_USING_SFUD
static int cmd_sfud(int argc, char **argv)
{
    rt_pin_mode(GET_PIN(A, 4), PIN_MODE_OUTPUT);
    rt_pin_write(GET_PIN(A, 4), PIN_HIGH);

    static struct rt_spi_device spi_dev;
    rt_err_t ret = rt_spi_bus_attach_device(&spi_dev, "spi10", "spi1", RT_NULL);
    if (ret != RT_EOK) { rt_kprintf("attach failed\n"); return -1; }
    spi_dev.cs_pin = GET_PIN(A, 4);

    rt_spi_flash_device_t flash = rt_sfud_flash_probe("W25Q128", "spi10");
    rt_kprintf("flash: %s\n", flash ? "OK" : "FAIL");
    return flash ? 0 : -1;
}
MSH_CMD_EXPORT(cmd_sfud, probe W25Q128 via SFUD);
#endif

/* ── 挂载文件系统 ── */
#ifdef RT_USING_DFS
static int cmd_mount(int argc, char **argv)
{
    /* SFUD 探测 → 创建 /dev/w25q128 块设备 */
    rt_spi_flash_device_t flash = rt_sfud_flash_probe("W25Q128", "spi10");
    if (!flash) { rt_kprintf("flash probe failed\n"); return -1; }

    /* 格式化（出厂 Flash 一般为全 0xFF，首次需格式化） */
    if (dfs_mount("W25Q128", "/", "elm", 0, 0) != 0)
    {
        rt_kprintf("mount failed, try mkfs...\n");
        /* mkfs 会让 DFS 初始化 FAT 文件系统 */
        if (dfs_mkfs("elm", "W25Q128") != 0)
        {
            rt_kprintf("mkfs failed\n");
            return -1;
        }
        /* 格式化后重新挂载 */
        if (dfs_mount("W25Q128", "/", "elm", 0, 0) != 0)
        {
            rt_kprintf("remount failed\n");
            return -1;
        }
    }

    rt_kprintf("W25Q128 mounted to /\n");
    return 0;
}
MSH_CMD_EXPORT(cmd_mount, mount W25Q128 as FAT);

/* ── 写文件测试 ── */
static int cmd_writetest(int argc, char **argv)
{
    int fd = open("/hello.txt", O_CREAT | O_WRONLY, 0);
    if (fd < 0) { rt_kprintf("open failed\n"); return -1; }
    write(fd, "Hello from RT-Thread!\n", 22);
    close(fd);
    rt_kprintf("written /hello.txt\n");

    fd = open("/hello.txt", O_RDONLY);
    if (fd >= 0) {
        char buf[64];
        int n = read(fd, buf, sizeof(buf) - 1);
        if (n > 0) { buf[n] = '\0'; rt_kprintf("read: %s", buf); }
        close(fd);
    }
    return 0;
}
MSH_CMD_EXPORT(cmd_writetest, write + read hello.txt);

/* ── DF（显示磁盘空间） ── */
static int cmd_df(int argc, char **argv)
{
    struct statfs stat;
    if (statfs("/", &stat) == 0) {
        rt_kprintf("total: %d KB, free: %d KB\n",
            stat.f_blocks * stat.f_bsize / 1024,
            stat.f_bfree * stat.f_bsize / 1024);
    }
    return 0;
}
MSH_CMD_EXPORT(cmd_df, show disk free);
#endif

int main(void)
{
    rt_pin_mode(LED_PIN, PIN_MODE_OUTPUT);
    while (1) {
        rt_pin_write(LED_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
}
```

### 4. 构建 & 运行

```powershell
xmake -r
pyocd flash -t py32f403xd dist\rtthread.elf
```

打开串口（COM14, 115200），Shell 中验证：

```powershell
msh /> sfud_test
[I/SFUD] Found a Winbond W25Q128FW flash chip. Size is 16777216 bytes.
[I/SFUD] Probe SPI flash W25Q128 by SPI device spi10 success.
flash: OK

msh /> mount
[I/SFUD] ...
W25Q128 mounted to /

msh /> list device
device           type         ref count
W25Q128  Block Device         0          ← 块设备已注册

msh /> writetest
written /hello.txt
read: Hello from RT-Thread!

msh /> list fd
fd    type    ref
───   ────    ───
  0   file      1
```

## 组件依赖关系

```
┌──────────────────────────────┐
│   应用层 (main.c)             │
│   cmd_mount / cmd_writetest    │
└────┬─────────────────────┬───┘
     │                     │
     ▼                     ▼
┌──────────┐      ┌──────────────┐
│  DFS     │      │  SFUD         │
│  elmfat  │      │  spi_flash_sfud │
│  devfs   │      └──────┬───────┘
└────┬─────┘             │
     │                    ▼
     │              ┌──────────┐
     │              │ SPI 驱动 │
     │              │ drv_spi  │
     │              └────┬─────┘
     │                    │
     ▼                    ▼
┌──────────────────────────────┐
│      POSIX 桥接层             │
│  syscalls_stub.c (newlib)     │
│  os/posix_override/  (头文件)  │
└──────────────────────────────┘
```

## 常见问题

### Q: mkfs 失败 / mount 返回 -1
A: 确保 `rt_sfud_flash_probe` 先执行成功，然后检查 `list device` 中是否有
`W25Q128  Block Device` 条目。若没有，SFUD 探测未完成。

### Q: 编译提示 `undefined reference to _sbrk`
A: 需要在链接中包含 syscall stub。确认 `xmake.lua` 中有：
```lua
add_files("lib/drivers/syscalls_stub.c")
```

### Q: 编译提示 `dirent.h` 中 `DIR` 未定义
A: newlib 的 dirent.h 在裸机下不完整。确认 `-Ios/posix_override` 在
include path 中且位于 newlib 系统路径前。用 `-v` 编译参数查看搜索顺序。

### Q: menuconfig 生成的 rtconfig.h 缺少 SFUD/SPI 定义
A: 这些定义在 `os/Kconfig` 的 `Hardware Drivers Config` 菜单中。
`xmake menuconfig` → Hardware Drivers Config → 勾选所需选项。
