#include <LPC214X.H>
#include<stdio.H>
#include<string.H>


void tx1(char *p);
void tx0(char *p);

int count=1,no=0;
char cn[15];

int i,k,p=0,f=0,l,m,n,sum=0;
char a[20],u[10],add[15];
char x[5]="792",y[5]="589",z[5]="386";
int r1=0,r2=0,r3=0;


//---------------fn to give line feed----------------------     
void line_feed(int a)
{
int i;
for(i=0;i<a;i++)
{
U1THR=0x0A;      //line feed
while((U0LSR&(1<<6))==0);
}
}
 //------------------------------------------  

//------------------fn for double_height and width----------------

void double_hw()
{
U1THR=0X1B;    //ESC hex value
while((U0LSR&(1<<6))==0);
        
U1THR=0X21;          //! hex value
while((U0LSR&(1<<6))==0);
        
U1THR=0X31;   //DOUBLE height and width
while((U0LSR&(1<<6))==0);
}



//----------------------------------------------------------

void print()
{
count++;
double_hw();  //fn for double height and width
        
tx1("     QIS ACADEMY");
line_feed(1);   

U1THR=0X1B;    //ESC hex value
while((U0LSR&(1<<6))==0);
        
U1THR=0X21;          //! hex value
while((U0LSR&(1<<6))==0);
        
U1THR=0X00;   //normal
while((U0LSR&(1<<6))==0);

sprintf(cn,"Bill No:-QIS100%d",count);
tx1(cn);
      
      line_feed(2);
        
       	tx1("Sl.No     product     MRP");
      line_feed(4); 

	  if(r1==2)
	  {	 no++;
	  sprintf(cn,"  %d        apple       30 ",no);
    	 tx1(cn);
		       line_feed(2);
      }
	  if(r2==2)
	  {
    	 no++;
	  sprintf(cn,"  %d        orange      20 ",no) ;
    	 tx1(cn);
		       line_feed(2);
	  }
	  if(r3==2)
	  { 
	  no++;
	  sprintf(cn,"  %d        grape       10 ",no)	;
    	 tx1(cn);
		       line_feed(2);
	  }

	  line_feed(4);
 
         tx1("-----------------------------");

      line_feed(2);  

				   sprintf(cn,"Total                Rs %d",sum);
        	 tx1(cn);  //29 char-max for a single line

      line_feed(3);  
  
      double_hw();
        
   		  tx1("      THANK YOU");  //6 SPACES
 //-------------------------------------   
      line_feed(8);
        
	   no=0;
	   r1=0;
	   r2=0;
	   r3=0;
         sum=0;
}
//----------------------------------------------------

__irq void uart1()
{

if((U0IIR&0X0E)==4)
{
a[p]=U0RBR;

if(a[p]=='\r')
{
a[p]='\0';
f=1;
}

else
{
p++;
}

}
VICVectAddr=0;
}




main()
{
PINSEL0|=(1<<0)|(1<<2)|(1<<16)|(1<<18);

IODIR0&=~(1<<2);

U1LCR=0X83;
U1DLL=97;
U1LCR=0X03;

U0LCR=0X83;
U0DLL=97;
U0LCR=0X03;

U0IER=1;

VICIntSelect=0;
VICIntEnable=1<<6;
VICDefVectAddr=(unsigned)uart1;

while(1)
{
if((IOPIN0&1<<2)==0)
{
while((IOPIN0&1<<2)==0);
print();
}


for(k=0,i=7;i<10;i++,k++)
{
u[k]=a[i];
}

if(f==1)
{  

l=strcmp(u,x);
m=strcmp(u,y);
n=strcmp(u,z);

if(l==0 && r1==0)
{
sum=sum+30;
tx0("apple added to cart");
r1++;
}
if(m==0 && r2==0)
{
sum=sum+20;
tx0("orange added to cart");
r2++;
}
if(n==0 && r3==0)
{
sum=sum+10;
tx0("grape added to cart");
r3++;
}


if(l==0 && r1==2)
{
sum=sum-30;
tx0("apple removed from cart");
r1=0;
}
if(m==0 && r2==2)
{
sum=sum-20;
tx0("orange removed from cart");
r2=0;
}
if(n==0 && r3==2)
{
sum=sum-10;
tx0("grape removed from cart");
r3=0;
}

if(l==0 && r1==1)
{ r1++; }
if(m==0 && r2==1)
{ r2++; }
if(n==0 && r3==1)
{ r3++; }

sprintf(add,"TOTAL= %d",sum);
tx0(add);

f=0;
p=0;
}



}
}

void tx1(char *p)
{
int i;
for(i=0;p[i]!='\0';i++)
{
U1THR=p[i];
while((U1LSR&(1<<6))==0);
}
}


void tx0(char *p)
{
for(i=0;p[i]!='\0';i++)
{
U0THR=p[i];
while((U0LSR&(1<<6))==0);
}
U0THR='\r';
while((U0LSR&(1<<6))==0);
U0THR='\n';
while((U0LSR&(1<<6))==0);
}
