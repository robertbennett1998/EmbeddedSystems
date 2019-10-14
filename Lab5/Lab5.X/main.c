#include <xc.h>

//begin config
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF//Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = ON// Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF// Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit SerialProgramming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
//end config

#define true 1
#define false 0
typedef char bool;

void SetBitHigh(char* byValue, char byBitOffset)
{
    *byValue |= (1U << byBitOffset);
}

void SetBitLow(char* byValue, char byBitOffset)
{
    *byValue &= ~(1U << byBitOffset);
}

void SetBit(char* byValue, char byBitOffset, char byBitValue)
{
    if (byBitValue > 0)
        SetBitHigh(byValue, byBitOffset);
    else
        SetBitLow(byValue, byBitOffset);
}

bool IsBitSet(char byValue, char byBitOffset)
{
    return (byValue >> byBitOffset) & 1U;
}

void main(void) 
{
    ADCON1 = 0b00000110; //set all pins to digital (port a)
    TRISA = 0b00001111; //set first 4 to input
    TRISB = 0b00001111; //set first 4 to input
    TRISC = 0x00;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    
    while (1)
    {
        for (char i = 0; i < 4; i++)
        {
            SetBit(&PORTC, i, IsBitSet(PORTA, i));
            SetBit(&PORTC, i + 4, IsBitSet(PORTB, i));
        }
    };   
    
    return;
}
