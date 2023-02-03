#include "exp_io.h"


// FUNCOES DE ESCRITA DE REGISTRADOR ==============================================================================
void exp_direction_config(uint16_t virtual_reg, exp_ports_t port)
{
    char log_msg[20];
    uint32_t direction_reg_cfg = 0;
    extended_reg_content_t direction_cfg_buffer =
        {
            .extended_cfg_content_0 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_0)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_1 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_1)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_2 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_2)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_3 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_3)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_4 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_4)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_4 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_4)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_5 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_5)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_6 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_6)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_7 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_7)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_8 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_8)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_9 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_9)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_10 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_10)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_11 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_11)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_12 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_12)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_13 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_13)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_14 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_14)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
            .extended_cfg_content_15 = (SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_15)) ? EXP_OUTPUT_MODE : EXP_INPUT_MODE,
        };

    direction_reg_cfg = (uint32_t)((direction_cfg_buffer.extended_cfg_content_15 << EXTENDED_REG_POS_15) |
                                   (direction_cfg_buffer.extended_cfg_content_14 << EXTENDED_REG_POS_14) |
                                   (direction_cfg_buffer.extended_cfg_content_13 << EXTENDED_REG_POS_13) |
                                   (direction_cfg_buffer.extended_cfg_content_12 << EXTENDED_REG_POS_12) |
                                   (direction_cfg_buffer.extended_cfg_content_11 << EXTENDED_REG_POS_11) |
                                   (direction_cfg_buffer.extended_cfg_content_10 << EXTENDED_REG_POS_10) |
                                   (direction_cfg_buffer.extended_cfg_content_9 << EXTENDED_REG_POS_9) |
                                   (direction_cfg_buffer.extended_cfg_content_8 << EXTENDED_REG_POS_8) |
                                   (direction_cfg_buffer.extended_cfg_content_7 << EXTENDED_REG_POS_7) |
                                   (direction_cfg_buffer.extended_cfg_content_6 << EXTENDED_REG_POS_6) |
                                   (direction_cfg_buffer.extended_cfg_content_5 << EXTENDED_REG_POS_5) |
                                   (direction_cfg_buffer.extended_cfg_content_4 << EXTENDED_REG_POS_4) |
                                   (direction_cfg_buffer.extended_cfg_content_3 << EXTENDED_REG_POS_3) |
                                   (direction_cfg_buffer.extended_cfg_content_2 << EXTENDED_REG_POS_2) |
                                   (direction_cfg_buffer.extended_cfg_content_1 << EXTENDED_REG_POS_1) |
                                   (direction_cfg_buffer.extended_cfg_content_0 << EXTENDED_REG_POS_0));

    sprintf(log_msg, "virtual reg: ");
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
    print_register(virtual_reg);

    sprintf(log_msg, "STM32 reg: ");
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
    print_register(direction_reg_cfg);

    switch (port)
    {
    case EXP_PORT_A:
        GPIOA->MODER = direction_reg_cfg;
        break;
    case EXP_PORT_B:
        GPIOB->MODER = direction_reg_cfg;
        break;
    case EXP_PORT_C:
        GPIOC->MODER = direction_reg_cfg;
        break;
    default:
        break;
    };
}

void exp_out_mode_config(uint16_t virtual_reg, exp_ports_t port)
{
    uint32_t out_mode_reg_cfg = 0;

    out_mode_reg_cfg = virtual_reg;
    // TODO: tratar pinos que podem ser alterados (pinoo I2C e UART)

    switch (port)
    {
    case EXP_PORT_A:
        GPIOA->OTYPER = out_mode_reg_cfg;
        break;
    case EXP_PORT_B:
        GPIOB->OTYPER = out_mode_reg_cfg;
        break;
    case EXP_PORT_C:
        GPIOC->OTYPER = out_mode_reg_cfg;
        break;
    default:
        break;
    };
}


void exp_gpio_state_config(uint16_t virtual_reg, exp_ports_t port)
{
    uint32_t state_config = 0;

    for (int i = 15; i >= 0; i--)
    {
        ((virtual_reg >> i) & 1) ? SLAVE_SET_BIT(state_config, i) : SLAVE_SET_BIT(state_config, i + 16);
    }

    switch (port)
    {
    case EXP_PORT_A:
        GPIOA->BSRR = state_config;
        break;
    case EXP_PORT_B:
        GPIOB->BSRR = state_config;
        break;
    case EXP_PORT_C:
        GPIOC->BSRR = state_config;
        break;
    default:
        break;
    };
}

void exp_init_gpio_clks(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
}
