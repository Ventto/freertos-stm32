#ifndef STM32F4XX_L3GD20_H
#define STM32F4XX_L3GD20_H

#include <stdint.h>

void init_spi(void);

void write_spi_register(uint8_t register_addr, uint8_t reg_value);

uint8_t read_spi_register(uint8_t register_addr);

#endif /* !STM32F4XX_L3GD20_H */
