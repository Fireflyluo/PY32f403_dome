@echo off
openocd -s ./scripts -f ./scripts/interface/cmsis-dap.cfg -f ./scripts/target/py32f002b.cfg
pause