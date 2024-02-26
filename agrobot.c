
 
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

void release()
{
    int i;
    for(i=0;i<20;i++)
        {
        RC3=1;
        __delay_us(1500);
        RC3=0;
        __delay_us(18500);
        }
}

void catch()
{
    int i;
        for(i=0;i<20;i++)
        {
        RC3=1;
        __delay_us(1000);
        RC3=0;
        __delay_us(19000);
        } 
}

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
        else if(data=='m')
        {
          x=6;
        }
       
       
    }
   

      

/*************************** main function *********************/

void main()
{
    int i;
     
    /////////////////////////////////// motor configurations ///////////
    TRISB2=0; 
    TRISB3=0;
    TRISB4=0;
    TRISB5=0;
    PORTB=0XFF;
    
    TRISC3=0;  ///////////// servo motor ////////
    TRISA5=0;   //////////// buzzer/////
    int moist;
    char e[20];
    TRISB0=0;  //////// motor pump 
    TRISA0=1; /////// moisture sensor
    RB0=1;    
     
    ADCON0=0x41;
    ADCON1=0x80;
    
     ///////////////// interrupt configuration ////////
    RCIE=1;
    GIE=1;
    PEIE=1;
   
   		///////////////// uart configuration ////////  
    TXSTA=0X24;
    RCSTA=0X90;
    SPBRG=25;
 
  while(1)
  {
    
	
    if(x==1)    ///////////////// forward ////////  
	{
    		RB2=1;
    		RB3=0;
            RB4=1;
    		RB5=0;
    }
	else if(x==2)   ///////////////// backward ////////  
	{ 
    		RB2=0;
    		RB3=1;
            RB4=0;
    		RB5=1;
    }
	else if(x==3)   ///////////////// stop ////////  
	{ 
    		RB2=0;
    		RB3=0;
            RB4=0;
    		RB5=0;
    		
    }
    else if(x==4)   ///////////////// right ////////  
        {
            RB2=1;
    		RB3=0;
            RB4=0;
    		RB5=0;
           
        }
        else if(x==5)  ///////////////// left ////////  
        {
             RB2=0;
    		RB3=0;
            RB4=1;
    		RB5=0;
            
        }
    else if(x==6)  ////////// check moisture content in water 
	{ 
    		
        catch(); ///////////////// to move the sensor down, servomotor rotation
        GO=1;
   		 while(GO==1);
    	moist=(ADRESH<<8)|ADRESL;
        sprintf(e,"%d \r\n",moist);
        trans(e);
        if(moist>750)
        {
            RB0=0;    ///// water pump on
            RA5=0;    ///// buzzer on
            __delay_ms(3000);
            RB0=1;   ///// water pump off
            RA5=1;   ///// buzzer off
        }
         __delay_ms(3000);
        release();   ///////////////to move the sensor up, servomotor rotation
    	x=0;
       
    }
  }
}
