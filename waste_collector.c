
 
// direction button connected to pin RB1

// define motor control pins

 #pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>
#define _XTAL_FREQ 4000000
#include <stdio.h>        // include stdint header
int x=0; 


void trans(char *p)
{
    int i;
    for(i=0;*(p+i)!='\0';i++)
    {
        TXREG=*(p+i);
        while(TRMT==0);
    }
}

void interrupt isr()
{
 char data;
  
  
   data=RCREG;
   TXREG=data;
    while(TRMT==0);    
           if(data=='f')
        {
         x=1;
        }
        else if(data=='b')
        {
           x=2;
        }
        
        else if(data=='s')
        {
           x=3;
        }
        else if(data=='r')
        {
          x=4;
        }
        else if(data=='l')
        {
            x=5;
        }
       
       
       
    }
   

      

/*************************** main function *********************/

void main()
{
    ///////////// wheel motors /////////
    int i;
    TRISB2=0;
    TRISB3=0;
    TRISB4=0;
    TRISB5=0;
    
    ///////////// garbage collector motors ////////////////
    
    TRISC0=0;
    TRISC3=0;
    TRISC4=0;
    TRISC5=0;

    ///////////// UART interrupt /////////    
    
    RCIE=1;
    GIE=1;
    PEIE=1;
   
    TXSTA=0X24;
    RCSTA=0X90;
    SPBRG=25;
  int j=51;
  
  // PWM configuration
    TRISC2=0;
    CCP1CON=0X0f;
    T2CON=0X05;
    TMR2=0;
    PR2=249;
    CCPR1L=124;
    __delay_ms(1000);   // wait 1 second
 
  while(1)
  {
     RC0=1;
     RC3=0;
     RC4=1;
     RC5=0;
	 if(x==1)      ///////////// forward ///////////////
	{
    		RB2=1;
    		RB3=0;
            RB4=1;
    		RB5=0;
    }
	else if(x==2)   ///////////// backward ///////////////
	{ 
    		RB2=0;
    		RB3=1;
            RB4=0;
    		RB5=1;
    }
	else if(x==3)  ///////////// stop ///////////////
	{ 
    		RB2=0;
    		RB3=0;
            RB4=0;
    		RB5=0;
    		
    }
  
    else if(x==4) ///////////// right ///////////////
	{
            
            RB3=RB2=0;
            RB4=1;
            RB5=0;
            
             
   }
	else if(x==5)    ///////////// left ///////////////
	{
            
			RB5=RB4=0;
            RB3=1;
            RB2=0;
            
    }
        
  }
}
