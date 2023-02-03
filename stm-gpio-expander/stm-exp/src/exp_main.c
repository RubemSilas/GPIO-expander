#include "exp_main.h"

void read_register(exp_registers_addr_t r_register, uint16_t r_reg_value)
{
    char log_msg[50];
    HAL_StatusTypeDef err;

    uint32_t i2c_transmit_msg = 0;
    i2c_msg_t exp_transmit_msg =
        {
            .rw = I2C_READ_OPERATION,
            .exp_register = r_register,
            .exp_data = r_reg_value,
        };

    i2c_transmit_msg |= (exp_transmit_msg.rw << EXP_RW_BIT_POS) | (exp_transmit_msg.exp_register << EXP_REG_POS) | (exp_transmit_msg.exp_data << EXP_DATA_POS);
    sprintf(log_msg, "leitura do registrador %d - valor lido: %X\r\n", r_register, r_reg_value);
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);

    err = HAL_I2C_Slave_Transmit(&I2C_HANDLER, (uint8_t *)&i2c_transmit_msg, sizeof(uint32_t), I2C_TIMEOUT);
    if (err != HAL_OK)
    {
        sprintf(log_msg, "erro ao retornar valor do registrador: (%d)\r\n", r_register);
        HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
    }
    else
    {
        sprintf(log_msg, "valor enviado\r\n");
        HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
    }
}

void write_to_register(genereric_reg_t *gen_register, uint16_t incoming_data)
{
    char log_msg[50];

    gen_register->reg_content = incoming_data; // alteracao do dado

    uint32_t i2c_transmit_msg = 0;
    i2c_msg_t exp_transmit_msg =
        {
            .rw = I2C_READ_OPERATION,
            .exp_register = gen_register->reg_name,
            .exp_data = gen_register->reg_content,
        };

    i2c_transmit_msg |= (exp_transmit_msg.rw << EXP_RW_BIT_POS) | (exp_transmit_msg.exp_register << EXP_REG_POS) | (exp_transmit_msg.exp_data << EXP_DATA_POS);
    sprintf(log_msg, "escrita do registrador %d - novo valor: %X\r\n", gen_register->reg_name, gen_register->reg_content);
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
}

void i2c_request_listener(void)
{
    genereric_reg_t expander_registers[EXP_TOTAL_REGISTERS] =
        {
        {.reg_name = EXP_IO_DIR_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_direction_config, .reg_content = 0},
        {.reg_name = EXP_IO_DIR_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_direction_config, .reg_content = 0},
        {.reg_name = EXP_IO_DIR_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_direction_config, .reg_content = 0},

        {.reg_name = EXP_IO_OUTPUT_MODE_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_out_mode_config, .reg_content = 0},
        {.reg_name = EXP_IO_OUTPUT_MODE_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_out_mode_config, .reg_content = 0},
        {.reg_name = EXP_IO_OUTPUT_MODE_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_out_mode_config, .reg_content = 0},

        {.reg_name = EXP_IO_INPUT_MODE_A_REG, .port_name = EXP_PORT_A, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_MODE_B_REG, .port_name = EXP_PORT_B, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_MODE_C_REG, .port_name = EXP_PORT_C, .reg_content = 0},

        {.reg_name = EXP_IO_INPUT_REF_A_REG, .port_name = EXP_PORT_A, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_REF_B_REG, .port_name = EXP_PORT_B, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_REF_C_REG, .port_name = EXP_PORT_C, .reg_content = 0},

        {.reg_name = EXP_IO_INPUT_INVERT_POL_A_REG, .port_name = EXP_PORT_A, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_INVERT_POL_B_REG, .port_name = EXP_PORT_B, .reg_content = 0},
        {.reg_name = EXP_IO_INPUT_INVERT_POL_C_REG, .port_name = EXP_PORT_C, .reg_content = 0},

        {.reg_name = EXP_IO_GPIO_A_REG, .port_name = EXP_PORT_A, .op_func = &exp_gpio_state_config, .reg_content = 0},
        {.reg_name = EXP_IO_GPIO_B_REG, .port_name = EXP_PORT_B, .op_func = &exp_gpio_state_config, .reg_content = 0},
        {.reg_name = EXP_IO_GPIO_C_REG, .port_name = EXP_PORT_C, .op_func = &exp_gpio_state_config, .reg_content = 0},
        };

    char log_msg[100];

    char uart_init_msg[] = "========== SLAVE I2C ==========\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_init_msg, strlen(uart_init_msg), 100);

    exp_init_gpio_clks();

    uint32_t rqst_buffer;
    i2c_msg_t incoming_i2c_msg;

    while (1)
    {
        if (HAL_I2C_Slave_Receive(&I2C_HANDLER, (uint8_t *)&rqst_buffer, 4, I2C_TIMEOUT) == HAL_OK)
        {
            /* preenchimento da estrutura */
            incoming_i2c_msg.rw = SLAVE_READ_BIT(rqst_buffer, EXP_RW_BIT_POS);
            incoming_i2c_msg.exp_register = (rqst_buffer >> 16) & BYTE_MASK;
            incoming_i2c_msg.exp_data = rqst_buffer & HALF_WORD_MASK;

            if (incoming_i2c_msg.rw == I2C_WRITE_OPERATION)
            {
                sprintf(log_msg, "msg de requisicao de escrita\r\n");
                HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);

                if (incoming_i2c_msg.exp_register < EXP_TOTAL_REGISTERS)
                {
                    write_to_register(&expander_registers, incoming_i2c_msg.exp_data);
                }
            }
            else if (incoming_i2c_msg.rw == I2C_READ_OPERATION)
            {
                sprintf(log_msg, "msg de requisicao de leitura\r\n");
                HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);

                if (incoming_i2c_msg.exp_register < EXP_TOTAL_REGISTERS)
                {
                    read_register(expander_registers[incoming_i2c_msg.exp_register].reg_name, expander_registers[incoming_i2c_msg.exp_register].reg_content);
                }
            }
        }
    }
}
