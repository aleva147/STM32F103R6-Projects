#include "homework.h"

#include "FreeRTOS.h"
#include "task.h"
#include "adc.h"
#include "gpio.h"
#include "timers.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "driver_lcd.h"
#include "driver_uart.h"
#include "driver_thermometer.h"
#include "driver_anemometer.h"
#include "driver_windvane.h"


int prevTemperature = 0;
int prevWindvane = 0;
int prevAnemometer = 0;

TimerHandle_t LED_Handle;



void LED_Callback() {
	if (anemometerSensor < 50) {
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_13);
	}
}


static void homeworkTask(void *parameters)
{
	LCD_WriteString(0x00, "WindDir: ");
	LCD_WriteString(0x40, "WindSpd: ");
	LCD_WriteString(0x10, "Temp(C): ");
	while(1) {
		if (temperatureSensor != prevTemperature
				|| windvaneSensor != prevWindvane
				|| anemometerSensor != prevAnemometer) {
			prevTemperature = temperatureSensor;
			prevWindvane = windvaneSensor;
			prevAnemometer = anemometerSensor;

			LCD_WriteString(0x19, "      ");	// Erase old value (necessary when decreasing the temperature from double digits to a single digit)
			LCD_WriteInt(0x19, temperatureSensor);
			LCD_WriteString(0x09, "      ");
			LCD_WriteInt(0x09, windvaneSensor);
			LCD_WriteString(0x49, "      ");
			LCD_WriteInt(0x49, anemometerSensor);

			UART_TransmitString("\b\b\b\b\b\b\b\b\b\b\b");
			UART_TransmitInt(windvaneSensor);
			UART_TransmitCharacter('/');
			UART_TransmitInt(anemometerSensor);
			UART_TransmitCharacter('/');
			UART_TransmitInt(temperatureSensor);
		}

		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

void homeworkInit()
{
	LCD_Init();
	UART_Init();
	Thermometer_Init();
	Windvane_Init();
	Anemometer_Init();
	xTaskCreate(homeworkTask, "Homework_Task", 128, NULL, 2, NULL);
	LED_Handle = xTimerCreate("LED_Timer", pdMS_TO_TICKS(500), pdTRUE, NULL, LED_Callback);
	xTimerStart(LED_Handle, portMAX_DELAY);
}

