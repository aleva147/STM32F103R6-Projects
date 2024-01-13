#include "driver_uart.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "usart.h"

// TRANSMIT
// -----------------------------------------------------------------------------

static TaskHandle_t UART_TransmitTaskHandle;
static QueueHandle_t UART_TransmitQueueHandle;

static void UART_TransmitTask(void *parameters)
{
	uint8_t byte;
	while (1) {
		xQueueReceive(UART_TransmitQueueHandle, &byte, portMAX_DELAY);
		HAL_UART_Transmit_IT(&huart1, &byte, sizeof(uint8_t));
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance != huart1.Instance) {
		return;
	}
	BaseType_t woken = pdFALSE;
	vTaskNotifyGiveFromISR(UART_TransmitTaskHandle, &woken);
	portYIELD_FROM_ISR(woken);
}

void UART_Init()
{
	xTaskCreate(UART_TransmitTask, "Trasmit_Task", 128, NULL, 5, &UART_TransmitTaskHandle);
	UART_TransmitQueueHandle = xQueueCreate(64, sizeof(uint8_t));
}

// TRANSMIT UTIL
// -----------------------------------------------------------------------------

void UART_AsyncTransmitCharacter(char character)
{
	uint8_t byte = character;
	xQueueSendToBack(UART_TransmitQueueHandle, &byte, portMAX_DELAY);
}
