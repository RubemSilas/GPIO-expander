#ifndef EXP_UTILS_H
#define EXP_UTILS_H

#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "stdlib.h"

#include "main.h"
#include "i2c.h"
#include "usart.h"

#define UART_HANDLER huart2
#define I2C_HANDLER hi2c1

#define UART_TIMEOUT (100) // milisegundos
#define I2C_TIMEOUT (100)  // milisegundos

// COMUNICATION RELATED
#define I2C_WRITE_OPERATION 0
#define I2C_READ_OPERATION 1

#endif