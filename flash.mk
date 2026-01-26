# Author: Liwei Xue 2024.3.20
# Flash script for makefile project, using OPENOCD.

SCRIPTS_PATH = C:/ST/STM32CubeIDE_2.0.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.debug.openocd_2.3.200.202510310951/resources/openocd/st_scripts

PROJECT = h533
TARGET = stm32h5x
INTERFACE = stlink-dap

OCD = C:/ST/STM32CubeIDE_2.0.0/STM32CubeIDE/plugins/com.st.stm32cube.ide.mcu.externaltools.openocd.win32_2.4.300.202509300731/tools/bin/openocd
ELF = build/$(PROJECT).elf

default: load

load: 
	$(OCD) -s $(SCRIPTS_PATH) \
	 -f interface/$(INTERFACE).cfg \
	 -c "transport select dapdirect_swd" \
	 -f target/$(TARGET).cfg \
	 -c "program $(ELF) verify reset exit"

build_code:
	make -C ./ -j

showpath:
	@echo Scripts: $(SCRIPTS_PATH)

.PHONY: build_code load