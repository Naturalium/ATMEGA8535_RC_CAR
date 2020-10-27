// ADC
#include <mega8535.h>
#include <delay.h>
#include <stdlib.h>
unsigned char chread = 0;
int a[3] = {0};
char ADCR[5];
int i = 0;

void ff()
{
    PORTC |= (1 << PORTC6) | (1 << PORTC4);
}


void bb()
{
    PORTC |= (1 << PORTC7) | (1 << PORTC5);
}


void fr()
{
    PORTC |= (1 << PORTC4) | (1 << PORTC7);
}


void fl()
{
    PORTC |= (1 << PORTC5) | (1 << PORTC6);  
}


char cchr(void)
{

    char rx_data = 0;   
         
    while(!(UCSRA & (1 << RXC)));
   rx_data = UDR;     
   
   return rx_data; 
}

void prbyte(char ch)
{
    while(!(UCSRA & (1 << UDRE)));    
    UDR = ch;                          
    while(!(UCSRA & (1 << TXC)));
}

void prbytes(char *p)
{
    while(*p != '\0')
    {      
        prbyte(*p);
        p++;
    }
}

void main(void)
 {    
   DDRA = (0 << DDA0) | (0 << DDA1) | (0 << DDA2);
   DDRC = (1 << DDC7) | (1 << DDC6) | (1 << DDC5) | (1 << DDC4) | (1 << DDC3) | (1 << DDC2);  // PORTC를 출력
   
   ADMUX = 0x40; // 0100 0000  기준전압, AD변환 결과조정 
   ADCSRA = 0xc7; //1100 0111
   DDRD |= (0 << DDD0) | (1 << DDD1) | (1 << DDD4) | (1 << DDD5);
   //DDRC = 0x04; 
   PORTC = 0;

   UCSRC = (1<<URSEL) | (0<<UMSEL) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UPM1) |(0<<UPM0);
   UBRRL = 103;                                                            /// parity none / baud rate 9600 / data bits 8 / stop bits 1
   
   UCSRB |= (1 << RXEN)| (1 << TXEN); 
   
   
   OCR1A=100;
    OCR1B=100;
    TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(0<<COM1A0)|(0<<COM1B0)|(1<<WGM11)|(0<<WGM10);
    TCCR1B|=(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(1<<CS11)|(0<<CS10);
    ICR1L=100;
    PORTC =(0<<PORTC7)|(0<<PORTC6)|(0<<PORTC5)|(0<<PORTC4);
   

   while (1){
       for(i = 0; i < 3; i++)
        {   
        ADMUX = i+0x40;
        ADCSRA |= (1<<ADSC);  // Start conversion   
        a[i] = ADCW;
        a[i] = (int)(20 - ((float)a[i] / 409.0 * 20.));
        itoa(a[i], ADCR);
        prbyte(i+'0');
        prbytes("nd value : ");
        prbytes(ADCR);
        prbytes("cm\r\n"); 
        }
        
        prbytes("-----------------\r\n");
        chread = cchr();
        if(chread == 's')
        {
            PORTC = 0;
        }
        else if(chread == 'd')
        {            
            bb();
        }              
        else if(chread == 'u')
        {                
            ff();
        }
        else if(chread == 'l')
        {
            fl();
        }                  
        else if(chread == 'r')
        {
            fr();
        }
        else if(chread == '1')
        {
            OCR1A = 80;
            OCR1B = 80;
        }              
        else if(chread == '2')
        {
            OCR1A = 90;    
            OCR1B = 90;
        }            
        else if(chread == '3')
        {
            OCR1A = 100;
            OCR1B = 100;
        }  
        
    }  
 }