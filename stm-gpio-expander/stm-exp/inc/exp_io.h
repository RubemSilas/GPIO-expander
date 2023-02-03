#ifndef EXP_GPIOS_H
#define EXP_GPIOS_H

#include "utils.h"
#include "gpio.h"

// PINOS
enum exp_pins_e
{
    EXP_IO_PIN_0,
    EXP_IO_PIN_1,
    EXP_IO_PIN_2,
    EXP_IO_PIN_3,
    EXP_IO_PIN_4,
    EXP_IO_PIN_5,
    EXP_IO_PIN_6,
    EXP_IO_PIN_7,
    EXP_IO_PIN_8,
    EXP_IO_PIN_9,
    EXP_IO_PIN_10,
    EXP_IO_PIN_11,
    EXP_IO_PIN_12,
    EXP_IO_PIN_13,
    EXP_IO_PIN_14,
    EXP_IO_PIN_15,
};

// CONFIGURAÇÕES DE PINOS
typedef enum
{
    EXP_INPUT_MODE,
    EXP_OUTPUT_MODE,
} exp_dir_t;

typedef enum
{
    EXP_OUT_VERY_LOW,
    EXP_OUT_LOW,
    EXP_OUT_HIGH,
    EXP_OUT_VERY_HIGH,
}exp_output_speed_t;

typedef enum
{
    EXP_FLOATING_INPUT,
    EXP_REFERENCED_INPUT,
}exp_input_mode_t;

typedef enum
{
    EXP_PULL_UP = 1,
    EXP_PULL_DOWN,
}exp_input_reference_t;

// ESTRUTURAS AUXILIARES PARA REGISTRATODORES EXTENDIDOS
/*
    Alguns registradores de configuracao precisao de 2 bits para sua configuracao
    por esse motivo, as estruturas auxiliares servem para adaptar o conteudo do
    registrador virtual para o conteudo do registrador do STM32.
*/
enum reg_pos_e 
{
    EXTENDED_REG_POS_0 = 0,
    EXTENDED_REG_POS_1 = 2,
    EXTENDED_REG_POS_2 = 4,
    EXTENDED_REG_POS_3 = 6,
    EXTENDED_REG_POS_4 = 8,
    EXTENDED_REG_POS_5 = 10,
    EXTENDED_REG_POS_6 = 12,
    EXTENDED_REG_POS_7 = 14,
    EXTENDED_REG_POS_8 = 16,
    EXTENDED_REG_POS_9 = 18,
    EXTENDED_REG_POS_10 = 20,
    EXTENDED_REG_POS_11 = 22,
    EXTENDED_REG_POS_12 = 24,
    EXTENDED_REG_POS_13 = 26,
    EXTENDED_REG_POS_14 = 28,
    EXTENDED_REG_POS_15 = 30,
};

typedef struct
{
    unsigned extended_cfg_content_0 : 2;
    unsigned extended_cfg_content_1 : 2;
    unsigned extended_cfg_content_2 : 2;
    unsigned extended_cfg_content_3 : 2;
    unsigned extended_cfg_content_4 : 2;
    unsigned extended_cfg_content_5 : 2;
    unsigned extended_cfg_content_6 : 2;
    unsigned extended_cfg_content_7 : 2;
    unsigned extended_cfg_content_8 : 2;
    unsigned extended_cfg_content_9 : 2;
    unsigned extended_cfg_content_10 : 2;
    unsigned extended_cfg_content_11 : 2;
    unsigned extended_cfg_content_12 : 2;
    unsigned extended_cfg_content_13 : 2;
    unsigned extended_cfg_content_14 : 2;
    unsigned extended_cfg_content_15 : 2;
}extended_reg_content_t;

// ESCRITA
void exp_direction_config(uint16_t virtual_reg, exp_ports_t port);
void exp_out_mode_config(uint16_t virtual_reg, exp_ports_t port);
void exp_gpio_state_config(uint16_t virtual_reg, exp_ports_t port);

// LEITURA
uint16_t read_stm_reg(exp_registers_addr_t reg_name, exp_ports_t port);

// INICIALIZACAO
void exp_init_gpio_clks(void);

#endif