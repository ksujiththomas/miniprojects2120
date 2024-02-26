/* 
 * File:   1234.c
 * Author: SujithKT
 *
 * Created on 27 February, 2020, 3:16 PM
 */

#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */

// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

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

void tx(char *p);
void gsm(char *p);
char a[20];
int time;
float dist;
int flag=0,r,n=0;

char h[55]="I'M IN DANGER HELP ME.......10.1260 N , 76.3340 E\r\n";
char x[50]="I'M IN DANGER HELP ME.......HIGH TEMPERATURE\r\n";
void main()
{
    TRISB0=1;
    TRISB1=0;
    
    TRISC0=1;
    TRISC2=0;
    TRISC6=0;
    TRISC7=1;
    
    INTE=1;
    GIE=1;
    INTEDG=1;

    ADCON0=0X41;
    ADCON1=0X80;
    
    TXSTA=0x24;
    RCSTA=0x90;
    SPBRG=25;
    
    while(1)
    {
    //ultrasonic sensor trigger  
    RB1=1;
    __delay_us(10);
    RB1=0;
   //LM35 adc
    GO=1;
    while(GO==1);
    r=ADRESL|(ADRESH<<8);
    r=r/2.048;
    
    if(n==1)
    {
        gsm(x);
        n=2;
    }   
    if(r<40)
    {
        n=0;
    }
    
    if(r>40&&n!=2)
    {
      n=1;
    }
    
    if(RC0==0)
    {
        while(RC0==0);
        gsm(h);
    }
    
    
        if(flag==1)
        {
               
                time=TMR1L|(TMR1H<<8);
                dist=(float)time * 0.017;
                TMR1=0;
             
                if (dist<50)
                {
                    RC2=0;
                }
                else
                {
                    RC2=1;
                }
                flag=0;
        }
    }
   
}

void interrupt ISR()
{
    GIE=0;
    TMR1ON=1;
    while(RB0==1);
    TMR1ON=0;
    flag=1;
    INTF=0;
    GIE=1;
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

void gsm(char *p)
{
    char a[5]="AT\r\n",b[20]="AT+CMGF=1\r\n",c[50]="AT+CMGS=\"7306426641\"\r\n";
    char d=26;
    
     tx(a);
__delay_ms(1000);
     tx(b);
__delay_ms(1000);
     tx(c);
__delay_ms(1000);
     tx(p);
__delay_ms(1000);
     TXREG=d;
     while(TRMT==0);
     __delay_ms(100);
}


