#ifndef EXP_UTILS_H
#define EXP_UTILS_H

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"

#include "main.h"
#include "i2c.h"
#include "usart.h"

#define UART_HANDLER huart2
#define I2C_HANDLER hi2c1

#define UART_TIMEOUT (100) // milisegundos
#define I2C_TIMEOUT (100)  // milisegundos

// COMUNICATION RELATED
#define I2C_WRITE_OPERATION 0
#define I2C_READ_OPERATION 1

// MACROS
#define SLAVE_SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define SLAVE_CLR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))
#define SLAVE_READ_BIT(reg, bit) (((reg) & ((1) << (bit))) >> bit) // retorna valor booleano

// MSG RELATED
#define BYTE_MASK (0xFF)
#define EXP_RW_BIT_POS (24)
#define EXP_REG_POS (16)
#define EXP_DATA_POS (0)

#endif