# 待修复问题记录

## 1. 串口无输出（已修复 ✅）
**根因**：codex 重构时还原了 `startup_py32f403xx.s` 原有版本：
- `bl __libc_init_array` 未注释 → NULL 函数指针 HardFault
- `bl main` 应为 `bl entry` → RTT 入口错误
- FPU CPACR 使能丢失 → 硬浮点指令 HardFault

**修复**：2026-05-19，在 `sdk/CMSIS/.../startup_py32f403xx.s` 恢复三处修复。

## 2. SFUD 芯片识别（已修复 ✅）
**根因**：
1. `rtconfig.h` 缺少 `RT_SFUD_USING_FLASH_INFO_TABLE`，静态参数表未编译
2. W25Q128FW (JEDEC ID `EF 60 18`) 不在 SFUD 内置表中
3. SFDP 解析器对 `table[0] & 0x03 = 0`（JESD216 "未提供信息"）误判为错误

**修复**：
- `sfud_flash_def.h` 添加 `W25Q128FW` 条目 (0x60, 0x18)
- `rtconfig.h` 添加 `RT_SFUD_USING_FLASH_INFO_TABLE`
- `sfud_sfdp.c` `case 0`: 添加 4KB 擦除假设支持
- 现通过表回退成功识别，SFDP 容量编码警告为已知限制（不影响功能）

## 3. SPI 写后 DMA 异常（已修复 ✅）
（见 `USAGE.md` §7 修复列表）

## 4. menuconfig 依赖外部 RTT 路径（已修复 ✅）
**根因**：
- `os/Kconfig` 中 `RTT_DIR` 默认值硬编码外部路径 `"D:/MCU/6.OS/RTT/rt-thread-v5.1.0"`
- `xmake menuconfig` task 直接引用外部 RTT 树
- kconfiglib 需要 `$RTT_DIR` 等环境变量直接映射

**修复**：
- 拷贝完整 RTT Kconfig 树到 `os/rt-thread/`（含所有组件 Kconfig）
- `os/Kconfig` `RTT_DIR` 默认值改为 `"os/rt-thread"`
- `xmake.lua` menuconfig task 使用 `os.scriptdir()` 推导本地路径
- 同时设置 `RTT_ROOT`/`RTT_DIR`、`BSP_ROOT`/`BSP_DIR`、`PKGS_ROOT`/`PKGS_DIR`
- 创建 `packages/Kconfig` 入口文件
- 已通过 kconfiglib 验证：892 symbols 正确解析

## 5. drivers/ 与 lib/drivers/ 双目录并存（已修复 ✅）
- 旧 `drivers/` 目录已确认无外部代码引用
- 所有文件已迁移到 `lib/drivers/`（含 `config/` 子目录）
- 已安全删除

## 6. DFS POSIX/libc 桥接（已修复 ✅）
DFS 头文件依赖 `<sys/statfs.h>`、`<dirent.h>` 等 POSIX 头，与 bare-metal ARM GCC newlib 冲突。
**根因**：RTT 的 POSIX shim 头与 newlib 存在 `sys/time.h`、`dirent.h` 重叠，相互冲突。
**修复**：
- 在 `os/posix_override/` 放置 RTT 的 `dirent.h` + `sys/statfs.h`（仅 DFS 需要的头）
- 添加 `lib/drivers/syscalls_stub.c` 提供缺失的 newlib syscall（`_gettimeofday`、`_sbrk`、`timegm`）
- 添加 `msh_file.c`（finsh 脚本执行支持）
- 添加 `devfs.c`（DFS 设备文件系统）

**状态**：DFS + elmfat + devfs 全部编译通过，运行时 SPI 读写正常。

## 7. menuconfig 配置丢失 BSP 自定义选项（已修复 ✅）
menuconfig 全量重写 `rtconfig.h` 时清掉了 SFUD/SPI DMA/I2C/PWM 等手动配置。
**修复**：将这些选项添加到 `os/Kconfig` 的 `Hardware Drivers Config` 菜单中，
下次运行 `xmake menuconfig` 时统一管理，不再依赖手动编辑。
