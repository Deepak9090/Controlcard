
#include "Config.h"
#include <xc.h> 
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <pic18f4520.h>
#define _XTAL_FREQ 20000000
#define Baud_rate 9600

bool got_data_bool=false;
char Buffer[35];
char string[35]="$,10,20,20.5,10.5,11.5,10,#";

int i=0;
char data;

void __interrupt() high_isr(void);
//***Initializing UART module for PIC16F877A***//
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
void main(void)
{
     RCONbits.IPEN = 1;         // Enable periferal intrupt
    INTCONbits.GIEH = 1;       //Enable global intrupt
    int get_value;
    
    TRISB = 0xFF; //Initialize PortB as output
    Initialize_UART();    //Initialize UART module                    
    
    UART_send_string("UART Module Initialized and active");    // Introductory Text
    UART_send_char('\n');
    while(1) //Infinite loop
    {
        if(got_data_bool){
            
            UART_send_string(&Buffer);
            got_data_bool=false;
        }
        
    }
}
// **********END of Main Function**************//
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