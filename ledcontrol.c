/*
 * i2c.c
 *
 *  Created on: Mar 11, 2025
 *      Author: Greg Valijan
 */

#include "tm4c123gh6pm.h"

#define PORTF_EN                (7 << 1);                   //Enable PF2 & PF3
#define PF1_EN                  (1 << 1);                   //Enable PF1
#define PF2_EN                  (1 << 2);                   //Enable PF2
#define PF3_EN                  (1 << 3);                   //Enable PF3

void led_config();
void set_led(uint8_t);

void sendChar(uint8_t);

void led_config()
{
    SYSCTL_RCGCGPIO_R       |=  SYSCTL_RCGCGPIO_R5;         //Port F Clock
    GPIO_PORTF_DEN_R        |=  PORTF_EN;                   //PB2 and PB3 digital enable
    GPIO_PORTF_DIR_R        |=  PORTF_EN;                   //PB2 and PB3 output direction
}

void set_led(uint8_t mode)
{
    GPIO_PORTF_DATA_R = 0x0;

    if(mode == 'P'){
        GPIO_PORTF_DATA_R = PF3_EN;
    }
    else if(mode == 'R'){
        GPIO_PORTF_DATA_R = PF1_EN;
    }
    else if(mode == 'K'){
        GPIO_PORTF_DATA_R = PF2_EN;
    }
}
