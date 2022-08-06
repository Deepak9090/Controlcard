#include "Config.h"
#include <xc.h> 
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pic18f4520.h>

#define _XTAL_FREQ 20000000 

#define Baud_rate 9600
#define Dac_ID 0xC0
#define Dac1 0x58 
#define Dac2 0x5A
#define Dac3 0x5C


bool got_data_bool=false;
bool Logic=0;
float Att1, Att2, Att3;
float Ps1, Ps2, Ps3;
float Data[6];
char Buffer[35];
int i;
uint8_t Temp;


void __interrupt() high_isr(void);
void I2C_Initialize(const unsigned long feq_K);
void I2C_Hold();
void I2C_Begin();
void I2C_End();
void I2C_Write(unsigned);
void Dac_Write(unsigned,unsigned);
void Update();
void Initialize_UART(void);
void UART_send_char(char);
char UART_get_char();
void UART_send_string(char*);


uint8_t Eeprom_read(uint8_t addr){
    EEADR = addr;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.RD = 1;
    while(EECON1bits.RD);
    return EEDATA;
}

void Eeprom_write(uint8_t addr,uint8_t data){
    EEADR = addr;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    INTCONbits.GIEH = 0; 
    
    EECON2 = 0x55;
    EECON2 = 0xAA;
    
    EECON1bits.WR = 1;
    while(EECON1bits.WR);
    EECON1bits.WREN = 0;
    INTCONbits.GIEH = 1;
    
     
}

void I2C_Initialize(const unsigned long feq_K)              //Begin IIC as master
{
  TRISC3 = 1;  TRISC4 = 1; TRISC2=0;                        //Set SDA and SCL pins as input pins
  PORTCbits.RC2=1;
  SSPCON1 = 0b00101000;                                     //pg84/234
  SSPCON2 = 0b00000000;                                     //pg85/234

  SSPADD = (unsigned char)(_XTAL_FREQ/(4*feq_K*100))-1;     //Setting Clock Speed pg99/234
  SSPSTAT = 0b00000000;                                     //pg83/234
}
void I2C_Hold()
{
    while((SSPCON2 & 0b00011111)||(SSPSTAT & 0b00000100));  //check the this on registers to make sure the IIC is not in progress
}
void I2C_Begin()
{
  I2C_Hold();                                               //Hold the program is I2C is busy 
  SEN = 1;                                                  //Begin IIC pg85/234
}
void I2C_End()
{
  I2C_Hold();                                               //Hold the program is I2C is busy 
  PEN = 1;                                                  //End IIC pg85/234
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

void Update(){
    
    //Attenuator1
    Temp= (uint8_t)Att1;
    if (Att1-Temp==0.5)
        PORTDbits.RD7=0;        
    else
        PORTDbits.RD7=1;
    
    Logic=(Temp & 0x01);
    PORTDbits.RD6=!Logic;
    Logic=(Temp & 0x02)>>1;
    PORTDbits.RD5=!Logic;
    Logic=(Temp & 0x04)>>2;  
    PORTDbits.RD4=!Logic;
    Logic=(Temp & 0x08)>>3;      
    PORTDbits.RD3=!Logic;
    Logic=(Temp & 0x010)>>4;
    PORTDbits.RD2=!Logic;
            
    
    //Attenuator2
    Temp= (uint8_t)Att2;
    if (Att2-Temp==0.5)
        PORTDbits.RD1=0; 
    else
        PORTDbits.RD1=1;
    
    Logic=(Temp & 0x01);
    PORTDbits.RD0=!Logic;
    Logic=(Temp & 0x02)>>1;
    PORTBbits.RB4=!Logic;
    Logic=(Temp & 0x04)>>2;  
    PORTBbits.RB3=!Logic;
    Logic=(Temp & 0x08)>>3;      
    PORTBbits.RB2=!Logic;
    Logic=(Temp & 0x010)>>4;
    PORTBbits.RB1=!Logic;
            
    
    //Attenuator3
    Temp= (uint8_t)Att3;
    if (Att3-Temp==0.5)
        PORTBbits.RB0=0;  
    else
        PORTBbits.RB0=1;
    
    Logic=(Temp & 0x01);
    PORTAbits.RA5=!Logic;
    Logic=(Temp & 0x02)>>1;
    PORTAbits.RA4=!Logic;
    Logic=(Temp & 0x04)>>2;  
    PORTAbits.RA3=!Logic;
    Logic=(Temp & 0x08)>>3;      
    PORTAbits.RA2=!Logic;
    Logic=(Temp & 0x010)>>4;
    PORTAbits.RA1=!Logic;
    
    //Phase Shifter 1
    Dac_Write(Dac1 ,(unsigned)Ps1*273);
    __delay_ms(50);
    //Phase Shifter 2
    Dac_Write(Dac2 ,(unsigned)Ps2*273);
    __delay_ms(50);
    //Phase Shifter 3
    Dac_Write(Dac3 ,(unsigned)Ps3*273);
    __delay_ms(50);
}
void Initialize_UART(void)
{
    //****Setting I/O pins for UART****//
    TRISC6 = 1; // TX Pin set as output
    TRISC7 = 1; // RX Pin set as input
    //________I/O pins set __________//
    
    /**Initialize SPBRG register for required 
    baud rate and set BRGH for fast baud_rate**/
    SPBRG = ((_XTAL_FREQ/16)/Baud_rate) - 1;
    BRGH  = 1;  // for high baud_rate
    //_________End of baud_rate setting_________//
    
    //****Enable Asynchronous serial port*******//
    SYNC  = 0;    // Asynchronous
    SPEN  = 1;    // Enable serial port pins
    //_____Asynchronous serial port enabled_______//

    //**Lets prepare for transmission & reception**//
    TXEN  = 1;    // enable transmission
    CREN  = 1;    // enable reception
    //__UART module up and ready for transmission and reception__//
    
    //**Select 8-bit mode**//  
    TX9   = 0;    // 8-bit reception selected
    RX9   = 0;    // 8-bit reception mode selected
    //__8-bit mode selected__//    
    IPR1bits.RCIP=1;
    PIE1bits.RCIE=1;
}
//________UART module Initialized__________//

//**Function to send one byte of date to UART**//
void UART_send_char(char bt)  
{
    while(!TXIF);  // hold the program till TX buffer is free
    TXREG = bt; //Load the transmitter buffer with the received value
}
//_____________End of function________________//

//**Function to get one byte of date from UART**//
char UART_get_char()   
{
    if(OERR) // check for Error 
    {
        CREN = 0; //If error -> Reset 
        CREN = 1; //If error -> Reset 
    }
    
    while(!RCIF);  // hold the program till RX buffer is free
    
    return RCREG; //receive the value and send it to main function
}
//_____________End of function________________//

//**Function to convert string to byte**//
void UART_send_string(char* st_pt)
{
    while(*st_pt) //if there is a char
        UART_send_char(*st_pt++); //process it as a byte data
}
//___________End of function______________//

// **********START of Main Function**************//
void main(void) {
    ADCON0bits.ADON=0;         // Disable ADC
    RCONbits.IPEN = 1;         // Enable periferal intrupt
    INTCONbits.GIEH = 1;       //Enable global intrupt
    ADCON1=0x0F;               //Make port c as digital IO
    CMCON=0xff;
    TRISA=TRISB=TRISD=0x00;    //Make this ports are IO lines
    
    I2C_Initialize(100);       //Initialize I2C Master with 100KHz clock
    Initialize_UART(); 
    
    Att1=Eeprom_read(0x00);
    Att2=Eeprom_read(0x02);
    Att3=Eeprom_read(0x04);

    Temp= (uint8_t)Att1;
    if (Eeprom_read(0x01)==0x01)
        PORTDbits.RD7=0;        
    else
        PORTDbits.RD7=1;
    
    Logic=(Temp & 0x01);
    PORTDbits.RD6=!Logic;
    Logic=(Temp & 0x02)>>1;
    PORTDbits.RD5=!Logic;
    Logic=(Temp & 0x04)>>2;  
    PORTDbits.RD4=!Logic;
    Logic=(Temp & 0x08)>>3;      
    PORTDbits.RD3=!Logic;
    Logic=(Temp & 0x010)>>4;
    PORTDbits.RD2=!Logic;
            
    
    //Attenuator2
    Temp= (uint8_t)Att2;
    if (Eeprom_read(0x03)==0x01)
        PORTDbits.RD1=0; 
    else
        PORTDbits.RD1=1;
    
    Logic=(Temp & 0x01);
    PORTDbits.RD0=!Logic;
    Logic=(Temp & 0x02)>>1;
    PORTBbits.RB4=!Logic;
    Logic=(Temp & 0x04)>>2;  
    PORTBbits.RB3=!Logic;
    Logic=(Temp & 0x08)>>3;      
    PORTBbits.RB2=!Logic;
    Logic=(Temp & 0x010)>>4;
    PORTBbits.RB1=!Logic;
            
    
    //Attenuator3
    Temp= (uint8_t)Att3;
    if (Eeprom_read(0x05)==0x01)
        PORTBbits.RB0=0;  
    else
        PORTBbits.RB0=1;
    
    Logic=(Temp & 0x01);
    PORTAbits.RA5=!Logic;
    Logic=(Temp & 0x02)>>1;
    PORTAbits.RA4=!Logic;
    Logic=(Temp & 0x04)>>2;  
    PORTAbits.RA3=!Logic;
    Logic=(Temp & 0x08)>>3;      
    PORTAbits.RA2=!Logic;
    Logic=(Temp & 0x010)>>4;
    PORTAbits.RA1=!Logic;
    
    while(1){
        if(got_data_bool){
            char * token = strtok(Buffer, ",");
        if(*token=='$' ){
            int i=0;
            while( token != NULL ) {                // loop through the string to extract all other tokens
                token = strtok(NULL, ",");
                Data[i++]=(float)atof(token);
            }
            Att1=Data[0];
            Eeprom_write(0x00,(uint8_t)Att1);
            if(Att1-Eeprom_read(0x00)==0.5)
                Eeprom_write(0x01,0x01);
            else
                Eeprom_write(0x01,0x00);                               
            Att2=Data[1];
            Eeprom_write(0x02,(uint8_t)Att2);
            if(Att2-Eeprom_read(0x02)==0.5)
                Eeprom_write(0x03,0x01);
            else
                Eeprom_write(0x03,0x00);
            Att3=Data[2];
            Eeprom_write(0x04,(uint8_t)Att3);
            if(Att3-Eeprom_read(0x04)==0.5)
                Eeprom_write(0x05,0x01);
            else
                Eeprom_write(0x05,0x00);
            Ps1=Data[3];
            Ps2=Data[4];
            Ps3=Data[5];    
            Update();
            UART_send_string("Updated!");
            UART_send_char('\n');
            } 
            //UART_send_string(&Buffer);
            got_data_bool=false;
        }

    /*
    Dac_Write(Dac1 ,(unsigned)0*273);
    Dac_Write(Dac2 ,(unsigned)0*273);
    Dac_Write(Dac3 ,(unsigned)0*273);
    while(1);*/
    }
    
    //while(1);
    return;
}
void __interrupt() high_isr(void){
    INTCONbits.GIEH = 0;
    if(PIR1bits.RCIF){
        if(RCREG=='$'){
            i=0;
        }
        Buffer[i++]=RCREG;
        if(i>36||RCREG=='#'){
            while(i!=36){
                Buffer[i]=(char)NULL;
                i++;
            }
            i=0;
            got_data_bool = true;
        }
    }        
    PIR1bits.RCIF=0;
    //got_data_bool = true;
    INTCONbits.GIEH = 1;
}