#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB6 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <xc.h>

#include<stdio.h>
#define _XTAL_FREQ 4000000
int s=8;
void interrupt isr()
{
    char data;
   data=RCREG;
   TXREG=data;
   while(TRMT==0);    
 
           if(data=='f')
        {
              s=0;
        }
        else if(data=='b')
        {
            s=1;
        }
         else if(data=='r')
        {
            s=2;
        }
         else if(data=='l')
        {
            s=3;
        }
         else if(data=='s')
        {
            s=4;
        }
    else if(data=='m')
        {
            s=5;
        }
}
void transmit(char *p)
{
    int i;
      for(i=0;p[i]!='\0';i++)
      {
          TXREG=p[i];
          while(TRMT==0);
      }
}
void main()
{
    int gas,motion,i;
    float dist;

    char m[28],a[10],u[10],c[]="AT+CMGF=1\r\n",b[]="AT+CMGS=\"9048083221\"\r\n";
    
    	///////////////////// Setting the direction of port pins ///////
   TRISB2=0;
   TRISB3=0;
   TRISB4=0;
   TRISB5=0;
     
     //////////////////////  ultrasonic configuration  //////////////
    T1CON=0X00;
    TMR1=0;
    TRISB0=0;
    TRISB1=1;
   
   TRISC5=0;   /// DRILER MOTOR
   TRISB7=0;   /////// BUZZER
    
    TXSTA=0X24;
    RCSTA=0X90;
    SPBRG=25;
    
    
    //////////////////// ADC CONFIGURATION(MQ3) ///////////////
    
    TRISA0=1;
    TRISA1=1;
    ADCON0=0X41;
    ADCON1=0X80;
    
     //////////////////////////// INTERRUPT CONFIGURATION ////////////////////
    RCIE=1;
    GIE=1;
    PEIE=1;
    
   
    while(1)
    {
       /////////////////////////////// FORWARD ///////////////////////////////////
      	if(s==0)
		{
            
            
            RB2=1;
            RB3=0;
            RB4=1;
            RB5=0;
            
    	} 
        
        ////////////////////////////// BACKWARD ////////////////////////////////////
        else if(s==1)
		{
            RB2=0;
            RB3=1;
            RB4=0;
            RB5=1;
    	}
        
        /////////////////////////////// RIGHT ////////////////////////////
        else if(s==2){
            
            RB4=0;
            RB5=0;
            RB3=0;
            RB2=1 ;
           
            
             
        }
        /////////////////////////////// LEFT //////////////////////////////////
        else if(s==3){
            
             RB3=0;
            RB2=0;
            RB4=1;
            RB5=0;
            
            
            
           
      
        }
        ///////////////////////////////// STOP ///////////////////////////////////
         else if(s==4){
            
            
            RB4=0;
            RB5=0;
            RB2=0;
            RB3=0;
            
             
      
        }
        else if(s==5){
            
         
           RC5=1;             /////DRILLER MOTOR ON
            __delay_ms(8000);
            
             RC5=0;  /////DRILLER MOTOR OFF
             s=6;
      
        }
        ////////////////////////// ultrasonic ////////////////////
     	   RB0=1;
   	 		__delay_us(10);
    		RB0=0;
  	  
    		 while (RB1==0);
            	TMR1ON = 1;
       		 while (RB1==1);
            TMR1ON = 0;
    		time=TMR1L|(TMR1H<<8);
   		 dist=(float)time*0.017;
   		 TMR1=0;
    	sprintf(m,"distance=%f\r\n",dist);
    	tx(m);
    	__delay_ms(100);
    	
    	if(distance < 25)  //////// stop the vehicle
    	{
    		  
            RB4=0;
            RB5=0;
            RB2=0;
            RB3=0;
    		
		}
       
         CHS0=0;
       __delay_ms(20);
       GO=1;
      while(GO==1);
      motion=ADRESL|(ADRESH<<8);
      sprintf(a,"ADC=%d\r\n",motion);
      transmit(a);
      //__delay_ms(1000);
      if(motion>500)
      {
           RB4=0;
            RB5=0;
            RB2=0;
            RB3=0;
              s=7;
              
             RB7=1; ///    BUZZER ON
           
           //////////////////////////// GSM //////////////////
            transmit(c);
            __delay_ms(1500);
            transmit(b);
            __delay_ms(1500);
            transmit("human detected");
              TXREG=0X1A;
           __delay_ms(2000);
            //////////////////////////////////////////////////////
            RB7=0;   ///    BUZZER OFF
      }
      CHS0=1;
       __delay_ms(20);
       GO=1;
      while(GO==1);
      gas=(ADRESH<<8)|ADRESL;
        sprintf(u,"gas=%d\r\n",gas);
      transmit(u); 
       
       if(gas>500)
       {
           RB7=1; ///    BUZZER ON
           
           //////////////////////////// GSM //////////////////
            transmit(c);
            __delay_ms(1500);
            transmit(b);
            __delay_ms(1500);
              transmit("gas detected");
              TXREG=0X1A;
           __delay_ms(2000);
            //////////////////////////////////////////////////////
            RB7=0;   ///    BUZZER OFF
       }
        
    }
}
