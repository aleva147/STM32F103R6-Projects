#include "homework.h"

#include "FreeRTOS.h"
#include "task.h"

#include <stdlib.h>
#include <string.h>

#include "driver_lcd.h"
#include "driver_uart.h"
#include "driver_thermometer.h"
#include "driver_anemometer.h"
#include "driver_windvane.h"



void writeToLCD(uint32_t addr, const char* str) {
	LCD_CommandEnqueue(LCD_INSTRUCTION, LCD_SET_DD_RAM_ADDRESS_INSTRUCTION | addr);
	for (size_t i = 0; i < strlen(str); i++) {
		LCD_CommandEnqueue(LCD_DATA, str[i]);
	}
}

static void homeworkTask(void *parameters)
{
	writeToLCD(0x00, "WindDir: ");
	writeToLCD(0x40, "WindSpd: ");
	writeToLCD(0x10, "Temp(C): ");
	while(1) {
		vTaskDelay(pdMS_TO_TICKS(200));
	}
}

void homeworkInit()
{
	LCD_Init();
	xTaskCreate(homeworkTask, "Homework_Task", 128, NULL, 2, NULL);
}

