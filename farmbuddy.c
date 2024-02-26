// PIC16F876A Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>
#include <stdint.h>
#include<stdio.h>
#include<stdlib.h>
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
#define  RS RB1
#define  RW RB2
#define  E RB3

#define   pir RC0
#define   mot RC1
#define   lux RC2

unsigned char t,a[95],d[13],ar[20];
unsigned int count=0,flag=1;
float res1=0,res2=0,res3=0,res4=0;
uint8_t  t1=0,sa=0,r=0,k=0,up=0,w=0,fdrc=0,i=0,val=0,val0=40,val1=40,val2=40,md=0,f1=3;//,res4=0,res1=0,res2=0,res3=0;;

//funtion declaration
  int adc(uint8_t x,uint8_t y,uint8_t o)
{
     //ADC CONVERTION
     float res;
   
        CHS2 = o;
        CHS1 = y;
        CHS0 = x;
        __delay_ms(3);
       
        GO = 1;
        while (GO == 1);
        res = ((ADRESH<< 8) | ADRESL);           
        return res;
}

void task(char s)
{
    if(s==1)mot=0;
    if(s==2)pir=0;
    __delay_ms(4000);
    if(s==1)mot=1;
    if(s==2)pir=1;   
}


void data(char c)
{
    char b;
    b = c & 0xF0;
    RS = 1;
    RW = 0;
    PORTB = PORTB & 0x0F;
    PORTB = PORTB | b;
    E = 1;
    __delay_ms(5);
    E = 0;
    b = c & 0x0F;
    PORTB = PORTB & 0x0F;
    PORTB = PORTB | (b << 4);
    E = 1;
    __delay_ms(5);
    E = 0;

}

void command(char a)
{
    char b;
    b = a & 0xF0;
    RS = 0;
    RW = 0;
    PORTB = PORTB & 0x0F;
    PORTB = PORTB | b;
    E = 1;
    __delay_ms(5);
    E = 0;
    b = a & 0x0F;
    PORTB = PORTB & 0x0F;
    PORTB = PORTB | (b << 4);
    E = 1;
    __delay_ms(5);
    E = 0;
}


void s_lcd(char *p)
{
    k=0;
        while(1)
    {
        data(p[k]);
        if(p[k+1]=='\0')break;
        k++;
    }

}
 


void tx_s(char *e)
{
    k=0;
    while(1)
        {
            TXREG=e[k];  //Load TXREG register with data to be sent
            while(!TRMT);  //Wait till transmission is complete
            if(e[k+1]=='\0')break;
            k++;
        }
}
unsigned char get_c()
{
  while(!RCIF);
  return RCREG;
}
void interrupt ISR(void)
{
    GIE=0; 
   if(RCIF)
    {
            if(RCREG==':')
            {  
                i=0;
                while(1)
                {               
                    d[i]=get_c();                   
                    
                    if(d[i]=='C')
                    {
                        d[i]='\0';
                        break;
                    }
                    i++;
                 }    
                    //tx_s(d);
            }
    }
     if(TMR1IF==1)
    {
       
       TMR1IF=0;
       TMR1=35543;
       count++;
        if(count<=1000)
        {  
            count=0;
            flag=1;
            TMR1ON=0;
        }
    }
    GIE=1;
}
void main()
  {
   
    TXSTA = 0X24;
    RCSTA = 0X90;
    SPBRG = 25;
   
    GIE=1;          //Enable Global Interrupt
    PEIE=1;         //Enable the Peripheral Interrupt
    RCIE=1;
    
    ADCON0 = 0x41;
    ADCON1 = 0x80;
   
    T1CON=0X01;
    TMR1IE=1;
    
    TRISB =0x00;
    TRISC =0x80;
    
    lux=mot=pir=1;
    command(0x02);
    command(0x28);
    command(0x0C);
    command(0x01);
    command(0x80);


    sprintf(a,">CONNECTING");
    s_lcd(a);
    sprintf(a,"AT+RST\r\n");
    tx_s(a);
    __delay_ms(4000);
    command(0x01);

       while (1)
     {  
       
      /////////////////////////////////////////////////////////////////////////////  
      //temperature sensor(A2D))
      /////////////////////////////////////////////////////////////////////////////
     
        res1=adc(0,0,0);
        sprintf(a,"T:%.0f",res1);
        r++;
        command(0x80);
        s_lcd(a);
        data(0xDF);
        data('C');
       
        /////////////////////////////////////////////////////////////////////////
        //light sensor(A2D))
        /////////////////////////////////////////////////////////////////////////
       
        res3=adc(0,1,0);
        r++;
        sprintf(a,"L:%.0f%%",res3);
        if(res3<val2)
        {lux=0;}
        else
        {lux=1;}
        command(0xC0);
        s_lcd(a);  

        //////////////////////////////////////////////////////////////////////////
        //moisture sensor(A2D))
        //////////////////////////////////////////////////////////////////////////
     
        res2=adc(1,0,0);
        sprintf(a,"M:%.0f%%",res2);
        r++;
        command(0xC8);
        s_lcd(a);

       ///////////////////////////////////////////////////////////////////////////
       //PIR motion sensor(A2D))
       ///////////////////////////////////////////////////////////////////////////

        res4=adc(1,1,0);    
        res4 = (res4>500)?1:0;
        /////////////////////////////////////////////////////////////////////////
        //various conditions to sent notification to lcd
        //temperature warning
       
 
          if(res1>val0)
          {
              command(0x01);
              sprintf(a,"TEMPERATURE HIGH");
              task(1);
              command(0x01);
          }

        //moisture warning
          if(res2<val1)//noti(2);
          {
           command(0x01);
           sprintf(a,"MOISTURE DOWN");
           task(1);
           command(0x01);
          }  
            
        //motion warning
          if((res4==1)&&(t1==1))
        {
            command(0x01);
            md=1;  
            task(2);
            command(0x01);
          }

   res1=res2=res3=res4=1;
   
    //data updating to cloud
    if(flag==1)
    {  
        
        f1++;
        md=0;
      // cloud_connect(87,1);
       sprintf(a,"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
       __delay_ms(3000);
       tx_s(a);
       sprintf(a,"GET /update?api_key=CYZTZNYNG08ZZ4EE&field1=%.0f&field2=%.0f&field3=%.0f&field4=%d\n\r\n\r",res1,res2,res3,md);
       sa=strlen(a);
       sprintf(ar,"AT+CIPSEND=%d\r\n",(sa-2));
       __delay_ms(2000);
       tx_s(ar);
       __delay_ms(1000);
       tx_s(a);  
       __delay_ms(2000);     
            flag=0;
            TMR1ON=1;
            TMR1=35543; 
        }
   
   
         /////////////////////////////////////////////////////////////////////////////  
      //reading  value from cloud
      /////////////////////////////////////////////////////////////////////////////
       if(f1==4)
        {
           t1=1;
           TMR1ON=0;
            f1=0;
            while(1)
       {
       //    cloud_connect(63,2);
       sprintf(a,"AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
       __delay_ms(3000);
       tx_s(a);
       sprintf(a,"AT+CIPSEND=64\r\n");
       __delay_ms(2000);
       tx_s(a);
       up=w+5;
       sprintf(a,"GET /channels/1001164/fields/%d/last?api_key=GBZNZOJCP1QSYWMT\n\r\n\r",up);
       __delay_ms(1000);     
        i=0;
        RCIE=1;
        fdrc=1;
        tx_s(a);
       __delay_ms(2000); 
       tx_s(d);
       val = atoi(d);
            if(up==5)val0=val;  
            if(up==6)val1=val;
            if(up==7)
                {
               val2=val;
               TMR1ON=1;
               TMR1=35543;
               break;
               }
       w++;
       }      
}
    }


}