#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "gpio.h"
#include "adc.h"


extern SemaphoreHandle_t mx_MutexHandle;
TaskHandle_t windvane_TaskHandle;

int windvaneSensor = 0;


void windvaneTask(void* parameters) {
	while (1) {
		xSemaphoreTake(mx_MutexHandle, portMAX_DELAY);	// Prevent conflict when thermometerTask wants to use ADC.
		mxSelect = 1;									// Tells ADC whether it's reading temperature or windvane value.
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);		// MX's channel 1 is being selected.
		HAL_ADC_Start_IT(&hadc1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);		// Wait for ADC to finish.
		xSemaphoreGive(mx_MutexHandle);

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void Windvane_Init() {
	xTaskCreate(windvaneTask, "Windvane_Task", 128, NULL, 5, &windvane_TaskHandle);
}
