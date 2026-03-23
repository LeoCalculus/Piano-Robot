
1. NVIC interrupt is different from those interrupt function, e.g. we never use HAL_UART_TRANSMIT_IT with DMA using them will get error
2. DMA in this MCU always need to config both cube side and code side, Code side should use START_DMA(&Target place) to link the place and in Cube side should check from where to where: from memory to memory and source / destination increment.
3. Flash elf with specific destination: make -f flash.mk load ELF= <target destination>


Error: critical data lost, while sending from VOFA to MCU

Test for UART:


1. FIFO enabled
2. disable the half / full interrupt


Missing data due to uart losing packet, does not affect what it actually did.


Fixed using VOFA sending data, reason: the DMA chunk save the latest data wrap around length, did not save the full length, when error length passed to command system, the incorrect length leads to data loss and command error.



