#ifndef EXP_UTILS_H
#define EXP_UTILS_H

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"

#include "main.h"
#include "i2c.h"
#include "usart.h"

// SYSTEM PIN PARAMETERS
#define TWO_DEFALT_CONFIG_MASK  0x03
#define ONE_DEFALT_CONFIG_MASK  0x01

// DEFAULT PIN CONFIG REG POSITION
#define SWCLK_32_BIT_POS (28)
#define SWDIO_32_BIT_POS (26)
#define I2C_SDA_32_BIT_POS (20)
#define I2C_SCL_32_BIT_POS (18)
#define UC_INT_32_BIT_POS (16)
#define UART_RX_32_BIT_POS (6)
#define UART_TX_32_BIT_POS (4)

#define SWCLK_16_BIT_POS (14)
#define SWDIO_16_BIT_POS (13)
#define I2C_SDA_16_BIT_POS (10)
#define I2C_SCL_16_BIT_POS (9)
#define UC_INT_16_BIT_POS (8)
#define UART_RX_16_BIT_POS (3)
#define UART_TX_16_BIT_POS (2)

// DEFAULT PIN CONFIG REG VALUES
// 32 bits
#define MODER_SWCLK (2 << 28)   //(10 - alternative function mode)
#define MODER_SWDIO (2 << 26)   //(10 - alternative function mode)
#define MODER_I2C_SDA (2 << 20) //(10 - alternative function mode)
#define MODER_I2C_SCL (2 << 18) //(10 - alternative function mode)
#define MODER_UC_INT (1 << 16)  //(01 - General purpose output mode)
#define MODER_UART_RX (2 << 6)  //(10 - alternative function mode)
#define MODER_UART_TX (2 << 4)  //(10 - alternative function mode)

// 16 bits
#define OTYPER_SWCLK (0 << 14)   //(0 - output push-pull)
#define OTYPER_SWDIO (0 << 13)   //(0 - output push-pull)
#define OTYPER_I2C_SDA (1 << 10) //(1 - output open-drain)
#define OTYPER_I2C_SCL (1 << 9)  //(1 - output open-drain)
#define OTYPER_UC_INT (0 << 8)   //(0 - output push-pull)
#define OTYPER_UART_RX (0 << 3)  //(0 - output push-pull)
#define OTYPER_UART_TX (0 << 2)  //(0 - output push-pull)

// 32 bits
#define PUPDR_SWCLK (2 << 28)   //(10 - pull-down)
#define PUPDR_SWDIO (1 << 26)   //(01 - pull-up)
#define PUPDR_I2C_SDA (0 << 20) //(0 - no pull-up, pull-down)
#define PUPDR_I2C_SCL (0 << 18) //(0 - no pull-up, pull-down)
#define PUPDR_UC_INT (0 << 16)  //(0 - no pull-up, pull-down)
#define PUPDR_UART_RX (0 << 6)  //(0 - no pull-up, pull-down)
#define PUPDR_UART_TX (0 << 4)  //(0 - no pull-up, pull-down)

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
#define SLAVE_READ_BIT(reg, bit) ((reg >> bit) & 1) // retorna valor booleano

// MSG RELATED
#define BYTE_MASK (0xFF)
#define HALF_WORD_MASK (0xFFFF)
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
    EXP_PORT_A,
    EXP_PORT_B,
    EXP_PORT_C,
}exp_ports_t;

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

    // habilita referencia do pino
    EXP_IO_INPUT_REF_A_REG,
    EXP_IO_INPUT_REF_B_REG,
    EXP_IO_INPUT_REF_C_REG,

    // inverte a polaridade do pino
    EXP_IO_INPUT_INVERT_POL_A_REG,
    EXP_IO_INPUT_INVERT_POL_B_REG,
    EXP_IO_INPUT_INVERT_POL_C_REG,

    // registrtadores de GPIO
    EXP_IO_GPIO_A_REG,
    EXP_IO_GPIO_B_REG,
    EXP_IO_GPIO_C_REG,

    EXP_TOTAL_REGISTERS,
} exp_registers_addr_t;

typedef void (*exp_operational_func_t)(uint16_t virtual_reg, exp_ports_t port);

typedef struct
{
    exp_registers_addr_t reg_name;
    exp_ports_t port_name;
    exp_operational_func_t op_func;
    uint16_t reg_content;
} genereric_reg_t;
#pragma pack(pop)

void print_register(uint32_t reg);

#endif