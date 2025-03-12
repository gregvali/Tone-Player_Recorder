/*
 * i2c.c
 *
 *  Created on: Mar 8, 2025
 *      Author: Greg Valijan
 */

#include "tm4c123gh6pm.h"

#define PORTB_EN                (3 << 2);                   //Enable PB2 & PB3
#define PB3_EN                  (1 << 3);                   //Enable PB3
#define PB2_EN                  (1 << 2);                   //Enable PB2

#define SYS_CLK                 (7 << 0);                   //Set EEPROM Clock

const uint16_t DELAY        =  0x2000;                      //delay in us

const uint8_t EEPROM_WRITE  =  0xA0;                        //Init data sent to EEPROM: Basic control nibble + chip sel + write
const uint8_t EEPROM_READ   =  0xA1;                        //Init data sent to EEPROM: Basic control nibble + chip sel + read
const uint8_t STARTEN       =  0x03;                        //Generate Start and enable I2C
const uint8_t STOPEN        =  0x05;                        //Generate Stop and enable I2C
const uint8_t I2CEN         =  0x01;                        //Enable I2C
const uint8_t STARTSTOP     =  0x07;                        //Generate Start and Stop with I2C enable
const uint8_t STOP          =  0x04;                        //Generate Stop


void i2c_config();
void store_byte(uint8_t, uint16_t);
uint8_t random_read(uint16_t);
void change_addr(uint16_t);
void write_dr(uint8_t, uint8_t);
uint8_t read_dr();

void print(char*);                                          //UART print
void sendChar(char);

void delay_us(int);

void i2c_config()
{
    SYSCTL_RCGCI2C_R        |=  SYSCTL_RCGCI2C_R0;          //I2C module 0 clock
    SYSCTL_RCGCGPIO_R       |=  SYSCTL_RCGCGPIO_R1;         //Port B Clock

    GPIO_PORTB_DEN_R        |=  PORTB_EN;                   //PB2 and PB3 digital enable
    GPIO_PORTB_AFSEL_R      |=  PORTB_EN;                   //PB2 and PB3 Alt Function Select
    GPIO_PORTB_ODR_R        |=  PB3_EN;                     //PB3 Open Drain Select
    GPIO_PORTB_PUR_R        |=  PORTB_EN;                   //Weak Pull up resistor for SCL and SDA
    GPIO_PORTB_PCTL_R       |=  GPIO_PCTL_PB3_I2C0SDA;      //PCTL function #3 (I2C0 SDA) of PB3
    GPIO_PORTB_PCTL_R       |=  GPIO_PCTL_PB2_I2C0SCL;      //PCTL function #3 (I2C0 SCL) of PB2

    I2C0_MCR_R               =  I2C_MCR_MFE;                //Enable I2C0 Master
    I2C0_MTPR_R              =  SYS_CLK;                    //Set I2C0 System Clock to 100Kbps
}

void store_byte(uint8_t data, uint16_t addr)
{
    uint8_t MSB              =  addr / 256;                 //Extract Most Significant Byte of addr
    uint8_t LSB              =  addr % 256;                 //Extract Least Significant Byte of addr

    I2C0_MSA_R               =  EEPROM_WRITE;               //Slave address, write operation

    write_dr(MSB, STARTEN);                                 //Send MSB of addr
    write_dr(LSB, I2CEN);                                   //Send LSB of addr
    write_dr(data, STOPEN);                                 //Generate Stop and enable I2C to send data (last byte)
}


uint8_t random_read(uint16_t addr)
{
    uint8_t MSB              =  addr / 256;                 //Extract Most Significant Byte of addr
    uint8_t LSB              =  addr % 256;                 //Extract Least Significant Byte of addr

    I2C0_MSA_R               =  EEPROM_WRITE;               //Slave address, write operation

    write_dr(MSB, STARTEN);                                 //Send MSB of addr
    write_dr(LSB, I2CEN);                                   //Send LSB of addr

    I2C0_MSA_R               =  EEPROM_READ;                //Send EEPROM Config code

    return read_dr();
}

void change_addr(uint16_t addr)
{
    uint8_t MSB              =  addr / 256;                 //Extract Most Significant Byte of addr
    uint8_t LSB              =  addr % 256;                 //Extract Least Significant Byte of addr

    I2C0_MSA_R               =  EEPROM_WRITE;               //Slave address, write operation

    write_dr(MSB, STARTEN);                                 //Send MSB of addr
    write_dr(LSB, STOPEN);                                  //Send LSB of addr
}

void write_dr(uint8_t data, uint8_t code)
{
    I2C0_MDR_R               =  data;                       //Write data to DR
    I2C0_MCS_R               =  code;                       //Write code to MCS

    while(I2C0_MCS_R & (1 << 1));                           //Busy Flag
    if(I2C0_MCS_R & (1 << 1))                               //Error Handling
    {
        print("ERROR write\n\r");
    }
    delay_us(DELAY);
}

uint8_t read_dr()
{
    I2C0_MCS_R               =  STARTSTOP;                  //Send recieve request
    while(I2C0_MCS_R & (1 << 0));                           //Busy Flag
    if(I2C0_MCS_R & (1 << 1))                               //Error Handling
    {
        print("ERROR read\n\r");
    } else
    {
        return I2C0_MDR_R;
    }
    return 0x0;
}
