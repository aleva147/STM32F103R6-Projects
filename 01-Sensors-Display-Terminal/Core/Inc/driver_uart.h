#ifndef CORE_INC_DRIVER_UART_H_
#define CORE_INC_DRIVER_UART_H_

#include <stdint.h>

extern void UART_Init();

extern void UART_TransmitCharacter(char character);

extern void UART_TransmitString(char* str);

extern void UART_TransmitInt(int num);

#endif /* CORE_INC_DRIVER_UART_H_ */
