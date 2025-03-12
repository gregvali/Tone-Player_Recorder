
/**
 * Made by: Greg Valijan
 * Last Edit: 02/27/2025
 * uart.c
 */

#include "tm4c123gh6pm.h"

#define PIN12_HI            0x3

void EnableUART();
void sendChar(uint8_t);
char readChar();
void print(char*);
void uint32_to_str(uint32_t, char*);

void EnableUART()
{
    SYSCTL_RCGCUART_R   |= 0x1;         //Enables UART module
    SYSCTL_RCGCGPIO_R   |= 0x1;         //Enables  GPIO Clock for Port A
    GPIO_PORTA_AFSEL_R  |= PIN12_HI;    //Enables alt Function selection for PA0 and PA1 by GPIOPCTRL
    GPIO_PORTA_PCTL_R   |= PIN12_HI;    //Sets PA0 and PA1 to their alt functions (UART RX0 and TX0 respectively)
    GPIO_PORTA_DEN_R    |= PIN12_HI;    //Digital Enable for PA0 and PA1

    UART0_CTL_R          = 0x0;         //Temporarily Disable UART

    UART0_IBRD_R         = 104;         //Set the integer part of the baud rate
    UART0_FBRD_R         = 11;          //Set the fractional part of the baud rate
    UART0_LCRH_R         = (0x3<<5);    //Set UART to have 8 data bits (bits 5 and 6 are active), 1 stop bit and 0 parity bits
    UART0_CC_R           = 0x0;         //Set UART clock to System Clock (DEFAULT)

    UART0_CTL_R          = 0x301;       //Enable UART, Transmit and Receive
}

void sendChar(uint8_t data)
{
    while((UART0_FR_R & 0x20) != 0);        //Runs when receive holding register is not full
    UART0_DR_R = data;                      // data into DR
}

char readChar()
{
    while((UART0_FR_R & 0x10) != 0);        //Runs when transmit holding register is not full
    return UART0_DR_R;
}

void print(char* string)
{
    while(*string)
        sendChar(*(string++));          //while pointer to next char is not NULL, visit next char
}

void uint32_to_str(uint32_t num, char *str)
{
    int i = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';  // Null-terminate the string
        return;
    }

    // Convert the number to string (in reverse order)
    while (num > 0) {
        str[i++] = (num % 10) + '0';  // Get the current digit and convert it to a char
        num /= 10;  // Remove the last digit
    }

    str[i] = '\0';  // Null-terminate the string

    // Reverse the string (since we've been adding digits in reverse order)
    int start = 0;
    int end = i - 1;
    while (start < end) {
        // Swap characters at the start and end
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

