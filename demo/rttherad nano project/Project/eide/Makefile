#################################################################
# AUTO GENERATE AT 2026-03-05 19:42:20 BY unify_builder v3.11.0.0
#################################################################

# Usage:
#  - Build:	make prebuild && make COMPILER_DIR=<dir path>
#  - Clean:	make clean

# Targets Dependences Chain:
#  prebuild
#  all -> postbuild -> bin -> elf -> *.o

# Use 'make V=1' to see the full commands
ifdef V
	Q = 
else
	Q = @
endif

COMPILER_DIR ?= /D/APP/Keil_v5/ARM/ARMCLANG
ifneq ($(COMPILER_DIR),)
	_PATH_TMP:=$(COMPILER_DIR)/bin:$(PATH)
	export PATH=$(_PATH_TMP)
endif

EXE?=
CC=$(COMPILER_DIR)/bin/armclang$(EXE)
AS=$(COMPILER_DIR)/bin/armasm$(EXE)
CXX=$(COMPILER_DIR)/bin/armclang$(EXE)
LD=$(COMPILER_DIR)/bin/armlink$(EXE)
AR=$(COMPILER_DIR)/bin/armar$(EXE)

###########################
# targets
###########################

COLOR_END=\033[0m
COLOR_ERR=\033[31;1m
COLOR_WRN=\033[33;1m
COLOR_SUC=\033[32;1m
COLOR_INF=\033[34;1m

.PHONY : all postbuild bin elf prebuild clean

all: postbuild
	@echo ==========
	@printf "$(COLOR_SUC)ALL DONE.$(COLOR_END)\n"
	@echo ==========

clean:
	-rm -fv ./build/Target 1/*.elf ./build/Target 1/*.axf ./build/Target 1/*.out
	-rm -fv ./build/Target 1/*.hex ./build/Target 1/*.bin ./build/Target 1/*.s19
	-rm -rfv ./build/Target 1/.obj

prebuild:
	@printf "$(COLOR_INF)prebuild ...$(COLOR_END)\n"
	@echo nothing to prebuild.

"./build/Target 1/.obj/__/__/Application/Src:
	$(Q)mkdir -p $@
-include "./build/Target 1/.obj/__/__/Application/Src/main.d
"./build/Target 1/.obj/__/__/Application/Src/main.o": ./../../Application/Src/main.c Makefile | "./build/Target 1/.obj/__/__/Application/Src
	@echo compiling $< ...
	$(Q)$(CC) --target=arm-arm-none-eabi -c -xc -std=c11 -I. -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Include -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include -I../../../BSP/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc -I../../Application/Inc -I../../../BSP/py32_drivers/Inc -I../../../BSP -I../../../BSP/example/uart -I../../../BSP/example/gpio -I../../../BSP/example/osal -I../../../rtthread-nano/src -I../../../rtthread-nano/include -I../../../rtthread-nano/libcpu/arm/cortex-m4 -I../../../rtthread-nano -I.cmsis/include -I"../MDK-ARM/RTE/_Target 1" -D"USE_HAL_DRIVER" -D"PY32F403xD" -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mlittle-endian -D__MICROLIB -O0 -ffunction-sections -funsigned-char -fshort-enums -fshort-wchar -fno-rtti -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality -g -o "./build/Target 1/.obj/__/__/Application/Src/main.o" -MMD ./../../Application/Src/main.c

-include "./build/Target 1/.obj/__/__/Application/Src/py32f403_hal_msp.d
"./build/Target 1/.obj/__/__/Application/Src/py32f403_hal_msp.o": ./../../Application/Src/py32f403_hal_msp.c Makefile | "./build/Target 1/.obj/__/__/Application/Src
	@echo compiling $< ...
	$(Q)$(CC) --target=arm-arm-none-eabi -c -xc -std=c11 -I. -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Include -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include -I../../../BSP/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc -I../../Application/Inc -I../../../BSP/py32_drivers/Inc -I../../../BSP -I../../../BSP/example/uart -I../../../BSP/example/gpio -I../../../BSP/example/osal -I../../../rtthread-nano/src -I../../../rtthread-nano/include -I../../../rtthread-nano/libcpu/arm/cortex-m4 -I../../../rtthread-nano -I.cmsis/include -I"../MDK-ARM/RTE/_Target 1" -D"USE_HAL_DRIVER" -D"PY32F403xD" -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mlittle-endian -D__MICROLIB -O0 -ffunction-sections -funsigned-char -fshort-enums -fshort-wchar -fno-rtti -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality -g -o "./build/Target 1/.obj/__/__/Application/Src/py32f403_hal_msp.o" -MMD ./../../Application/Src/py32f403_hal_msp.c

-include "./build/Target 1/.obj/__/__/Application/Src/py32f403_it.d
"./build/Target 1/.obj/__/__/Application/Src/py32f403_it.o": ./../../Application/Src/py32f403_it.c Makefile | "./build/Target 1/.obj/__/__/Application/Src
	@echo compiling $< ...
	$(Q)$(CC) --target=arm-arm-none-eabi -c -xc -std=c11 -I. -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Include -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include -I../../../BSP/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc -I../../Application/Inc -I../../../BSP/py32_drivers/Inc -I../../../BSP -I../../../BSP/example/uart -I../../../BSP/example/gpio -I../../../BSP/example/osal -I../../../rtthread-nano/src -I../../../rtthread-nano/include -I../../../rtthread-nano/libcpu/arm/cortex-m4 -I../../../rtthread-nano -I.cmsis/include -I"../MDK-ARM/RTE/_Target 1" -D"USE_HAL_DRIVER" -D"PY32F403xD" -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mlittle-endian -D__MICROLIB -O0 -ffunction-sections -funsigned-char -fshort-enums -fshort-wchar -fno-rtti -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality -g -o "./build/Target 1/.obj/__/__/Application/Src/py32f403_it.o" -MMD ./../../Application/Src/py32f403_it.c

-include "./build/Target 1/.obj/__/__/Application/Src/startup_py32f403xx.d
"./build/Target 1/.obj/__/__/Application/Src/startup_py32f403xx.o": ./../../Application/Src/startup_py32f403xx.s Makefile | "./build/Target 1/.obj/__/__/Application/Src
	@echo assembling $< ...
	$(Q)$(AS) --diag_suppress=A1950 -I. -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Include -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include -I../../../BSP/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc -I../../Application/Inc -I../../../BSP/py32_drivers/Inc -I../../../BSP -I../../../BSP/example/uart -I../../../BSP/example/gpio -I../../../BSP/example/osal -I../../../rtthread-nano/src -I../../../rtthread-nano/include -I../../../rtthread-nano/libcpu/arm/cortex-m4 -I../../../rtthread-nano -I.cmsis/include -I"../MDK-ARM/RTE/_Target 1" --cpu Cortex-M4.fp --li --pd "__MICROLIB SETA 1" -g -o "./build/Target 1/.obj/__/__/Application/Src/startup_py32f403xx.o" --depend "./build/Target 1/.obj/__/__/Application/Src/startup_py32f403xx.d" ./../../Application/Src/startup_py32f403xx.s

-include "./build/Target 1/.obj/__/__/Application/Src/system_py32f403.d
"./build/Target 1/.obj/__/__/Application/Src/system_py32f403.o": ./../../Application/Src/system_py32f403.c Makefile | "./build/Target 1/.obj/__/__/Application/Src
	@echo compiling $< ...
	$(Q)$(CC) --target=arm-arm-none-eabi -c -xc -std=c11 -I. -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Include -I../../../BSP/PY32f403_Firmware_Library/CMSIS/Device/PUYA/PY32F403/Include -I../../../BSP/PY32f403_Firmware_Library/PY32F403_HAL_Driver/Inc -I../../Application/Inc -I../../../BSP/py32_drivers/Inc -I../../../BSP -I../../../BSP/example/uart -I../../../BSP/example/gpio -I../../../BSP/example/osal -I../../../rtthread-nano/src -I../../../rtthread-nano/include -I../../../rtthread-nano/libcpu/arm/cortex-m4 -I../../../rtthread-nano -I.cmsis/include -I"../MDK-ARM/RTE/_Target 1" -D"USE_HAL_DRIVER" -D"PY32F403xD" -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4 -mlittle-endian -D__MICROLIB -O0 -ffunction-sections -funsigned-char -fshort-enums -fshort-wchar -fno-rtti -Wno-packed -Wno-missing-variable-declarations -Wno-missing-prototypes -Wno-missing-noreturn -Wno-sign-conversion -Wno-nonportable-include-path -Wno-reserved-id-macro -Wno-unused-macros -Wno-documentation-unknown-command -Wno-documentation -Wno-license-management -Wno-parentheses-equality -g -o "./build/Target 1/.obj/__/__/Application/Src/system_py32f403.o" -MMD ./../../Application/Src/system_py32f403.c

objs = "./build/Target 1/.obj/__/__/Application/Src/main.o" "./build/Target 1/.obj/__/__/Application/Src/py32f403_hal_msp.o" "./build/Target 1/.obj/__/__/Application/Src/py32f403_it.o" "./build/Target 1/.obj/__/__/Application/Src/startup_py32f403xx.o" "./build/Target 1/.obj/__/__/Application/Src/system_py32f403.o"
elf: $(objs) Makefile
	@printf "$(COLOR_INF)linking "./build/Target 1/PY32F403_Project.axf" ...$(COLOR_END)\n"
	$(LD) --cpu Cortex-M4.fp --library_type=microlib --scatter "build/Target 1/PY32F403_Project.sct" --diag_suppress=L6329 --strict --summary_stderr --info summarysizes --map --xref --callgraph --symbols --info sizes --info totals --info unused --info veneers --list "./build/Target 1/PY32F403_Project.map" "./build/Target 1/.obj/__/__/Application/Src/main.o" "./build/Target 1/.obj/__/__/Application/Src/py32f403_hal_msp.o" "./build/Target 1/.obj/__/__/Application/Src/py32f403_it.o" "./build/Target 1/.obj/__/__/Application/Src/startup_py32f403xx.o" "./build/Target 1/.obj/__/__/Application/Src/system_py32f403.o" -o "./build/Target 1/PY32F403_Project.axf"

bin: elf Makefile
	@printf "$(COLOR_INF)output binary files ...$(COLOR_END)\n"
	$(COMPILER_DIR)/bin/fromelf$(EXE) --i32combined --output "./build/Target 1/PY32F403_Project.hex" "./build/Target 1/PY32F403_Project.axf"
	$(COMPILER_DIR)/bin/fromelf$(EXE) --m32combined --output "./build/Target 1/PY32F403_Project.s19" "./build/Target 1/PY32F403_Project.axf"

postbuild: bin
	@printf "$(COLOR_INF)postbuild ...$(COLOR_END)\n"
	axf2elf -d "$(COMPILER_DIR)" -i "build/Target 1/PY32F403_Project.axf" -o "build/Target 1/PY32F403_Project.elf" > "build/Target 1/axf2elf.log"

