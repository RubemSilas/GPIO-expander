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