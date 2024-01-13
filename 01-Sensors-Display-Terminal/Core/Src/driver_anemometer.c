#include "driver_anemometer.h"
#include "tim.h"

int overflowCnt = 0;
int anemometerSensor = 0;
uint32_t prevTicks = 0;


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance != htim1.Instance) {
		return;
	}
	uint32_t ticks = htim->Instance->CCR1 + overflowCnt * 65536 - prevTicks;
	prevTicks = htim->Instance->CCR1;
	overflowCnt = 0;
	anemometerSensor = 8000000 / ticks * 2.4;
}

void Anemometer_Init() {
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
}
