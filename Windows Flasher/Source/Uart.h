#ifndef UART_H_INCLUDED
#define UART_H_INCLUDED

extern Std_ReturnType Uart_Init(char* com);

extern uint8_t Uart_Send(char* data, DWORD size);

extern void Uart_Receive(char* data, DWORD size);

extern void Uart_End(void);

#endif // UART_H_INCLUDED
