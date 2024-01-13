#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "adc.h"

// MAX VOLTAGE = 5V
#define MAX_VOLTAGE 5.0
// ADC RESOLUTION = 2^12 = 4096 (12-bit resolution)
#define RESOLUTION 4096.0
