/*
 * File:   corona.c
 * Author: SujithTK
 *
 * Created on 30 April, 2021, 9:08 AM
 */



// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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
void tx(char *);
void gps_data_read();
void location();

char h[45],lat[15],lon[15];
int i=0,c=1,j=0,k=0,f=1;

int r=0,n=0,x,heartbeat,audio,flag=0,count=0,sec=0,cough=0;
float temp;
char result[30];

void main()
{   
    TRISB0=0;
    TRISB1=0;
    RB0=1;
    
    OPTION_REG=0x01;
    TMR0IE=1;
    RCIE=1;
    PEIE=1;
    GIE=1;
    
    ADCON0=0X41;
    ADCON1=0X80;
    
    TXSTA=0x24;
    RCSTA=0x90;
    SPBRG=25;
    
    tx("clip on your finger\r\n");
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
        
        if(heartbeat>50)
        {
            RB1=0;
            break;
        }
        else
        {
            RB1=1;
            __delay_ms(500);
            RB1=0;
            __delay_ms(500);
        }
    }
    
    
    while(1)
    {
        //LM35 temp 
        CHS2=0;CHS1=0;CHS0=0;
        for(i=0;i<20;i++)
        {
        GO=1;
        while(GO==1);
        x=ADRESL|(ADRESH<<8);
        r+=x;
        }
        r=r/20;
        temp=(float)r/2.048;
        
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
        
        //audio module  
        CHS2=0;CHS1=1;CHS0=0;
        GO=1;
        while(GO==1);
        x=ADRESL|(ADRESH<<8);
        audio=x;
       //temperature 
        if(n==1)
        {
            RB0=0;
            __delay_ms(1000);
            RB0=1;
            location();
            sprintf(result,"Temp=%0.2f Heartbeat=%d ",temp,heartbeat);
            tx(result);
            tx(h);
            tx("\r\n");
            n=2;
        }   
        
        if(temp<40)
        {
        n=0;
        }
    
        if(temp>40&&n!=2)
        {
        n=1;
        }
        ///cough
        if(audio>100)
        {
         cough++;   
        }
        if(count>=1000)
        {
            sec++;
            if(sec>=29)
            {
             flag=1;   
             sec=0;
            }
        count=0;
        }
        if(flag==1)
        {
            if(cough>3)
            {
            RB0=0;
            __delay_ms(1000);
            RB0=1;
            location();
            sprintf(result,"Temp=%0.2f Heartbeat=%d ",temp,heartbeat);
            tx(result);
            tx(h);
            tx("\r\n");
            }
            cough=0;
            flag=0;
        }
        
        sprintf(a,"temp=%.2f HB=%d audio=%d cough=%d sec=%d\r\n",temp,heartbeat,audio,cough,sec);
        tx(a);
        __delay_ms(100);
        
        heartbeat=temp=audio=0;
        
    }
}

void interrupt ISR()
{
    GIE=0;
    if(TMR0IF==1)
    {
    count++;
    TMR0=5;
    TMR0IF=0;
    }
    if(RCIF==1)
    {
        h[j]=RCREG;
        if(h[j]=='\r')
        {
            h[j]='\0';
            j=0;
        }
        else
        {
        j++;
        }
    }
    
    GIE=1;
}

void location()
{
    int i,k;
    float f1,f2;
    
    for(i=0;h[i];i++)
    {
        if(h[i]==',')
        {
            break;
        }
        lat[i]=h[i];
    }
    i=i+3;
    for(k=0;h[i];k++,i++)
    {
        if(h[i]==',')
        {
            break;
        }
        lon[k]=h[i];
    }
    f1=atof(lat);
    f2=atof(lon);
    f1=f1/100;
    f2=f2/100;
    sprintf(h,"http://maps.google.com/maps?q=%f,%f\r\n",f1,f2);
}

void tx(char *a)
{
    int i;
    for(i=0;a[i];i++)
    {
        TXREG=a[i];
        while(TRMT==0);
    }
}


