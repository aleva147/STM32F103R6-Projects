#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gpio.h"
#include "adc.h"


TaskHandle_t thermometer_TaskHandle;

int temperatureSensor = 0;


void thermometerTask(void* parameters) {
	while (1) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);	// MX's channel 0 is being selected.
		HAL_ADC_Start_IT(&hadc1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);		// Wait for ADC to finish.

		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void Thermometer_Init() {
	xTaskCreate(thermometerTask, "Thermometer_Task", 128, NULL, 5, &thermometer_TaskHandle);
}
