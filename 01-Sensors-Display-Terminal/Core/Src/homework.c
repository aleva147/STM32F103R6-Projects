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


void writeToLCD(uint32_t addr, const char* str) {
	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | addr);
	for (size_t i = 0; i < strlen(str); i++) {
		LCD_CommandEnqueue(LCD_DATA, str[i]);
	}
}

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
	writeToLCD(0x00, "WindDir: ");
	writeToLCD(0x40, "WindSpd: ");
	writeToLCD(0x10, "Temp(C): ");
	while(1) {
		if (temperatureSensor != prevTemperature
				|| windvaneSensor != prevWindvane
				|| anemometerSensor != prevAnemometer) {
			prevTemperature = temperatureSensor;
			prevWindvane = windvaneSensor;
			prevAnemometer = anemometerSensor;

			char temperature[4];
			char windvane[4];
			char anemometer[4];
			sprintf(temperature, "%d", temperatureSensor);
			sprintf(windvane, "%d", windvaneSensor);
			sprintf(anemometer, "%d", anemometerSensor);
			writeToLCD(0x18, "       ");	// Erase old value (necessary when decreasing the temperature from double digits to a single digit)
			writeToLCD(0x18, temperature);
			writeToLCD(0x08, "       ");
			writeToLCD(0x08, windvane);
			writeToLCD(0x48, "       ");
			writeToLCD(0x48, anemometer);
		}

		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

void homeworkInit()
{
	LCD_Init();
	Thermometer_Init();
	Windvane_Init();
	Anemometer_Init();
	xTaskCreate(homeworkTask, "Homework_Task", 128, NULL, 2, NULL);
	LED_Handle = xTimerCreate("LED_Timer", pdMS_TO_TICKS(500), pdTRUE, NULL, LED_Callback);
	xTimerStart(LED_Handle, portMAX_DELAY);
}

