#include "exp_io.h"

// PILHA DE REGISTRADORES VIRTUAIS
genereric_reg_t current_scope_registers[EXP_TOTAL_REGISTERS] =
    {
        {.reg_name = EXP_IO_DIR_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_direction_config, .reg_content = 0},
        {.reg_name = EXP_IO_DIR_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_direction_config, .reg_content = 0},
        {.reg_name = EXP_IO_DIR_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_direction_config, .reg_content = 0},

        {.reg_name = EXP_IO_OUTPUT_MODE_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_out_mode_config, .reg_content = 0},
        {.reg_name = EXP_IO_OUTPUT_MODE_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_out_mode_config, .reg_content = 0},
        {.reg_name = EXP_IO_OUTPUT_MODE_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_out_mode_config, .reg_content = 0},

        {.reg_name = EXP_IO_INPUT_MODE_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_in_mode_config, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_MODE_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_in_mode_config, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_MODE_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_in_mode_config, .reg_content = 0},

        {.reg_name = EXP_IO_INPUT_REF_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_in_reference_config, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_REF_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_in_reference_config, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_REF_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_in_reference_config, .reg_content = 0},

        {.reg_name = EXP_IO_INPUT_INVERT_POL_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_inverted_pol_config, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_INVERT_POL_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_inverted_pol_config, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_INVERT_POL_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_inverted_pol_config, .reg_content = 0},

        {.reg_name = EXP_IO_GPIO_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_gpio_state_config, .reg_content = 0},
        {.reg_name = EXP_IO_GPIO_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_gpio_state_config, .reg_content = 0},
        {.reg_name = EXP_IO_GPIO_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_gpio_state_config, .reg_content = 0},
};

// HELPERS ========================================================================================================
static uint16_t long_to_short_reg(uint32_t long_value)
{
    uint16_t short_value = 0x00;

    for (int parse_step = 30; parse_step >= 0; parse_step -= 2)
    {
        int resulting_bit = ((long_value >> parse_step) & 0x03) ? 1 : 0;
        short_value = (short_value << 1) | resulting_bit;
    }
    return short_value;
}

static uint16_t inverted_pol_mask(uint16_t stm_read_reg, uint16_t virtual_pol_reg)
{
    uint16_t pol_reg_content = 0;
    pol_reg_content = (uint16_t)(((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_15)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_15))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_15))) << EXP_IO_PIN_15) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_14)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_14))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_14))) << EXP_IO_PIN_14) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_13)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_13))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_13))) << EXP_IO_PIN_13) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_12)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_12))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_12))) << EXP_IO_PIN_12) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_11)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_11))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_11))) << EXP_IO_PIN_11) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_10)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_10))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_10))) << EXP_IO_PIN_10) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_9)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_9))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_9))) << EXP_IO_PIN_9) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_8)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_8))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_8))) << EXP_IO_PIN_8) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_7)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_7))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_7))) << EXP_IO_PIN_7) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_6)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_6))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_6))) << EXP_IO_PIN_6) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_5)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_5))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_5))) << EXP_IO_PIN_5) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_4)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_4))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_4))) << EXP_IO_PIN_4) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_3)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_3))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_3))) << EXP_IO_PIN_3) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_2)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_2))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_2))) << EXP_IO_PIN_2) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_1)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_1))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_1))) << EXP_IO_PIN_1) |
                      (((SLAVE_READ_BIT(virtual_pol_reg, EXP_IO_PIN_0)) ? (1 ^ (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_0))) : (SLAVE_READ_BIT(stm_read_reg, EXP_IO_PIN_0))) << EXP_IO_PIN_0);
    return pol_reg_content;
}

static uint32_t MODER_default_pins_mask(uint32_t cfg_reg)
{
    uint32_t cfg_reg_masked = cfg_reg;

    cfg_reg_masked &= ~(TWO_DEFALT_CONFIG_MASK << SWCLK_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << SWDIO_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << I2C_SDA_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << I2C_SCL_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << UC_INT_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << UART_RX_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << UART_TX_32_BIT_POS);

    cfg_reg_masked |= MODER_SWCLK |
                      (MODER_SWDIO) |
                      (MODER_I2C_SDA) |
                      (MODER_I2C_SCL) |
                      (MODER_UC_INT) |
                      (MODER_UART_RX) |
                      (MODER_UART_TX);

    return cfg_reg_masked;
}

static uint32_t PUPDR_default_pins_mask(uint32_t cfg_reg)
{
    uint32_t cfg_reg_masked = cfg_reg;

    cfg_reg_masked &= ~(TWO_DEFALT_CONFIG_MASK << SWCLK_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << SWDIO_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << I2C_SDA_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << I2C_SCL_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << UC_INT_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << UART_RX_32_BIT_POS) &
                      ~(TWO_DEFALT_CONFIG_MASK << UART_TX_32_BIT_POS);

    cfg_reg_masked |= PUPDR_SWCLK |
                      (PUPDR_SWDIO) |
                      (PUPDR_I2C_SDA) |
                      (PUPDR_I2C_SCL) |
                      (PUPDR_UC_INT) |
                      (PUPDR_UART_RX) |
                      (PUPDR_UART_TX);

    return cfg_reg_masked;
}

static uint32_t OTYPER_default_pins_mask(uint32_t cfg_reg)
{
    uint32_t cfg_reg_masked = 0;
    cfg_reg_masked |= cfg_reg;

    cfg_reg_masked &= ~(ONE_DEFALT_CONFIG_MASK << SWCLK_32_BIT_POS) &
                      ~(ONE_DEFALT_CONFIG_MASK << SWDIO_32_BIT_POS) &
                      ~(ONE_DEFALT_CONFIG_MASK << I2C_SDA_32_BIT_POS) &
                      ~(ONE_DEFALT_CONFIG_MASK << I2C_SCL_32_BIT_POS) &
                      ~(ONE_DEFALT_CONFIG_MASK << UC_INT_32_BIT_POS) &
                      ~(ONE_DEFALT_CONFIG_MASK << UART_RX_32_BIT_POS) &
                      ~(ONE_DEFALT_CONFIG_MASK << UART_TX_32_BIT_POS);

    cfg_reg_masked |= OTYPER_SWCLK |
                      (OTYPER_SWDIO) |
                      (OTYPER_I2C_SDA) |
                      (OTYPER_I2C_SCL) |
                      (OTYPER_UC_INT) |
                      (OTYPER_UART_RX) |
                      (OTYPER_UART_TX);

    return cfg_reg_masked;
}

static uint32_t config_floating_state(uint32_t stm_reg, uint64_t virtual_reg)
{
    uint32_t pupr_reg = stm_reg;

    pupr_reg &= ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_0) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_0 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_1) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_1 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_2) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_2 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_3) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_3 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_4) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_4 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_5) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_5 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_6) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_6 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_7) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_7 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_8) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_8 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_9) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_9 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_10) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_10 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_11) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_11 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_12) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_12 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_13) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_13 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_14) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_14 * 2)) : 0xFFFFFFFF) &
                ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_15) == EXP_FLOATING_INPUT) ? ~(TWO_DEFALT_CONFIG_MASK << (EXP_IO_PIN_15 * 2)) : 0xFFFFFFFF);

    return pupr_reg;
}

// FUNCOES DE ESCRITA DE REGISTRADOR ==============================================================================
void exp_direction_config(uint16_t virtual_reg, exp_ports_t port)
{
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

    switch (port)
    {
    case EXP_PORT_A:
        direction_reg_cfg = MODER_default_pins_mask(direction_reg_cfg);
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
    print_STM_reg(direction_reg_cfg);
    print_virtual_reg((uint32_t)virtual_reg);
}

void exp_out_mode_config(uint16_t virtual_reg, exp_ports_t port)
{
    uint32_t out_mode_reg_cfg = 0;
    out_mode_reg_cfg = virtual_reg;

    switch (port)
    {
    case EXP_PORT_A:
        out_mode_reg_cfg = OTYPER_default_pins_mask(out_mode_reg_cfg);
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
    print_STM_reg(out_mode_reg_cfg);
    print_virtual_reg((uint32_t)virtual_reg);
}

void exp_in_mode_config(uint16_t virtual_reg, exp_ports_t port)
{
    uint32_t pupdr_reg;

    switch (port)
    {
    case EXP_PORT_A:
        pupdr_reg = GPIOA->PUPDR;
        current_scope_registers[EXP_IO_INPUT_MODE_A_REG].reg_content = virtual_reg;
        current_scope_registers[EXP_IO_INPUT_MODE_A_REG].reg_content |= (1 << SWCLK_16_BIT_POS) | (1 << SWDIO_16_BIT_POS); // seta pinos de SWDIO e SWCLK
        current_scope_registers[EXP_IO_INPUT_MODE_A_REG].reg_content &= ~(1 << I2C_SDA_16_BIT_POS) &                       // reseta pinos de I2C, UART e interrupção
                                                                        ~(1 << I2C_SCL_16_BIT_POS) &
                                                                        ~(1 << UC_INT_16_BIT_POS) &
                                                                        ~(1 << UART_RX_16_BIT_POS) &
                                                                        ~(1 << UART_TX_16_BIT_POS);
        pupdr_reg = config_floating_state(pupdr_reg, current_scope_registers[EXP_IO_INPUT_MODE_A_REG].reg_content);     // reseta os pinos que deve ser flutuantes
        GPIOA->PUPDR = pupdr_reg;   // aplica a modifica no registrador do STM32
        break;

    case EXP_PORT_B:
        pupdr_reg = GPIOB->PUPDR;
        current_scope_registers[EXP_IO_INPUT_MODE_B_REG].reg_content = virtual_reg;
        pupdr_reg = config_floating_state(pupdr_reg, current_scope_registers[EXP_IO_INPUT_MODE_B_REG].reg_content);
        GPIOB->PUPDR = pupdr_reg;
        break;

    case EXP_PORT_C:
        pupdr_reg = GPIOC->PUPDR;
        current_scope_registers[EXP_IO_INPUT_MODE_C_REG].reg_content = virtual_reg;
        pupdr_reg = config_floating_state(pupdr_reg, current_scope_registers[EXP_IO_INPUT_MODE_C_REG].reg_content);
        GPIOC->PUPDR = pupdr_reg;

        break;
    default:
        break;
    };
    print_STM_reg(pupdr_reg);
    print_virtual_reg((uint32_t)virtual_reg);
}

void exp_in_reference_config(uint16_t virtual_reg, exp_ports_t port)
{
    uint16_t short_pupdr;

    switch (port)
    {
    case EXP_PORT_A:
        short_pupdr = current_scope_registers[EXP_IO_INPUT_MODE_A_REG].reg_content;
        break;
    case EXP_PORT_B:
        short_pupdr = current_scope_registers[EXP_IO_INPUT_MODE_B_REG].reg_content;
        break;
    case EXP_PORT_C:
        short_pupdr = current_scope_registers[EXP_IO_INPUT_MODE_C_REG].reg_content;
        break;
    default:
        break;
    };

    uint32_t in_ref_reg_cfg = 0;
    extended_reg_content_t pull_ref_cfg_buffer =
        {
            .extended_cfg_content_0 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_0)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_0)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_1 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_1)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_1)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_2 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_2)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_2)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_3 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_3)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_3)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_4 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_4)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_4)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_4 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_4)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_4)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_5 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_5)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_5)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_6 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_6)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_6)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_7 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_7)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_7)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_8 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_8)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_8)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_9 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_9)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_9)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_10 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_10)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_10)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_11 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_11)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_11)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_12 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_12)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_12)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_13 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_13)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_13)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_14 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_14)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_14)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
            .extended_cfg_content_15 = (SLAVE_READ_BIT(short_pupdr, EXP_IO_PIN_15)) ? ((SLAVE_READ_BIT(virtual_reg, EXP_IO_PIN_15)) ? EXP_PULL_UP : EXP_PULL_DOWN) : 0,
        };

    in_ref_reg_cfg = (uint32_t)((pull_ref_cfg_buffer.extended_cfg_content_15 << EXTENDED_REG_POS_15) |
                                (pull_ref_cfg_buffer.extended_cfg_content_14 << EXTENDED_REG_POS_14) |
                                (pull_ref_cfg_buffer.extended_cfg_content_13 << EXTENDED_REG_POS_13) |
                                (pull_ref_cfg_buffer.extended_cfg_content_12 << EXTENDED_REG_POS_12) |
                                (pull_ref_cfg_buffer.extended_cfg_content_11 << EXTENDED_REG_POS_11) |
                                (pull_ref_cfg_buffer.extended_cfg_content_10 << EXTENDED_REG_POS_10) |
                                (pull_ref_cfg_buffer.extended_cfg_content_9 << EXTENDED_REG_POS_9) |
                                (pull_ref_cfg_buffer.extended_cfg_content_8 << EXTENDED_REG_POS_8) |
                                (pull_ref_cfg_buffer.extended_cfg_content_7 << EXTENDED_REG_POS_7) |
                                (pull_ref_cfg_buffer.extended_cfg_content_6 << EXTENDED_REG_POS_6) |
                                (pull_ref_cfg_buffer.extended_cfg_content_5 << EXTENDED_REG_POS_5) |
                                (pull_ref_cfg_buffer.extended_cfg_content_4 << EXTENDED_REG_POS_4) |
                                (pull_ref_cfg_buffer.extended_cfg_content_3 << EXTENDED_REG_POS_3) |
                                (pull_ref_cfg_buffer.extended_cfg_content_2 << EXTENDED_REG_POS_2) |
                                (pull_ref_cfg_buffer.extended_cfg_content_1 << EXTENDED_REG_POS_1) |
                                (pull_ref_cfg_buffer.extended_cfg_content_0 << EXTENDED_REG_POS_0));

    switch (port)
    {
    case EXP_PORT_A:
        in_ref_reg_cfg = PUPDR_default_pins_mask(in_ref_reg_cfg);
        GPIOA->PUPDR = in_ref_reg_cfg;
        break;
    case EXP_PORT_B:
        GPIOB->PUPDR = in_ref_reg_cfg;
        break;
    case EXP_PORT_C:
        GPIOC->PUPDR = in_ref_reg_cfg;
        break;
    default:
        break;
    };
    print_STM_reg(in_ref_reg_cfg);
    print_virtual_reg((uint32_t)virtual_reg);
}

void exp_inverted_pol_config(uint16_t virtual_reg, exp_ports_t port)
{
    switch (port)
    {
    case EXP_PORT_A:
        current_scope_registers[EXP_IO_INPUT_INVERT_POL_A_REG].reg_content = virtual_reg;
        break;
    case EXP_PORT_B:
        current_scope_registers[EXP_IO_INPUT_INVERT_POL_B_REG].reg_content = virtual_reg;
        break;
    case EXP_PORT_C:
        current_scope_registers[EXP_IO_INPUT_INVERT_POL_C_REG].reg_content = virtual_reg;
        break;
    default:
        break;
    };
    print_virtual_reg((uint32_t)virtual_reg);
}

void exp_gpio_state_config(uint16_t virtual_reg, exp_ports_t port)
{
    uint32_t state_config = 0;

    for (int curr_BSRR_bit = 15; curr_BSRR_bit >= 0; curr_BSRR_bit--)
    {
        ((virtual_reg >> curr_BSRR_bit) & 1) ? SLAVE_SET_BIT(state_config, curr_BSRR_bit) : SLAVE_SET_BIT(state_config, curr_BSRR_bit + 16);
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
    print_STM_reg(state_config);
    print_virtual_reg((uint32_t)virtual_reg);
}

// FUNCOES DE LEITURA =============================================================================================
uint16_t read_stm_reg(exp_registers_addr_t reg_name, exp_ports_t port)
{
    uint16_t r_reg_16 = 0;
    uint32_t r_reg_32 = 0;

    if ((reg_name >= EXP_IO_DIR_A_REG) && (reg_name <= EXP_IO_DIR_C_REG)) // REGISTRADORES DE DIREÇÃO - reg de 32 bits
    {
        switch (port)
        {
        case EXP_PORT_A:
            r_reg_32 = GPIOA->MODER;
            r_reg_16 = long_to_short_reg(r_reg_32);
            break;
        case EXP_PORT_B:
            r_reg_32 = GPIOB->MODER;
            r_reg_16 = long_to_short_reg(r_reg_32);
            break;
        case EXP_PORT_C:
            r_reg_32 = GPIOC->MODER;
            r_reg_16 = long_to_short_reg(r_reg_32);
            break;
        default:
            break;
        };
        print_STM_reg(r_reg_32);
    }
    else if ((reg_name >= EXP_IO_OUTPUT_MODE_A_REG) && (reg_name <= EXP_IO_OUTPUT_MODE_C_REG)) // REGISTRADORES DE MODO DE SAIDA - reg de 16 bits
    {
        switch (port)
        {
        case EXP_PORT_A:
            r_reg_16 = GPIOA->OTYPER;
            break;
        case EXP_PORT_B:
            r_reg_16 = GPIOB->OTYPER;
            break;
        case EXP_PORT_C:
            r_reg_16 = GPIOC->OTYPER;
            break;
        default:
            break;
        };
        print_virtual_reg((uint32_t)r_reg_16);
    }
    else if ((reg_name >= EXP_IO_INPUT_MODE_A_REG) && (reg_name <= EXP_IO_INPUT_MODE_C_REG)) // REGISTRADORES DE MODO DE ENTRADA reg de 16 bits
    {
        switch (port)
        {
        case EXP_PORT_A:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_MODE_A_REG].reg_content;
            break;
        case EXP_PORT_B:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_MODE_B_REG].reg_content;
            break;
        case EXP_PORT_C:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_MODE_C_REG].reg_content;
            break;
        default:
            break;
        };
        print_virtual_reg((uint32_t)r_reg_16);
    }
    else if ((reg_name >= EXP_IO_INPUT_REF_A_REG) && (reg_name <= EXP_IO_INPUT_REF_C_REG)) // REGISTRADORES DE PULL UP E PULL DOWN reg de 32 bits
    {
        switch (port)
        {
        case EXP_PORT_A:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_REF_A_REG].reg_content;
            print_STM_reg(GPIOA->PUPDR);
            break;
        case EXP_PORT_B:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_REF_B_REG].reg_content;
            print_STM_reg(GPIOB->PUPDR);
            break;
        case EXP_PORT_C:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_REF_C_REG].reg_content;
            print_STM_reg(GPIOC->PUPDR);
            break;
        default:
            break;
        };
        print_virtual_reg((uint32_t)r_reg_16);
    }
    else if ((reg_name >= EXP_IO_INPUT_INVERT_POL_A_REG) && (reg_name <= EXP_IO_INPUT_INVERT_POL_C_REG)) // REGISTRADORES DE INVERSÃO DE POLARIDADE reg de 16 bits
    {
        switch (port)
        {
        case EXP_PORT_A:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_INVERT_POL_A_REG].reg_content;
            break;
        case EXP_PORT_B:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_INVERT_POL_B_REG].reg_content;
            break;
        case EXP_PORT_C:
            r_reg_16 = current_scope_registers[EXP_IO_INPUT_INVERT_POL_C_REG].reg_content;
            break;
        default:
            break;
        };
        print_virtual_reg((uint32_t)r_reg_16);
    }
    else if ((reg_name >= EXP_IO_GPIO_A_REG) && (reg_name <= EXP_IO_GPIO_C_REG)) // REGISTRADORES DE GPIO reg de 32 bits
    {
        switch (port)
        {
        case EXP_PORT_A:
            r_reg_16 = inverted_pol_mask((uint16_t)GPIOA->IDR, current_scope_registers[EXP_IO_INPUT_INVERT_POL_A_REG].reg_content);
            break;
        case EXP_PORT_B:
            r_reg_16 = inverted_pol_mask((uint16_t)GPIOB->IDR, current_scope_registers[EXP_IO_INPUT_INVERT_POL_B_REG].reg_content);
            break;
        case EXP_PORT_C:
            r_reg_16 = inverted_pol_mask((uint16_t)GPIOC->IDR, current_scope_registers[EXP_IO_INPUT_INVERT_POL_C_REG].reg_content);
            break;
        default:
            break;
        };
        print_virtual_reg((uint32_t)r_reg_16);
    }
    return r_reg_16;
}

// INICIALIZACAO ==================================================================================================
genereric_reg_t *gpio_setup_cfg(void)
{
    return &current_scope_registers[0];
}

void exp_init_gpio_clks(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
}
