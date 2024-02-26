/*
 * File:   TCS3200.c
 * Author: SujithKT
 *
 * Created on 16 April, 2021, 6:06 PM
 */



// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include<stdio.h>

#define _XTAL_FREQ 4000000
#define S0 RB2
#define S1 RB3
#define S2 RB4
#define S3 RB5
#define out RB6

#define RS RC0
#define RW RC1
#define E RC2

void catch();
void release();
void base_center();
void base_left();
void base_right();
void found_green();
void found_red();
void data(char);
void command(char);

int freq();
void tx(char *a);

int RED=0,BLUE=0,GREEN=0,i;
char color[50];

void main()
{
    TRISC=0x00;
    
    command(0x02);
    command(0x28);
    command(0x0C);
    command(0x01);
    command(0x80);
    
    TRISA=0x00;
   
    TRISB2=0;
    TRISB3=0;
    TRISB4=0;
    TRISB5=0;
    TRISB6=1;
    
    T1CON=0x20;
    
//    TXSTA=0x24;
//    RCSTA=0x90;
//    SPBRG=25;
    
    S0=1;
    S1=1;
    tx("GETTING READY");
    base_center();
    release();
    
    while(1)
    {
        S2=0;
        S3=0;
        for(i=0;i<20;i++)
        {
        RED+=freq();
        }
        RED=RED/20;
        S2=0;
        S3=1;
        for(i=0;i<20;i++)
        {
        BLUE+=freq();
        }
        BLUE=BLUE/20;
        S2=1;
        S3=1;
        for(i=0;i<20;i++)
        {
        GREEN+=freq();
        }
        GREEN=GREEN/20;
//        sprintf(color,"red=%d   blue=%d   green=%d\r\n",RED,BLUE,GREEN);
//        tx(color);
        if(RED<30)
        {
            tx("RED OBJECT");
            found_red();
            tx("GETTING READY....");
            RED=0;
        BLUE=0;
        GREEN=0;
        }
        if(RED>40 && GREEN<80)
        {
            tx("GREEN OBJECT");
            found_green();
            tx("GETTING READY....");
            RED=0;
        BLUE=0;
        GREEN=0;
        }
        __delay_ms(200);
        RED=0;
        BLUE=0;
        GREEN=0;
    }
}

void found_red()
{
    catch();
    __delay_ms(1000);
    base_left();
    __delay_ms(1000);
    release();
    __delay_ms(1000);
    base_center();
}

void found_green()
{
    catch();
    __delay_ms(1000);
    base_right();
    __delay_ms(1000);
    release();
    __delay_ms(1000);
    base_center();
}

void tx(char *a)
{
//    int i;
//    for(i=0;a[i];i++)
//    {
//        TXREG=a[i];
//        while(TRMT==0);
//    }
    command(0x01);
    command(0x80);
    int i;
    for(i=0;a[i];i++)
    {
        data(a[i]);
    }
}

int freq()
{
    int time;
    while(1)
    {
        if(out==0)
        {
            TMR1=0;
            while(out==0);
            TMR1ON=1;
            while(out==1);
            while(out==0);
            TMR1ON=0;
            time=TMR1L|(TMR1H<<8);
            time=time*4;
            break;
        }
    }
    return time;
}

void base_right()
{
    int i;
    for(i=0;i<20;i++)
    {
        RA2=1;
        __delay_us(1000);
        RA2=0;
        __delay_us(19000);
        //__delay_ms(100);
    }
    
}

void base_center()
{
    int i;
    for(i=0;i<20;i++)
    {
        RA2=1;
        __delay_us(1750);
        RA2=0;
        __delay_us(18250);
        //__delay_ms(100);
    }
    
}


void base_left()
{
    int i;
    for(i=0;i<20;i++)
    {
        RA2=1;
        __delay_us(2800);
        RA2=0;
        __delay_us(17200);
       // __delay_ms(100);
    }
    
}

void release()
{
    int i;
    for(i=0;i<20;i++)
    {
        RA0=1;
        __delay_us(1500);
        RA0=0;
        __delay_us(18500);
    }
}

void catch()
{
    int i;
    for(i=0;i<20;i++)
    {
        RA0=1;
        __delay_us(2500);
        RA0=0;
        __delay_us(17500);
    }
    
}

void command(char a)
{
    char b;
    b=a&0xF0;
    RS=0;
    RW=0;
    PORTC=PORTC&0x0F;
    PORTC=PORTC|b;
    E=1;
    __delay_ms(20);
    E=0;
    
    b=a&0x0F;
    PORTC=PORTC&0x0F;
    PORTC=PORTC|b<<4;
    E=1;
    __delay_ms(20);
    E=0;
}

void data(char a)
{
    char b;
    b=a&0xF0;
    RS=1;
    RW=0;
    PORTC=PORTC&0x0F;
    PORTC=PORTC|b;
    E=1;
    __delay_ms(20);
    E=0;
    
    b=a&0x0F;
    PORTC=PORTC&0x0F;
    PORTC=PORTC|b<<4;
    E=1;
    __delay_ms(20);
    E=0;
}