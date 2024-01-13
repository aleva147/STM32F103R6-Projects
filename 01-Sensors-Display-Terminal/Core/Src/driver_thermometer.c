#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "gpio.h"
#include "adc.h"


extern SemaphoreHandle_t mx_MutexHandle;
TaskHandle_t thermometer_TaskHandle;

int temperatureSensor = 0;
extern int mxSelect;


void thermometerTask(void* parameters) {
	while (1) {
		xSemaphoreTake(mx_MutexHandle, portMAX_DELAY);	// Prevent conflict when windvaneTask wants to use ADC.
		mxSelect = 0;									// Tells ADC whether it's reading temperature or windvane value.
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);	// MX's channel 0 is being selected.
		HAL_ADC_Start_IT(&hadc1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);		// Wait for ADC to finish.
		xSemaphoreGive(mx_MutexHandle);

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void Thermometer_Init() {
	xTaskCreate(thermometerTask, "Thermometer_Task", 128, NULL, 5, &thermometer_TaskHandle);
}
