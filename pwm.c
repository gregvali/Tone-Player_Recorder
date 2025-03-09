
/**
 * Made by: Greg Valijan
 * Last Edit: 03/6/2025
 * pwn.c
 */

#include "tm4c123gh6pm.h"

#define CLK                     250000
#define DIVISOR             0x000E0000
#define GENA_SET            0x0000008C                  //Generator Settings for comparator and load
//#define LOADA               0x00000237                  //Note A Load
//#define LOADB               0x000001FA                  //Note B Load
//#define LOADC               0x000003BA                  //Note C Load
//#define LOADD               0x00000353                  //Note D Load
//#define LOADE               0x000002F6                  //Note E Load
//#define LOADF               0x000002CB                  //Note F Load
//#define LOADG               0x0000027D                  //Note G Load
#define A                   440
#define B                   494
#define C                   262
#define D                   294
#define E                   330
#define F                   349
#define G                   392
#define EN                  0x00000001                  //Enable
#define DN                  0x00000000                  //Disable

void pwm_config();
void set_frequency(uint32_t value);
void delay_us(int);

void pwm_config()                                       //Configure PWM module
{
    SYSCTL_RCGCPWM_R        |=  SYSCTL_RCGCPWM_R0;      //PWM Clock
    SYSCTL_RCGCGPIO_R       |=  (1 << 1);               //Port B Clock
    SYSCTL_RCC_R            |=  SYSCTL_RCGC0_PWM0;      //Set clock to divisor
    SYSCTL_RCC_R            |=  DIVISOR;                //Set divisor to /64         (Clock set to 250kHz)
    GPIO_PORTB_AFSEL_R      |=  (1 << 6);               //PB6 AFSEL enable
    GPIO_PORTB_PCTL_R       |=  GPIO_PCTL_PB6_M0PWM0;   //PCTL function #4 of PB6
    GPIO_PORTB_DEN_R        |=  (1 << 6);               //PB6 Digital Enable
    PWM0_0_CTL_R             =  DN;                     //Disable PWM0 for Configuration
    PWM0_0_GENA_R            =  GENA_SET;               //Settings for comparator handling
    PWM0_0_LOAD_R            =  0x0;                    //Load value of 955 for C note
    PWM0_0_CMPA_R            =  0x0;                    //Load initial comparator value
    PWM0_0_CTL_R             =  EN;                     //Enable Generator
    PWM0_ENABLE_R            =  EN;                     //Enable Output
}

void set_frequency(uint32_t frequency)                  //Set PWM frequency.
{
    uint32_t load             =  CLK/frequency;
    PWM0_0_CTL_R              =  DN;                    //Disable PWM0 for Configuration
    PWM0_0_LOAD_R             =  load;                  //Set new load value
    PWM0_0_CMPA_R             =  load/2;                //50% duty cycle
    PWM0_0_CTL_R              =  EN;                    //Enable PWM0
}

void play_note(char note){
    switch(note)
    {
        case 'A':
            set_frequency(A);
            break;
        case 'B':
            set_frequency(B);
            break;
        case 'C':
            set_frequency(C);
            break;
        case 'D':
            set_frequency(D);
            break;
        case 'E':
            set_frequency(E);
            break;
        case 'F':
            set_frequency(F);
            break;
        case 'G':
            set_frequency(G);
            break;
        default:
            set_frequency(0);
    }

    delay_us(100000);
    set_frequency(0);
    delay_us(30000);
}

void delay_us(int n)
{                                               //US delay
    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < 16; j++){
            continue;
        }
    }
}
