#include "utils.h"

void print_register(uint32_t reg)
{
    char reg_value[100];
    char curr_bit[5];

    sprintf(reg_value, "reg - int: %d - bin: ", reg);
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)reg_value, strlen(reg_value), UART_TIMEOUT);

    for (int i = 31; i >= 0; i--)
    {
        sprintf(curr_bit, "%d ", ((reg >> i) & 1));
        HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)curr_bit, strlen(curr_bit), UART_TIMEOUT);

        if (i % 8 == 0)
        {
            sprintf(curr_bit, "|| ");
            HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)curr_bit, strlen(curr_bit), UART_TIMEOUT);
        }
    }

    sprintf(reg_value, " - hex: %X", reg);
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)reg_value, strlen(reg_value), UART_TIMEOUT);
}

void print_STM_reg(uint32_t stm_reg)
{
    char log_msg[20];
    sprintf(log_msg, "STM reg: ");
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
    print_register(stm_reg);
}

void print_virtual_reg(uint32_t virtual_reg)
{
    virtual_reg &= 0x0000FFFF;
    char log_msg[20];
    sprintf(log_msg, "virtual reg: ");
    HAL_UART_Transmit(&UART_HANDLER, (uint8_t *)log_msg, strlen(log_msg), UART_TIMEOUT);
    print_register(virtual_reg);
}

uint32_t switch_endianes(uint32_t target_value)
{
    uint32_t new_value = 0;
    uint8_t bytes_position_buffer[4] = {0};

    bytes_position_buffer[0] = target_value & (0x000000FF);
    bytes_position_buffer[1] = (target_value >> 8 & (0x000000FF));
    bytes_position_buffer[2] = (target_value >> 16 & (0x000000FF));
    bytes_position_buffer[3] = (target_value >> 24 & (0x000000FF));
    
    new_value = (bytes_position_buffer[0] << 24) | (bytes_position_buffer[1] << 16)| (bytes_position_buffer[2] << 8) | (bytes_position_buffer[3] << 0);
    
    return new_value;
}
