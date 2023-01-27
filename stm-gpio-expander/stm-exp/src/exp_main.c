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
