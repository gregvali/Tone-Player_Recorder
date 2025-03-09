
/**
 * Made by: Greg Valijan
 * Last Edit: 03/06/2025
 * keypad.c
 */

#include "tm4c123gh6pm.h"

void keypad_config();
char keypad_input();

void keypad_config()
{
    //Port C
    SYSCTL_RCGCGPIO_R |= 0x4;               //Port C Clock
    GPIO_PORTC_DEN_R |= 0xF0;               //PC4 - PC7 Digital Enable
    GPIO_PORTC_DIR_R |= 0x0;                //Port C input
    GPIO_PORTC_PDR_R |= 0xF0;               //PC4 - PC7 Weak PD resistor

    //Port E
    SYSCTL_RCGCGPIO_R |= 0x10;              //Port E Clock
    GPIO_PORTE_DEN_R |= 0x1E;               //PE1 - PE4 Digital Enable For RS, RW, E
    GPIO_PORTE_DIR_R |= 0x1E;               //Port E output
}

char keypad_input()
{
    unsigned char symbol[4][4] = {
        {'A','B','C','P'},
        {'D','E','F','R'},
        { 0 ,'G', 0 ,'B'},
        { 0 , 0 , 0 , 0 }
    };

    int i = 0;
    int j = 0;

    //E1-4 is row 1-4       output to pad
    //C4-7 is column 1-4    input from pad
    for(i = 0; i < 4; i++)
    {
        GPIO_PORTE_DATA_R = (1 << i+1);
        for (j = 0; j < 4; j++)
        {
            if(GPIO_PORTC_DATA_R & (1 << j+4))
            {
                return symbol[i][j];
            }
        }
        GPIO_PORTE_DATA_R = 0x0;
    }
    return 0x0;
}
