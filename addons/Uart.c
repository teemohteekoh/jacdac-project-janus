/*
 * Uart.c
 *
 *  Created on: Jul 31, 2022
 *      Author: janispang
 */

#include "Uart.h"

#include "stm32g0xx_ll_usart.h"
#include <stdbool.h>
#include <stddef.h>
#include "circbuf.h"

#define Error_Handler(void) 	\
	  __disable_irq();			\
	  while(1);

/* Buffer used for reception */
static uint8_t RXBuffer[RX_BUFFER_SIZE];
static uint8_t TXBuffer[TX_BUFFER_SIZE];
static volatile uint32_t NumCharsReceived = 0;
static volatile uint32_t NumCharsToTransmit = 0;
static getline_cb getline_callback = NULL;
static circbuf_struct TxCircbuff;

void UART_CharReception_Callback(void)
{
	uint8_t ch;

  /* Read Received character. RXNE flag is cleared by reading of RDR register */
  ch= LL_USART_ReceiveData8(USART2);

  RXBuffer[NumCharsReceived++] =ch;


  if (ch == '\n' || ch =='\r'){
	  if (getline_callback!= NULL){
		  getline_callback((char *)RXBuffer,NumCharsReceived);
	   }
	  NumCharsReceived = 0;
  }

}

void UART_TXFT_Callback(void)
{
	__disable_irq();
	uint8_t ch = 0;
	while(LL_USART_IsActiveFlag_TXE(USART2)){
		if(circbuf_is_empty(&TxCircbuff)==1){
			LL_USART_DisableIT_TXE(USART2);
			break;
		}
		else if(circbuf_is_empty(&TxCircbuff)==0)
		{
			circbuf_buffer_remove_item(&TxCircbuff, &ch );
			LL_USART_TransmitData8(USART2, ch);
		}
	}


  __enable_irq();
}

void UART_register_getline_callback(getline_cb cb)
{
	getline_callback = cb;
}

/**
  * @brief  UART error callbacks
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void UART_Error_Callback(void)
{
  __IO uint32_t isr_reg;

  /* Disable USARTx_IRQn */
  NVIC_DisableIRQ(USART1_IRQn);

  /* Error handling example :
    - Read USART ISR register to identify flag that leads to IT raising
    - Perform corresponding error handling treatment according to flag
  */
  isr_reg = LL_USART_ReadReg(USART2, ISR);
  if (isr_reg & LL_USART_ISR_NE)
  {
    /* Turn LED4 on: Transfer error in reception/transmission process */
	  // Do something else
  }
}

/* Initializes Buffer swap mechanism :
   2 physical buffers RXBuffer and TXBuffer (TX_BUFFER_SIZE length)
   Any data received will be stored in active buffer : the number max of
   data received is RX_BUFFER_SIZE */
void UART_init(void)
{
	circbuf_init(&TxCircbuff,TXBuffer, TX_BUFFER_SIZE);

	MX_USART2_UART_Init();
	  NumCharsReceived = 0;
	  NumCharsToTransmit = 0;
}

/* Checks if Buffer full indication has been set */

void UART_tx_poll(void)
{
	if (NumCharsToTransmit != 0)
		{
		  /* Reset indication */
		  NumCharsToTransmit = 0;

		  /* USART IRQ handler is not anymore routed to HAL_UART_IRQHandler() function
			 and is now based on LL API functions use.
			 Therefore, use of HAL IT based services is no more possible : use TX HAL polling services */

		  UART_tx((uint8_t*)TXBuffer, TX_BUFFER_SIZE);
		}
}

#if BLOCK
void UART_tx(uint8_t *buffer, size_t buffer_size)
{
	if(buffer ==NULL)
		return;

  if(HAL_UART_Transmit(&huart2, (uint8_t*)buffer, buffer_size, 1000)!= HAL_OK)
  {
	/* Transfer error in transmission process */
	Error_Handler();
  }
}
#else
void UART_tx(uint8_t *buffer, size_t buffer_size)
{
	if(buffer ==NULL)
		return;

	__disable_irq();
	for (size_t i =0; i < buffer_size; i ++){
		if (circbuf_is_full(&TxCircbuff) == 0)
		{
			circbuf_buffer_add_item(&TxCircbuff, buffer[i]);
		}
		else
		{
			break;
		}

	}

	__enable_irq();
	  LL_USART_EnableIT_TXE_TXFNF(USART2);
}
#endif
/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN USART2_Init 2 */
  /* Enable RXNE and Error interrupts */
  LL_USART_EnableIT_RXNE(USART2);
  LL_USART_EnableIT_ERROR(USART2);
  /* USER CODE END USART2_Init 2 */

}

