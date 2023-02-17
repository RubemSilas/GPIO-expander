#include "exp_main.h"

void read_register(genereric_reg_t *gen_register)
{
    char log_msg[50];
    HAL_StatusTypeDef err;

    gen_register->reg_content = read_stm_reg(gen_register->reg_name, gen_register->port_name);

    uint32_t i2c_transmit_msg = 0;
    i2c_msg_t exp_transmit_msg =
        {
            .rw = I2C_READ_OPERATION,
            .exp_register = gen_register->reg_name,
            .exp_data = gen_register->reg_content,
        };

    i2c_transmit_msg |= (exp_transmit_msg.rw << EXP_RW_BIT_POS) | (exp_transmit_msg.exp_register << EXP_REG_POS) | (exp_transmit_msg.exp_data << EXP_DATA_POS);
    sprintf(log_msg, "leitura do registrador %d - valor lido: %X\r\n", gen_register->reg_name, gen_register->reg_content);
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);

    err = HAL_I2C_Slave_Transmit(&I2C_HANDLER, (uint8_t *)&i2c_transmit_msg, sizeof(uint32_t), I2C_TIMEOUT);
    if (err != HAL_OK)
    {
        sprintf(log_msg, "erro ao retornar valor do registrador: (%d)\r\n", gen_register->reg_name);
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

    gen_register->reg_content = incoming_data;
    gen_register->op_func(gen_register->reg_content, gen_register->port_name);

    uint32_t i2c_transmit_msg = 0;
    i2c_msg_t exp_transmit_msg =
        {
            .rw = I2C_READ_OPERATION,
            .exp_register = gen_register->reg_name,
            .exp_data = gen_register->reg_content,
        };

    i2c_transmit_msg |= (exp_transmit_msg.rw << EXP_RW_BIT_POS) | (exp_transmit_msg.exp_register << EXP_REG_POS) | (exp_transmit_msg.exp_data << EXP_DATA_POS);
    sprintf(log_msg, " - escrita do registrador %d - novo valor: %X\r\n", gen_register->reg_name, gen_register->reg_content);
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
}

void i2c_request_listener(void)
{
    char log_msg[100];

    char uart_init_msg[] = "========== SLAVE I2C ==========\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)uart_init_msg, strlen(uart_init_msg), 100);

    genereric_reg_t *expander_registers = gpio_setup_cfg();
    exp_init_gpio_clks();
    init_uc_it_pin();

    uint32_t rqst_buffer;
    i2c_msg_t incoming_i2c_msg;

    while (1)
    {
        if (HAL_I2C_Slave_Receive(&I2C_HANDLER, (uint8_t *)&rqst_buffer, sizeof(uint32_t), I2C_TIMEOUT) == HAL_OK)
        {
            /* preenchimento da estrutura de mensagem de envio */
            incoming_i2c_msg.rw = SLAVE_READ_BIT(rqst_buffer, EXP_RW_BIT_POS);
            incoming_i2c_msg.exp_register = (rqst_buffer >> 16) & BYTE_MASK;
            incoming_i2c_msg.exp_data = rqst_buffer & HALF_WORD_MASK;

            if (incoming_i2c_msg.rw == I2C_WRITE_OPERATION)
            {
                sprintf(log_msg, ">>> REQUISICAO DE ESCRITA\r\n");
                HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);

                if (incoming_i2c_msg.exp_register <= EXP_IO_PORT_C_INT_ENABLE)
                {
                    write_to_register(&expander_registers[incoming_i2c_msg.exp_register], incoming_i2c_msg.exp_data);
                }
            }
            else if (incoming_i2c_msg.rw == I2C_READ_OPERATION)
            {
                sprintf(log_msg, ">>> REQUISICAO DE LEITURA\r\n");
                HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);

                if (incoming_i2c_msg.exp_register < EXP_TOTAL_REGISTERS)
                {
                    read_register(&expander_registers[incoming_i2c_msg.exp_register]);
                }
            }
        }
        exp_interruption_detector();
    }
}
