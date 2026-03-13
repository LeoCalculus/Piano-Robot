
1. NVIC interrupt is different from those interrupt function, e.g. we never use HAL_UART_TRANSMIT_IT with DMA using them will get error
2. DMA in this MCU always need to config both cube side and code side, Code side should use START_DMA(&Target place) to link the place and in Cube side should check from where to where: from memory to memory and source / destination increment.
3. Flash elf with specific destination: make -f flash.mk load ELF= <target destination>


