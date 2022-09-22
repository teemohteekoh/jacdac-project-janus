#include "jdprofile.h"

#include "addons/example.h"
// #include "jdstm.h"
// #include "stm32g0.h"
// #include "stm32g0xx.h"
// #include "stm32g0xx_hal.h"
// #include "stm32g0xx_hal_uart.h"

UART_HandleTypeDef huart2;

// /* Single byte to store input */
// uint8_t byte;

// void SystemClock_Config(void);

// /* UART2 Interrupt Service Routine */
// void USART2_IRQHandler(void)
// {
//   HAL_UART_IRQHandler(&huart2);
// }

// /* This callback is called by the HAL_UART_IRQHandler when the given number of bytes are received */
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//   if (huart->Instance == USART2)
//   {
//     /* Transmit one byte with 100 ms timeout */
//     HAL_UART_Transmit(&huart2, &byte, 1, 100);

//     /* Receive one byte in interrupt mode */ 
//     HAL_UART_Receive_IT(&huart2, &byte, 1);
//   }
// }

// void my_uart_init(void)
// {
//   __USART2_CLK_ENABLE();

//   huart2.Instance = USART2;
//   huart2.Init.BaudRate = 115200;
//   huart2.Init.WordLength = UART_WORDLENGTH_8B;
//   huart2.Init.StopBits = UART_STOPBITS_1;
//   huart2.Init.Parity = UART_PARITY_NONE;
//   huart2.Init.Mode = UART_MODE_TX_RX;
//   huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//   huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//   HAL_UART_Init(&huart2);

//   /* Peripheral interrupt init*/
//   HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
//   HAL_NVIC_EnableIRQ(USART2_IRQn);
// }

#include "addons/Uart.h"

// Edit the string below to match your company name, the device name, and hardware revision.
// Do not change the generated 0x3.... value, as that would break the firmware update process.
FIRMWARE_IDENTIFIER(0x30249c44, "Project Janus XBox Gateway");

void app_init_services() {
    // see jacdac-c/services/jd_services.h for the services that can be
    // enabled here; you can enable zero or more services
    UART_init();
    button_init(PC_13, 0, NO_PIN);
	hidkeyboard_init();

    example_custom_function(); // call our custom function
}
