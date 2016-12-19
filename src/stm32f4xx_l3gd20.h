#ifndef STM32F4XX_L3GD20_H
#define STM32F4XX_L3GD20_H

#include <stdint.h>

void spi_init(void);

void spi_write_register(uint8_t register_addr, uint8_t reg_value);

uint8_t spi_read_register(uint8_t register_addr);

#endif /* !STM32F4XX_L3GD20_H */
