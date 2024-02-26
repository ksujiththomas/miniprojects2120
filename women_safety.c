/*
 * File:   women_s.c
 * Author: KSUJITHTHOMAS
 *
 * Created on April 10, 2021, 11:22 AM
 */


// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include<stdio.h>
#include<string.h>

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 4000000
#define buz RB4
#define sw RB3

char h[50];
void tx(char*);
void gsm();
void gps();
char str[]="GPRMC,";
int f1=0,x=0;
char b[10],lat[20],lon[20];
    char data,d1[8],_d1[]="GPRMC,";
	int i=0,c=1,j=0,k=0,f=1;
    char aa[10]="AT\n\r",bb[]="AT+CMGF=1\r\n",cc[]="AT+CMGS=\"8848435522\"\r\n",h1[]="\r\nNeed help!!\r\n";
    
int x,heartbeat;

void main()
{
   
    TRISB3=1;
    TRISB4=0;
    TRISC6=0;
    TRISC7=1;
    TXSTA=0X24;
    RCSTA=0X90;
    SPBRG=25;
    TXEN=0;
    CREN=0;

    ADCON0=0X41;
    ADCON1=0X80;
    
    while(1)
    {
	//heartbeat measurement
        CHS2=0;CHS1=0;CHS0=1;
        for(i=0;i<20;i++)
        {
        GO=1;
        while(GO==1);
        x=ADRESL|(ADRESH<<8);
        heartbeat+=x;
        }
        heartbeat=heartbeat/200;

        if(sw)        
        {
            while(sw);
            buz=0; //sound 
            gps();
            __delay_ms(1000);
            gsm();
            __delay_ms(5000);
            buz=1;  
       }   
    }
}
void gsm()
{

    TXEN=1;
    char d=26,beat[20];

     tx(aa);
     __delay_ms(1500);
     tx(bb);
     __delay_ms(1500);
     tx(cc);
     __delay_ms(1500);
     tx(h);
     __delay_ms(1500);
     tx(h1);
     __delay_ms(1000);
	sprintf(beat,"heartbeat=%d",heartbeat);
	tx(beat);
	__delay_ms(1000);
     TXREG=d;
     while(TRMT==0);
     __delay_ms(1000);
     TXEN=0;
}

void gps()
{
   
    CREN=1;
 i=0,c=1,j=0,k=0,f=1;

	while(f)
    {

		while(RCIF==0);
		data=RCREG;
		if(data=='$')
        {
        i=0;
		while(i!=6)
	      {
		while(RCIF==0);
		d1[i]=RCREG;
                i++;
	      }
                d1[i+1]='\0';
	        c=strcmp(d1,_d1);
                i=0;
              if(c==0)
              {
                while(j!=2)
                {
                    while(RCIF==0);
                    data=RCREG;
                    if(data==',')j++;
                    
                }
	        
		   while(k!=4)
                     {
               
                      while(RCIF==0);
		              h[i]=RCREG;
                      if(h[i]==',')k++;
                      i++;
                    }
		 h[i-1]='\0';f=0;CREN=0;

                 
              }
        }
		
   }
}
void tx(char *p)
{
 int i;
 
 for(i=0;p[i]!='\0';i++)
 {
     TXREG=p[i];
     while(TRMT==0);
 }

}
