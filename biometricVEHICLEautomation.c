

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
#include	<stdlib.h>
#include	<stdio.h>
#include<string.h>
#define E RB2
#define _XTAL_FREQ 4000000
void data1(char a);
void cmd(char a);
char cmp[7]="GPRMC,";
void tx(char*);
long int res;
int invalid=0,g1=0,z=0;
char g[20]="AT+CMGF=1\r\n",ph[15]="9744892039",p[35], c=26,h[25];
char data,gps[20],lat[20],log[20],test[20];
char b[20],a;
void finger_print();
void gsm();
void gps_data();
main()
{   TRISC2=1;
    TRISC0=0;
    TRISC1=0;
    TRISB0=0;
    int x=0;
    TXSTA=0X24;
    RCSTA=0X90;
    SPBRG=25;
        ADCON0=0X41;
    ADCON1=0X80;
   int j,res,r=0;
   cmd(0X02);
    cmd(0X28);
    cmd(0X0C);
    cmd(0X01);
    cmd(0X80);
   float h;
   // GIE=1;
  //  PEIE=1;
    //RCIE=1;
    while(1)
    {
        
    
    finger_print();
    
        
   while(z==1)
   {gps_data();
//      sprintf(h,"%s%s",lat,log); 
        CHS0=0;
      CHS1=0;
      CHS2=0;
      GO=1;
      while(GO==1);
      res=(ADRESH<<8)|ADRESL;
        
       
     
      
          if(res>500)
       {
           
       z=0;
       break;
       //    sprintf(b,"\r\nwarning gas=%d \r\n",res); 
          // tx(b);
          // __delay_ms(10);
       }   
		
       
       
       
       
       
       
  if(RC2==0)
  {
			while(RC2==0);
			r=1;
            
  }
                         
           
     /* CHS0=1;
      CHS1=0;
      CHS2=0;
      while(GO==1);
      res=(ADRESH<<8)|ADRESL; res<850 ||  */
      if( r==1)
      {
          
          gsm();
          //send msg
      }
      
    }
}
}
void tx(char *p){
    
    int i;
    
    for(i=0;*(p+i)!='\0';i++){
        
        TXREG=*(p+i);
        
        while(TRMT==0);
        TRMT=0;
    }
}
void finger_print()
{
    RC0=0;
    RC1=0;
    int i;
    char x[11];
    invalid=g1=z=0;
   
    while(RCIF==0);
    a=RCREG;
    TXREG=a;
    while(TRMT==0);
    if(a==0xff){
        sprintf(x,"try again");
			invalid=1;
            cmd(0x01);
            cmd(0X80);
            
             for(i=0;x[i]!='\0';i++)
        {
        data1(x[i]);
        }
       
		}else if(a==0x00 || a==0x01){

            sprintf(x,"welcome");
            cmd(0x01);
            cmd(0X80);
             for(i=0;x[i]!='\0';i++)
        {
        data1(x[i]);
        }
      
			g1=1;
            z=1;
            
    	
    
    }
}
void gsm()
{    RC0=0;
     RC1=1;   
    CREN=0;
        sprintf(h,"%s%s",lat,log);
    int i;
     for(i=0;g[i]!='\0';i++)
        {
          TXREG=g[i];  
        while(TRMT==0);
        }
        __delay_ms(100);
        sprintf(p,"AT+CMGS=\"%s\"\r\n",ph);
        for(i=0;p[i]!='\0';i++)
        {
          TXREG=p[i];  
        while(TRMT==0);
        }
        __delay_ms(100);
        for(i=0;h[i]!='\0';i++)
        {
          TXREG=h[i];  
        while(TRMT==0);
        }
        __delay_ms(100);
        TXREG=c;  
        while(TRMT==0);
       __delay_ms(100);
        CREN=1;
}
void gps_data()
{
    RC0=1;
    RC1=0;
	char a;
	int i=0,x=0,y=0,z=0,count=0,f=0,log_flag=0,speed_flag=0,run=1,p=0;

	while(run)
    {

		while(RCIF==0);
		a=RCREG;
		
		if(a=='V' && p==1){
			p=0;i=0;f=0;
			
		}else if(a=='A' && p==1){
			while(RCIF==0);
			a=RCREG;	

			while(1){
		
				while(RCIF==0);
				a=RCREG;

				if(a!=',' && log_flag==0){         // ,E,0.02,
					lat[x]=a;
					x++;
				}else if(a==','){
					lat[x]='\0';
					x++;
					count++;
					if(count==2){
						log_flag=1;
					}else if(count==4){
						speed_flag=1;	
						
					}
	
				}else if(a!=',' && log_flag==1){
					log[y]=a;
					y++;

				}else if(a!=',' && speed_flag==1){ 
					
					
					z++;
				}


				if(count==3){
					log[y]='\0';
					log_flag=2;
					
				}else if(count==5){
					
					run=0;
                   
					break;
				}
			}
		}
		else if(a==',' && f==1){

			gps[i]=a;
			gps[i+1]='\0';

			if(!(strcmp(gps,cmp)))
            {
				//tx("match\r\n");
				p=1;
			}else{
				i=0;f=0;
			}
		}
		else if(f==1){
			gps[i]=a;
			i++;
		}
		if(a=='$'){
			f=1;
		}
	}
    
}
void cmd(char a)
{
    char b;
    RB0=0;
    RB1=0;
    b=a&0XF0;
    PORTB&=0X0F;
    PORTB|=b;
    E=1;
    __delay_ms(50);
    E=0;
    b=a&0X0F;
    PORTB&=0X0F;
    PORTB|=b<<4;
    E=1;
    __delay_ms(50);
    E=0;
}
void data1(char a)
{
    char b;
    RB0=1;
    RB1=0;
    b=a&0XF0;
    PORTB&=0X0F;
    PORTB|=b;
    E=1;
    __delay_ms(50);
    E=0;
    b=a&0X0F;
    PORTB&=0X0F;
    PORTB|=b<<4;
    E=1;
    __delay_ms(50);
    E=0;
}