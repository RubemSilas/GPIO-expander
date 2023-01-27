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

#pragma pack(push)
#pragma pack(1)
typedef struct
{
    uint8_t rw;
    uint8_t exp_register;
    uint16_t exp_data;
} i2c_msg_t;

typedef enum
{
    // registradores de direcao
    EXP_IO_DIR_A_REG,
    EXP_IO_DIR_B_REG,
    EXP_IO_DIR_C_REG,

    // modo de saida
    EXP_IO_OUTPUT_MODE_A_REG,
    EXP_IO_OUTPUT_MODE_B_REG,
    EXP_IO_OUTPUT_MODE_C_REG,

    // entrada flutuante ou com referencia
    EXP_IO_INPUT_MODE_A_REG,
    EXP_IO_INPUT_MODE_B_REG,
    EXP_IO_INPUT_MODE_C_REG,

    // habilita pull up
    EXP_IO_INPUT_PULL_UP_A_REG,
    EXP_IO_INPUT_PULL_UP_B_REG,
    EXP_IO_INPUT_PULL_UP_C_REG,

    // habilita pull down
    EXP_IO_INPUT_PULL_DOWN_A_REG,
    EXP_IO_INPUT_PULL_DOWN_B_REG,
    EXP_IO_INPUT_PULL_DOWN_C_REG,

    // habilita pull down
    EXP_IO_INPUT_INVERT_POL_A_REG,
    EXP_IO_INPUT_INVERT_POL_B_REG,
    EXP_IO_INPUT_INVERT_POL_C_REG,

    // registrtadores de GPIO
    EXP_IO_GPIO_A_REG,
    EXP_IO_GPIO_B_REG,
    EXP_IO_GPIO_C_REG,

    EXP_TOTAL_REGISTERS,
} exp_registers_addr_t;

typedef struct
{
    exp_registers_addr_t reg_name;
    uint16_t reg_content;
} genereric_reg_t;
#pragma pack(pop)

#endif