CC:=arm-none-eabi-gcc
AS:=arm-none-eabi-as
GDB:=arm-none-eabi-gdb

BINDIR:=bin
BINELF:=out.elf

OPENOCD:=/usr/bin/openocd
OPENOCD_ROOT:=/usr/share/openocd

CPPFLAGS+=-DUSE_STDPERIPH_DRIVER \
		 -DSTM32F4XX \
		 -DSTM32F429_439xx \
		 -DUSE_DEFAULT_TIMEOUT_CALLBACK \
		 -DUSE_STM32F4_DISCOVERY

CFLAGS+=-mthumb -mcpu=cortex-m4 -nostdlib \
	   -Ifreertos/src/include \
	   -Ifreertos/src/portable/GCC \
	   -Istm32f4xx/CMSIS \
	   -Istm32f4xx/CMSIS/CM4/CoreSupport \
	   -Istm32f4xx/CMSIS/Device/ST/STM32F4xx/Include \
	   -Istm32f4xx/Config \
	   -Istm32f4xx/STM32F4xx_StdPeriph_Driver/inc \
	   -fdata-sections \
	   -ffunction-sections \
	   -mabi=aapcs \
	   -mfloat-abi=softfp \
	   -mfpu=fpv4-sp-d16 \
	   -mlittle-endian

LDFLAGS=-mcpu=cortex-m4 -mthumb -T stm32f4xx/stm32fx.ld
LDFLAGS+=--specs=nosys.specs --specs=nano.specs

OBJS= \
      freertos/src/timers.o \
      freertos/src/queue.o \
      freertos/src/tasks.o \
      freertos/src/portable/GCC/port.o \
      freertos/src/portable/MemMang/heap_1.o \
      freertos/src/croutine.o \
      freertos/src/list.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_sha1.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_crc.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_ltdc.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_iwdg.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rng.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dbgmcu.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_can.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sai.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_wwdg.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma2d.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_aes.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/misc.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_md5.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_tdes.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sdio.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dcmi.o \
      stm32f4xx/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_des.o \
      stm32f4xx/STM32F429I-Discovery/stm32f429i_discovery_l3gd20.o \
      stm32f4xx/STM32F429I-Discovery/stm32f429i_discovery_sdram.o \
      stm32f4xx/STM32F429I-Discovery/stm32f429i_discovery.o \
      stm32f4xx/STM32F429I-Discovery/stm32f429i_discovery_i2c_ee.o \
      stm32f4xx/STM32F429I-Discovery/stm32f429i_discovery_ioe.o \
      stm32f4xx/STM32F429I-Discovery/stm32f429i_discovery_lcd.o \
      src/hooks_FreeRTOS.o \
      src/system_stm32f4xx.o \
      src/newlibC_os_interface.o \
      src/stm32f4xx_it.o \
      src/usart.o \
      src/stm32f4xx_adc.o \
      src/stm32f4xx_l3gd20.o \
      src/main.o \
      src/startup_stm32f4xx.o

.PHONY: all clean debug gdb make_s

all: $(BINDIR)/$(BINELF)

$(BINDIR)/$(BINELF): make_s $(OBJS) | $(BINDIR)
	$(LINK.o) $(OUTPUT_OPTION) $(OBJS)

$(BINDIR):
	mkdir -p $@

debug: all

debug: CFLAGS+=-g -O0
debug: LDFLAGS+=-g -O0

deploy: all
	$(OPENOCD) -f "$(OPENOCD_ROOT)/scripts/board/stm32f4discovery.cfg" \
		-c "program $(BINDIR)/$(BINELF) verify reset exit"

connect: debug
	$(OPENOCD) -f "$(OPENOCD_ROOT)/scripts/board/stm32f4discovery.cfg" \
		-c "init" -c "reset halt"

.gdbscript:
	@echo "target remote localhost:3333" > .gdbscript
	@echo "monitor reset halt" >> .gdbscript
	@echo "monitor flash protect 0 0 11 off" >> .gdbscript
	@echo "monitor flash write_image erase \"$(shell realpath $(BINDIR)/$(BINELF))\"" >> .gdbscript
	@echo "monitor reset halt" >> .gdbscript
	@echo "file $(BINDIR)/$(BINELF)" >> .gdbscript
	@echo "load" >> .gdbscript
	@echo "break main.c:main" >> .gdbscript
	@echo "tabset" >> .gdbscript
	@echo "continue" >> .gdbscript

gdb: debug | .gdbscript
	$(GDB) -command=.gdbscript

clean:
	$(RM) $(OBJS)
	$(RM) -r $(BINDIR)
