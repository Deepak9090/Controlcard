/*
 * File:   DAC_Test.c
 * Author: omm
 *
 * Created on March 12, 2021, 2:57 PM
 */

#include "Config.h"
#include <xc.h>
#include <pic18f4520.h>
#define _XTAL_FREQ 20000000

#define Dac_ID 0xC0
#define Dac1 0x58 
#define Dac2 0x5A
#define Dac3 0x5C

float Ps1=0, Ps2=0, Ps3=0;

void I2C_Initialize(const unsigned long feq_K) //Begin IIC as master
{
  TRISC3 = 1;  TRISC4 = 1; TRISC2=0; //Set SDA and SCL pins as input pins
  PORTCbits.RC2=1;
  SSPCON1 = 0b00101000;    //pg84/234
  SSPCON2 = 0b00000000;    //pg85/234

  SSPADD = (unsigned char)(_XTAL_FREQ/(4*feq_K*100))-1; //Setting Clock Speed pg99/234
  SSPSTAT = 0b00000000;    //pg83/234
}
void I2C_Hold()
{
    while (   (SSPCON2 & 0b00011111)    ||    (SSPSTAT & 0b00000100)   ) ; //check the this on registers to make sure the IIC is not in progress
}
void I2C_Begin()
{
  I2C_Hold();  //Hold the program is I2C is busy 
  SEN = 1;     //Begin IIC pg85/234
}
void I2C_End()
{
  I2C_Hold(); //Hold the program is I2C is busy 
  PEN = 1;    //End IIC pg85/234
}
void I2C_Write(unsigned data)
{
    I2C_Hold(); //Hold the program is I2C is busy
    SSPBUF = (unsigned char)data;         //pg82/234
}
void Dac_Write(unsigned x,unsigned data){
    I2C_Begin();       
    I2C_Write(Dac_ID); 
    I2C_Write(x); 
    I2C_Write(0x0F & data>>8); 
    I2C_Write(data); 
    I2C_End(); 
    PORTCbits.RC2=0; 
    __delay_ms(10);
    PORTCbits.RC2=1;
}

void main(void) {
    I2C_Initialize(100); //Initialize I2C Master with 100KHz clock
//    while(1){ 
        //Phase Shifter 1
          Dac_Write(Dac1 ,(unsigned)Ps1*273);
        //Phase Shifter 2
          Dac_Write(Dac2 ,(unsigned)Ps2*273);
        //Phase Shifter 3
          Dac_Write(Dac3 ,(unsigned)Ps3*273);
 //       }
          while(1);
    return;
    }
