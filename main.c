

/**
 * made by: Greg Valijan
 * main.c
 */

#include "tm4c123gh6pm.h"

void EnableUART();
void print(char*);
void sendChar(uint8_t);
void delay_us(int);

void keypad_config();
uint8_t keypad_input();

void pwm_config();
void play_note_s(uint8_t);
void play_note_i(uint8_t);

void led_config();
void set_led(uint8_t);

void i2c_config();
void store_byte(uint8_t, uint16_t);
uint8_t random_read(uint16_t);
void reset();


enum State                                              //State Enumerations
{
    PLAY,
    RECORD,
    PLAYBACK
};


int main(void)
{
    EnableUART();
    keypad_config();
    pwm_config();
    led_config();
    i2c_config();

    uint8_t input            =  0x0;
    uint16_t addr            =  0x0;
    uint16_t total           =  0x0;

    enum State state = PLAY;                            //Default State
    set_led('P');

    while(1)
    {
        if(state == PLAY){

            if(input != 0x0) play_note_i(input);          //If there is an input, play a note

            if(input == 'R'){                           //Change to RECORD State
                reset();                                //Clear Memory
                state = RECORD;
                set_led('R');
            }
            else if(input == 'K') {                     //Change to PLAYBACK State
                state = PLAYBACK;
                set_led('K');
            }

            sendChar('p');
        }
        else if(state == RECORD){

            if(input != 0x0) {                          //If there is input
                play_note_s(input);                     //play the specified note
                store_byte(input, addr++);              //Save the note while incrementing the address
            }

            if(input == 'P'){                           //Change to PLAY State
                state = PLAY;
                set_led('P');
            }
            else if(input == 'K') {                     //Change to PLAYBACK State
                state = PLAYBACK;
                set_led('K');
            }

            sendChar('r');
        }
        else if(state == PLAYBACK){
            if(input == 'Y') {
                total = 0x0;
                while(total < addr){                    //Loop through every saved byte
                    play_note_s(random_read(total++));  //Play each note
                }
            }

            if(input == 'P'){                           //Change to PLAY State
                state = PLAY;
                set_led('P');
                addr = 0x0;
            }
            else if(input == 'R') {                     //Change to RECORD State
                reset();                                //Clear Memory
                state = RECORD;
                set_led('R');
                addr = 0x0;
            }

            sendChar('b');
        }

        input = keypad_input();
    }
}

