# RT-Thread 标准版 — PY32F403 BSP

基于 RT-Thread v5.1.0 标准版的 PY32F403 移植工程，使用 xmake 构建。

**完全自包含**——所有 RTT 内核源码、HAL 库、驱动文件均内含于工程中，无需额外安装 RT-Thread 源码包。拿到即可编译。

独立的 BSP 副本见 `../rtthread-py32f403/`。

## 快速开始

```bash
# 进入工程目录
cd demo/rtthread

# 编译（Debug 模式）
xmake

# 烧录（CMSIS-DAP 调试器）
pyocd flash -t py32f403xd dist/rtthread.elf

# 查看串口输出（COM14, 115200）
# 建议先打开串口工具再烧录，以完整捕获启动日志
```

## 目录结构

```
demo/rtthread/
├── xmake.lua              # 主构建脚本（精简，~100 行）
├── toolchain.lua          # ARM GCC 工具链独立定义
├── os/                    # RT-Thread 内核配置层
│   ├── Kconfig            #   menuconfig 菜单（可选）
│   ├── rtconfig.h         #   内核 + 组件 + 驱动宏配置
│   └── rtt.lua            #   自动按需加载 RTT 组件源文件
├── bsp/                   # 板级支持包
│   ├── board.c            #   时钟配置（HSE 16MHz → 144MHz）
│   ├── board.h            #   板级宏 + 堆内存边界
│   ├── linker_scripts/    #   链接脚本（384KB Flash / 64KB SRAM）
│   └── CubeMX_Config/     #   HAL 配置 + MSP 初始化
├── drivers/               # 外设驱动（RTT 设备框架）
│   ├── drv_gpio.c/h       #   GPIO / PIN（中断 + 输出 + 输入）
│   ├── drv_usart.c/h      #   UART 串口（中断 + DMA）
│   ├── drv_spi.c/h        #   SPI（中断 + DMA）
│   ├── drv_hard_i2c.c/h   #   硬件 I2C（中断 + DMA）
│   ├── drv_tim.c/h        #   硬件定时器
│   ├── drv_pwm.c          #   PWM 输出
│   ├── drv_adc.c          #   ADC 采集
│   ├── drv_wdt.c          #   独立看门狗
│   ├── drv_rtc.c          #   实时时钟
│   ├── drv_dma.h          #   DMA 通道重映射
│   ├── drv_common.c/h     #   驱动公共层（SysTick、板级初始化）
│   ├── drv_config.h       #   驱动配置入口
│   └── config/py32f4/     #   外设实例配置（SPI/I2C/UART/TIM/PWM/ADC/DMA）
├── sdk/                   # PY32F403 HAL 固件库 + CMSIS
├── app/main.c             # 应用层（Shell 测试命令集）
├── scripts/post_build.lua # 构建后处理（hex/bin + 内存分析）
├── dist/                  # 编译产物（elf/hex/bin/lst）
├── .vscode/               # VSCode 调试与任务配置
└── docs/                  # 文档
```

## Shell 命令一览

| 命令 | 功能 |
|------|------|
| `help` | 列出所有可用命令 |
| `ps` | 显示线程列表 |
| `free` | 显示内存使用情况 |
| `list` | 列出内核对象 |
| `cmd_bsp_probe` | 探测所有 BSP 设备是否就绪 |
| `cmd_dma_probe` | 全面探测 SPI/I2C/UART 的 DMA 通路 |
| `cmd_spi_flash` | 读取 W25Q128 JEDEC ID |
| `cmd_i2c_scan` | 扫描 I2C2 总线上的设备地址 |
| `cmd_xn297` | 读取 XN297L 射频芯片寄存器 |
| `cmd_ws2812 <序号> <颜色>` | 设置指定 WS2812 灯珠颜色，如 `cmd_ws2812 0 FF0000` |
| `cmd_ws2812_breath [颜色]` | WS2812 呼吸灯效果，再执行一次停止 |

## 构建配置

| 项目 | 配置 |
|------|------|
| 工具链 | `arm-none-eabi-gcc` 15.2.1 (Arm GNU Toolchain) |
| 目标架构 | Cortex-M4, Thumb-2 指令集 |
| 浮点 | `-mfpu=fpv4-sp-d16 -mfloat-abi=hard` |
| C 标准 | GNU C11 |
| 优化 | Debug: `-Og -g`, Release: `-Os` |
| 链接脚本 | `bsp/linker_scripts/link.lds` |

## 当前资源占用

| 项目 | 占用 | 总量 | 占比 |
|------|------|------|------|
| ROM（Flash） | ~75 KB | 384 KB | 20% |
| RAM（SRAM） | ~6 KB | 64 KB | 10% |

## 如何添加新的外设

1. 在 `os/rtconfig.h` 中添加对应的 `BSP_USING_XXX` 宏
2. 对应驱动配置文件已在 `drivers/config/py32f4/` 就绪
3. 外设 MSP 初始化在 `bsp/CubeMX_Config/Src/py32f403_hal_msp.c`
4. Shell 测试命令可在 `app/main.c` 中添加

## 移植过程中的关键修复

| 序号 | 问题 | 根因 | 修复 |
|------|------|------|------|
| 1 | 启动 HardFault | `__libc_init_array` 遍历到 NULL 函数指针 | 注释掉启动文件中的调用 |
| 2 | 调度器崩溃 | SysTick 在调度器未就绪时触发 | 延迟到 `INIT_DEVICE_EXPORT` 阶段使能 |
| 3 | GCC 入口错误 | `main()` 覆盖了 RTT 的启动 `main()` | 改用 `entry()` + `-Wl,-e,entry` |
| 4 | FPU 指令未定义异常 | 启动文件未使能 FPU | Reset_Handler 中配置 CPACR |
| 5 | 波特率错误 (461k 而非 115.2k) | `SystemCoreClockUpdate()` PLLSRC 位比较永远为假 | `== 0x01` → `!= 0U` |
| 6 | 时钟计算偏差 | HSI PLL 计算多除了 2 (`HSI>>1`) | 改为 `HSI_VALUE * pllmul` |
| 7 | 驱动编译失败 | 7 个驱动文件残留 STM32 的 `SOC_SERIES_STM32*` 条件编译 | 全部替换为 `SOC_SERIES_PY32F4` |
| 8 | DMA 通道错误 | PY32F403 DMA 需要显式重映射 | 编写 `drv_dma.h` 调用 `LL_SYSCFG_SetDMARemap` |
| 9 | WS2812 不亮 | PWM 周期和占空比与验证工程不一致 | ARR 89→180, bit0/bit1 29/58→60/120, OCFastMode 开启 |

## 许可

- RT-Thread: Apache-2.0 License
- PY32 HAL 库: BSD 3-Clause (Puya Semiconductor)
