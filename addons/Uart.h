/*
 * Uart.h
 *
 *  Created on: Jul 31, 2022
 *      Author: janispang
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include <stdint.h>
#include "stm32g0xx_hal.h"

#define BLOCK 0

extern UART_HandleTypeDef huart2;

/* Size of Reception buffer */
#define RX_BUFFER_SIZE                      128
#define TX_BUFFER_SIZE                      1024
typedef void (*getline_cb)(char* buffer, uint8_t length);

/* IRQ Handler treatment functions */
void UART_CharReception_Callback(void);
void UART_TXFT_Callback(void);
void UART_Error_Callback(void);
void UART_init(void);
void MX_USART2_UART_Init(void);
void UART_tx_poll(void);
void UART_tx(uint8_t *buffer, size_t buffer_size);
void UART_register_getline_callback(getline_cb cb);

#endif /* SRC_UART_H_ */
