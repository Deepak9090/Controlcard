/*
 * File:   Att_Ps.c
 * Author: omm
 *
 * Created on March 15, 2021, 4:28 PM
 */

#include "Config.h"
#include <xc.h> 
#include <stdint.h>
#include <stdbool.h>
#include <pic18f4520.h>

float Att1=0.5, Att2=0.5, Att3=0.5;
float Ps1, Ps2, Ps3;

void Update(){
    uint8_t Temp;
    
    //Attenuator1
    Temp= (uint8_t)Att1;
    if (Att1-Temp==0.5)
        PORTDbits.RD7=1;        
    else
        PORTDbits.RD7=0;
    
    PORTDbits.RD6=(Temp & 0x01);
    PORTDbits.RD5=(Temp & 0x02)>>1;
    PORTDbits.RD4=(Temp & 0x04)>>2;
    PORTDbits.RD3=(Temp & 0x08)>>3;
    PORTDbits.RD2=(Temp & 0x010)>>4;
    
    //Attenuator2
    Temp= (uint8_t)Att2;
    if (Att2-Temp==0.5)
        PORTDbits.RD1=1; 
    else
        PORTDbits.RD1=0;
    
    PORTDbits.RD0=(Temp & 0x01);
    PORTBbits.RB4=(Temp & 0x02)>>1;
    PORTBbits.RB3=(Temp & 0x04)>>2;
    PORTBbits.RB2=(Temp & 0x08)>>3;
    PORTBbits.RB1=(Temp & 0x010)>>4;
    
    //Attenuator3
    Temp= (uint8_t)Att3;
    if (Att3-Temp==0.5)
        PORTBbits.RB0=1;  
    else
        PORTBbits.RB0=0;
    
    PORTAbits.RA5=(Temp & 0x01);
    PORTAbits.RA4=(Temp & 0x02)>>1;
    PORTAbits.RA3=(Temp & 0x04)>>2;
    PORTAbits.RA2=(Temp & 0x08)>>3;
    PORTAbits.RA1=(Temp & 0x010)>>4; 
    
    //Phase Shifter 1
    //Dac_Write(Dac1 ,Ps1*273);
    //Phase Shifter 2
    //Dac_Write(Dac1 ,Ps2*273);
    //Phase Shifter 3
    //Dac_Write(Dac1 ,Ps3*273);
}
void main(void) {
    ADCON0bits.ADON=0;
    ADCON1=0x0F;
    TRISA=TRISB=TRISD=0x00;
    
    Update();       
    return;
}
