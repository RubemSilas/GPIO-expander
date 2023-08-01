#ifndef EXP_UTILS_H
#define EXP_UTILS_H

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"

#include "main.h"
#include "i2c.h"

#define EXP_UART_DEBUG_ENABLE

#ifdef EXP_UART_DEBUG_ENABLE
#include "usart.h"
#define UART_HANDLER huart2
#endif

// SYSTEM PIN PARAMETERS
#define TWO_DEFALT_CONFIG_MASK (0x03)
#define ONE_DEFALT_CONFIG_MASK (0x01)

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
#define MODER_SWCLK (2 << SWCLK_32_BIT_POS)     //(10 - alternative function mode)
#define MODER_SWDIO (2 << SWDIO_32_BIT_POS)     //(10 - alternative function mode)
#define MODER_I2C_SDA (2 << I2C_SDA_32_BIT_POS) //(10 - alternative function mode)
#define MODER_I2C_SCL (2 << I2C_SCL_32_BIT_POS) //(10 - alternative function mode)
#define MODER_UC_INT (1 << UC_INT_32_BIT_POS)   //(01 - General purpose output mode)
#define MODER_UART_RX (2 << UART_RX_32_BIT_POS) //(10 - alternative function mode)
#define MODER_UART_TX (2 << UART_TX_32_BIT_POS) //(10 - alternative function mode)

// 32 bits
#define OSPEEDR_SWCLK (1 << SWCLK_32_BIT_POS)     // (01 - Very high speed)
#define OSPEEDR_SWDIO (3 << SWDIO_32_BIT_POS)     // (11 - Very high speed)
#define OSPEEDR_I2C_SDA (1 << I2C_SDA_32_BIT_POS) // (01 - Low speed)
#define OSPEEDR_I2C_SCL (1 << I2C_SCL_32_BIT_POS) // (01 - Low speed)
#define OSPEEDR_UC_INT (1 << UC_INT_32_BIT_POS)   // (01 - Low speed)
#define OSPEEDR_UART_RX (1 << UART_RX_32_BIT_POS) // (01 - Low speed)
#define OSPEEDR_UART_TX (1 << UART_TX_32_BIT_POS) // (01 - Low speed)

// 16 bits
#define OTYPER_SWCLK (0 << SWCLK_16_BIT_POS)     //(0 - output push-pull)
#define OTYPER_SWDIO (0 << SWDIO_16_BIT_POS)     //(0 - output push-pull)
#define OTYPER_I2C_SDA (1 << I2C_SDA_16_BIT_POS) //(1 - output open-drain)
#define OTYPER_I2C_SCL (1 << I2C_SCL_16_BIT_POS) //(1 - output open-drain)
#define OTYPER_UC_INT (0 << UC_INT_16_BIT_POS)   //(0 - output push-pull)
#define OTYPER_UART_RX (0 << UART_RX_16_BIT_POS) //(0 - output push-pull)
#define OTYPER_UART_TX (0 << UART_TX_16_BIT_POS) //(0 - output push-pull)

// 32 bits
#define PUPDR_SWCLK (2 << SWCLK_32_BIT_POS)     //(10 - pull-down)
#define PUPDR_SWDIO (1 << SWDIO_32_BIT_POS)     //(01 - pull-up)
#define PUPDR_I2C_SDA (0 << I2C_SDA_32_BIT_POS) //(0 - no pull-up, pull-down)
#define PUPDR_I2C_SCL (0 << I2C_SCL_32_BIT_POS) //(0 - no pull-up, pull-down)
#define PUPDR_UC_INT (0 << UC_INT_32_BIT_POS)   //(0 - no pull-up, pull-down)
#define PUPDR_UART_RX (0 << UART_RX_32_BIT_POS) //(0 - no pull-up, pull-down)
#define PUPDR_UART_TX (0 << UART_TX_32_BIT_POS) //(0 - no pull-up, pull-down)

#define IT_SET_UC_PIN_POS (8)
#define IT_RESET_UC_PIN_POS (24)

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

    // =============== EXP IT ===============
    EXP_IO_PORT_A_INT_MASK,
    EXP_IO_PORT_B_INT_MASK,
    EXP_IO_PORT_C_INT_MASK,

    EXP_IO_PORT_A_INT_ENABLE,
    EXP_IO_PORT_B_INT_ENABLE,
    EXP_IO_PORT_C_INT_ENABLE,

    EXP_IO_PORT_A_INT_CAPTURE,
    EXP_IO_PORT_B_INT_CAPTURE,
    EXP_IO_PORT_C_INT_CAPTURE,

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

#ifndef EXP_UART_DEBUG_ENABLE
#define print_register(reg) ;
#define print_STM_reg(stm_reg) ; 
#define print_virtual_reg(virtual_reg) ; 
#else
void print_register(uint32_t reg); 
void print_STM_reg(uint32_t stm_reg); 
void print_virtual_reg(uint32_t virtual_reg); 
#endif

uint32_t switch_endianes(uint32_t target_value);

#endif