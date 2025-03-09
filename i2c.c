/*
 * i2c.c
 *
 *  Created on: Mar 8, 2025
 *      Author: Greg Valijan
 */

#include "tm4c123gh6pm.h"

#define PORTB_EN                    (3 << 2);
#define MASTER_EN                   (1 << 4);               //Enable Master
#define SYS_CLK                     0x00000007;             //Set EEPROM Clock
#define SET_SLAVE_ADDR              0x00000076;             //Set Slave Address and write
#define EEPROM_CONFIG               (5 << 5);               //Init data sent to EEPROM: Basic control nibble + chip sel + write
#define STARTEN                     (3 << 0);               //Generate Start and enable I2C
#define STOPEN                      (5 << 0);               //Generate Stop and enable I2C
#define I2CEN                       (1 << 0);               //Enable I2C
#define STOP                        (1 << 2);               //Generate Stop

void i2c_config();
void multibyte_send(char);

void i2c_config()
{
    SYSCTL_RCGCI2C_R         =  SYSCTL_RCGCI2C_R0;          //I2C module 0 clock
    SYSCTL_RCGCGPIO_R       |=  (1 << 0);                   //Port B Clock

    GPIO_PORTB_AFSEL_R      |=  PORTB_EN;                   //PB2 and PB2 Alt Function Select
    GPIO_PORTB_ODR_R        |=  PORTB_EN;                   //PB2 and PB2 Open Drain Select
    GPIO_PORTB_PCTL_R       |=  GPIO_PCTL_PB3_I2C0SDA;      //PCTL function #3 (I2C0 SDA) of PB3
    GPIO_PORTB_PCTL_R       |=  GPIO_PCTL_PB2_I2C0SCL;      //PCTL function #3 (I2C0 SCL) of PB2
//    GPIO_PORTB_PDUR_R       |=  (1 << 3);                   //Weak Pull up resistor for SDA

    I2C0_MCR_R               =  MASTER_EN;                  //Enable I2C0 Master
    I2C0_MTPR_R              =  SYS_CLK                     //Set I2C0 System Clock to 100Kbps
}

void multibyte_send(char data)
{
    I2C0_MSA_R               =  SET_SLAVE_ADDR;             //Slave address of x3B, write operation

    set_dr(EEPROM_CONFIG, STARTEN)                          //Send EEPROM Config code
    set_dr(0x00, I2CEN);//addr high                         //Send MSB of addr
    set_dr(0x00, I2CEN);//addr low                          //Send LSB of addr
    set_dr(data, STOPEN);                                   //Generate Stop and enable I2C to send data (last byte)
}

void set_dr(char data, char code)
{
    I2C0_MDR_R               =  data;                       //Write data to DR
    I2C0_MCS_R               =  code;                       //Write code to MCS
    while(I2C0_MCS_R & (1 << 6));                           //Busy Flag
    if(I2C0_MCS_R & (1 << 1))                               //Error Handling
    {
        if(!(code & (1 << 2)) && !(I2C0_MCS_R & (1 << 4)))  //Check if stop was generated and if arbitration is lost
        {
                I2C0_MCS_R   =  STOP;                       //Generate Stop
        }
        //ERROR SERVICE
    }
}
