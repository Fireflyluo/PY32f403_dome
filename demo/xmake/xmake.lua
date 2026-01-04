-- 设置项目名称
set_project("arm_m4_project")

add_rules("mode.debug", "mode.release")

-- 设置交叉编译工具链和目标平台
set_plat("cross")
set_arch("arm")
-- set_toolchains("gnu-rm")
-- 设置交叉编译工具链路径
set_toolchains("gnu-rm", {sdkdir = "D:/APP/path/arm-gcc-14.2"})

-- 设置语言标准
set_languages("c11", "c++17")
-- 自动生成 VSCode 的 compile_commands.json 文件
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

-- 添加脚本目录到模块搜索路径
add_moduledirs("scripts")
-- 添加自定义构建后处理任务
-- task("post-build")
--     on_run(function ()
--         import("core.project.project")
--         local target = project.target("arm_mcu")
--         if target then
--             import("post_build")  -- 从 scripts 目录导入
--             post_build.main(target)
--         else
--             print("错误: 找不到目标 'arm_mcu'")
--         end
--     end)

--     set_menu {
--         usage = "xmake post-build",
--         description = "执行构建后处理(生成bin/hex,显示内存使用情况)"
--     }




-- 定义目标
target("arm_mcu")
    set_kind("binary")
    
    -- 添加文件
    add_files("startup/startup_py32f403xx.s")
    add_files("src/*.c")
    add_files("src/syscalls.c")
    add_files("BSP/py32_drivers/Src/*.c")
    add_files("BSP/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Src/*.c")
    
    -- 包含路径
    add_includedirs("inc")
    add_includedirs("BSP/py32_drivers")
    add_includedirs("BSP/py32_drivers/Inc")
    add_includedirs("BSP/PY32f403_Firmware_Library/CMSIS/Include")
    add_includedirs("BSP/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc")
    add_includedirs("BSP/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include")
    
    -- 预处理器定义
    add_defines("ARM_MATH_CM4", "USE_STDPERIPH_DRIVER", "PY32F403xD")
    
    -- 编译选项
    add_cflags("-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=hard",
               "-Wall", "-Wextra", "-Wno-unused-parameter", 
               "-ffunction-sections", "-fdata-sections",
               {force = true})
    
    add_cxflags("-std=c11", {force = true})
    
    -- 调试选项
    if is_mode("debug") then
        add_cflags("-g", "-O0", {force = true})
        add_defines("DEBUG=1")
    else
        add_cflags("-Os", {force = true})
        add_defines("NDEBUG=1")
    end
    
    -- 链接选项
    add_ldflags("-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=hard",
                "-Wl,--gc-sections",
                {force = true})
    
    add_ldflags("-Wl,--no-warn-rwx-segments", {force = true})
    add_ldflags("-Tldscripts/py32f403xd.ld", {force = true})
    
    -- 系统库
    add_ldflags("-lc", "-lm", "-lnosys", {force = true})
    
    -- 浮点支持
    add_ldflags("-u_printf_float", "-u_scanf_float", {force = true})
    



  -- 构建后处理 - 调用外部脚本
    -- after_build(function (target)
    --     import("post_build")
    --     post_build.main(target)
    -- end)

   