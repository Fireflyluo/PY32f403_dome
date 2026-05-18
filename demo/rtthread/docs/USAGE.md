# PY32F403 RTT BSP 使用文档

## 1. 环境准备

### 1.1 必备工具
- **ARM GCC 15.2.1** 工具链：`C:\APP\Env\tools\gnu_gcc\arm_gcc\mingw\bin`
- **xmake 3.x**：`xmake --version`
- **Python 3.x + kconfiglib**：menuconfig 依赖（`pip install kconfiglib`）
- **pyocd**：烧录工具（`pyocd --version`）

### 1.2 硬件连接
| 外设  | 引脚           | 说明         |
|-------|---------------|-------------|
| UART2 | PA2(TX) PA3(RX) | 调试串口 115200 |
| LED   | PB2           | 心跳指示灯    |
| SPI1  | PA5/PA6/PA7   | W25Q128 Flash |
| SPI3  | PB3/PB4/PB5   | XN297 无线模块 |
| I2C2  | PB10/PB11     | 总线设备扫描   |

## 2. 构建与烧录

### 2.1 编译
```powershell
cd demo\rtthread
xmake                    # 首次构建
xmake -r                 # 全量重构建（推荐调试用）
xmake f -c; xmake        # 清理配置后重构建
```

### 2.2 烧录
```powershell
# 全片擦除 + 烧录
pyocd erase -t py32f403xd --chip
pyocd flash -t py32f403xd dist\rtthread.elf

# 烧录后 opens COM14 观察启动日志
# 若 pyocd 跳过（已是最新），先全片擦除
```

### 2.3 串口
- **端口**：COM14 (CH340)
- **参数**：115200 8N1
- **技巧**：先开串口工具监听 COM14，再执行 pyocd flash

## 3. RTT Kconfig 生态验证

### 3.1 组件启用流程

RTT 组件通过三步启用：

```
Kconfig ──> rtconfig.h ──> xmake.lua 添加源文件 ──> 编译链接
```

menuconfig 的作用是自动生成 `rtconfig.h` 宏定义，开发者再手动
将对应的 `*.c` 文件加入 `xmake.lua`。

### 3.2 SFUD（SPI Flash 通用驱动）

#### rtconfig.h 配置
```c
#define RT_USING_SFUD
#define RT_SFUD_USING_SFDP
#define RT_SFUD_SPI_MAX_HZ (50 * 1000 * 1000)
```

#### xmake.lua 源文件
```lua
-- add_includedirs:
path.join(RTT_ROOT, "components/drivers/spi/sfud/inc"),

-- add_files:
path.join(RTT_ROOT, "components/drivers/spi/spi_flash_sfud.c"),
path.join(RTT_ROOT, "components/drivers/spi/sfud/src/sfud.c"),
path.join(RTT_ROOT, "components/drivers/spi/sfud/src/sfud_sfdp.c"),
```

#### 测试命令
```c
// 在 Shell 中执行
sfud_test   // 探测 W25Q128 (SPI通信已通，SFUD框架运行正常)
sf          // RTT 内置 SPI Flash 工具
```
**状态**：SFUD 框架编译链接通过、运行时 SPI 通信成功、CS 控制正常。
当前 W25Q128JV 芯片 ID (EF 60 18) 不在 SFUD 内置表中，需添加芯片条目。

### 3.3 menuconfig

```powershell
# 在 PowerShell / cmd 中执行，弹出 GUI 配置窗口
xmake menuconfig
```

menuconfig 完全自包含于工程目录 `os/rt-thread/tools/`，无外部依赖。
Kconfig 选项包括：
- **Hardware Drivers Config** — SPI、I2C、PWM、SFUD 等 BSP 级开关
- **RT-Thread Components** — DFS、Finsh、Device Drivers 等 RTT 组件

首次运行会自动生成 `os/.config`，并更新 `os/rtconfig.h`。
再次运行直接在已有配置基础上修改。

### 3.4 DFS 文件系统

DFS 已通过如下方式集成：

| 层级       | 内容                          | 文件               |
|-----------|------------------------------|-------------------|
| Kconfig   | 通过 `xmake menuconfig` 启用  | `os/Kconfig`      |
| 配置      | `#define RT_USING_DFS` 等    | `os/rtconfig.h`   |
| POSIX桥接  | newlib syscall stub          | `lib/drivers/syscalls_stub.c` |
| POSIX头覆盖 | `dirent.h` + `sys/statfs.h` | `os/posix_override/` |
| 源文件     | `dfs.c` + `dfs_elm.c` + `ff.c` + `devfs.c` | `xmake.lua` |

#### Shell 命令验证

```powershell
# 查看已注册设备
msh /> list device

# 文件描述符列表
msh /> list fd

# 探测 W25Q128 并挂载 FAT
msh /> sfud_test
msh /> sf probe spi10
msh /> sf read 0 128
```

#### 从 C 代码中使用 DFS

```c
/* main.c 示例：挂载 W25Q128 并写入文件 */
#include <rtthread.h>
#include "spi_flash_sfud.h"
#include <dfs_fs.h>

static int cmd_mount(int argc, char **argv)
{
    rt_spi_flash_device_t flash = rt_sfud_flash_probe("W25Q128", "spi10");
    if (flash) {
        if (dfs_mount("W25Q128", "/", "elm", 0, 0) == 0) {
            rt_kprintf("FAT mounted on /\n");
            return 0;
        }
        rt_kprintf("mount failed (need mkfs first)\n");
    }
    return -1;
}
MSH_CMD_EXPORT(cmd_mount, mount W25Q128 as FAT);
```

## 4. Shell 命令（可用时）

| 命令        | 功能              |
|------------|------------------|
| `help`     | 列出所有命令        |
| `list_device` | 列出注册的设备     |
| `sfud_test`| 探测 W25Q128      |
| `free`     | 显示内存使用        |
| `ps`       | 列出所有线程        |

## 5. 内存占用

| 项     | 大小    | 占比         | 说明                        |
|--------|--------|-------------|----------------------------|
| ROM    | 62 KB  | 512 KB (12%) | 含 DFS + elmfat + newlib stub |
| RAM    | 10.5 KB| 64 KB (16%)  | 含 Finsh + 全部驱动 + DFS   |

## 6. 目录结构

```
demo/rtthread/
├── xmake.lua          # 构建主脚本（自包含，不调用 os/rtt.lua）
├── toolchain.lua       # ARM GCC 工具链配置
├── bsp/                # CubeMX 生成的 HAL 初始化代码
│   ├── board.c         # 板级初始化
│   ├── CubeMX_Config/  # HAL MSP
│   └── linker_scripts/ # 链接脚本 (link.lds)
├── lib/drivers/        # RTT 设备驱动
│   ├── drv_common.c    # 板级初始化 + Systick
│   ├── drv_usart.c     # UART 驱动
│   ├── drv_spi.c       # SPI 驱动
│   ├── drv_hard_i2c.c  # I2C 驱动
│   ├── drv_tim.c       # 定时器驱动
│   ├── drv_pwm.c       # PWM 驱动
│   ├── drv_dma.h       # DMA 重映射
│   ├── syscalls_stub.c # newlib syscall（_sbrk/_gettimeofday/timegm）
│   └── config/py32f4/  # 芯片级配置
├── app/main.c          # 应用入口 + Shell 命令
├── os/                 # RTT 操作系统
│   ├── Kconfig         # RTT 组件配置入口（含 BSP 自定义选项）
│   ├── rtconfig.h      # 组件配置（menuconfig 生成 + 自定义保留）
│   ├── rtconfig_project.h  # （已废弃，配置由 Kconfig 统一管理）
│   └── rt-thread/      # RTT v5.1.0 源文件（自包含）
│       ├── include/    # 内核头文件
│       ├── src/        # 内核源文件
│       ├── libcpu/     # CPU 移植 (Cortex-M4)
│       ├── components/ # RTT 组件
│       │   ├── finsh/  # Finsh Shell + msh_file.c
│       │   ├── drivers/ # 驱动框架 (serial/spi/i2c/sfud)
│       │   ├── dfs/    # 虚拟文件系统（DFS + elmfat + devfs）
│       │   └── libc/   # POSIX 层
│       └── tools/      # menuconfig 工具（自包含，无外部依赖）
├── os/posix_override/  # POSIX 头覆盖（dirent.h + sys/statfs.h）
├── sdk/                # PUYA HAL 库 + CMSIS
└── docs/               # 文档
    ├── TODO.md         # 待修复问题
    └── hardware.md     # 硬件引脚映射
```

## 7. 已修复的关键 bug

| Bug                   | 位置                                        | 修复              | 日期       |
|-----------------------|--------------------------------------------|------------------|-----------|
| 启动文件被还原          | `startup_py32f403xx.s`                     | 恢复3处修复       | 2026-05-19 |
| ├─ `__libc_init_array` 崩溃 | 同上:111                               | 永久注释          | 2026-05-19 |
| ├─ `bl main`→`bl entry` | 同上:113                                | RTT入口修正       | 2026-05-19 |
| ├─ FPU CPACR 使能      | 同上:73                                  | CPACR FPU使能     | 2026-05-19 |
| BRR 波特率错误          | `system_py32f403.c:172`                  | `== 0x01`→`!= 0U` | 之前      |
| HSI PLL 计算错误        | `system_py32f403.c:181`                  | `>>1`→直接乘      | 之前      |
| Systick 提前启动        | `drv_common.c:168`                       | `INIT_DEVICE_EXPORT` | 之前   |
| SPI DMA 锁死            | `drv_spi.c` DMA超时/错误路径              | `__HAL_SPI_DISABLE`+清除标志 | 2026-05-19 |
| SPI recv-only NULL拷贝  | `drv_spi.c` 对齐守卫                     | `&& send_buf != RT_NULL` | 2026-05-19 |
| DFS POSIX 桥接           | `lib/drivers/syscalls_stub.c` + `os/posix_override/` | newlib syscall 替代 | 2026-05-19 |
| menuconfig 外部路径      | `xmake.lua` + `os/Kconfig`               | 全本地化 | 2026-05-19 |
| BSP 配置被 menuconfig 清掉 | `os/Kconfig`                            | 移入 Hardware Drivers Config | 2026-05-19 |
