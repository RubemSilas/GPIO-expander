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


#endif