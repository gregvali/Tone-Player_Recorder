

/**
 * main.c
 */
#include "tm4c123gh6pm.h"

void keypad_config();
char keypad_input();
void pwm_config();
void set_frequency(uint32_t);
void play_note(char);
void delay_us(int);

int main(void)
{
    pwm_config();
    keypad_config();

    char input = '\0';

    while(1)
    {
        input = keypad_input();
        if(input != 0) play_note(input);
    }
}

