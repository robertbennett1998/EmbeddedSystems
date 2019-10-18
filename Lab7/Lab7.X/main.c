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
typedef unsigned char uchar;
typedef uchar bool;

inline void SetBitHigh(uchar* byValue, uchar byBitOffset)
{
    *byValue |= (1U << byBitOffset);
}

inline void SetBitLow(uchar* byValue, uchar byBitOffset)
{
    *byValue &= ~(1U << byBitOffset);
}

inline void SetBit(uchar* byValue, uchar byBitOffset, uchar byBitValue)
{
    if (byBitValue > 0)
        SetBitHigh(byValue, byBitOffset);
    else
        SetBitLow(byValue, byBitOffset);
}

bool IsBitSet(uchar byValue, uchar byBitOffset)
{
    return (byValue >> byBitOffset) & 1U;
}

inline void ClearLowNibble(uchar* val)
{
    *val = *val & 0xF0;
}

inline void ClearHighNibble(uchar* val)
{
    *val = *val & 0x0F;
}

inline uchar GetLowNibble(uchar* val)
{
    return *val & 0xF0;
}

inline uchar GetHighNibble(uchar* val)
{
    return (*val & 0xF0) >> 4;
}

void SetColumnHigh(uchar col)
{
    ClearLowNibble(&PORTC);
    SetBitHigh(&PORTC, col);
}

uchar ReadMatrixColumn(uchar col)
{
    PORTC = 0;
    TRISC = 0b11110000;
    SetColumnHigh(col);  
    return GetHighNibble(&PORTC);
}

void SetRowHigh(uchar row)
{
    ClearLowNibble(&PORTC);
    SetBitHigh(&PORTC, row + 4);
}

uchar ReadMatrixRow(uchar row)
{
    PORTC = 0;
    TRISC = 0b00001111;
    SetRowHigh(row);  
    return GetLowNibble(&PORTC);
}

inline void Initialise();   
void main(void)
{
    Initialise();
    
    uchar i;
    while (true)
    {
        for (i = 0; i < 4; i++)
        {
            PORTA = 0;
            PORTB = 0;
            uchar res = ReadMatrixColumn(i); 
            
            res &= ReadMatrixRow(i);
            PORTA = res;
            
            if (PORTA > 0)
                SetBitHigh(&PORTB, i);
        }
    }
    
    return;
}

inline void Initialise()
{
    // 1: input 0: output
    ADCON1 = 0b00000110; //set all pins to digital (port a)
    TRISA = 0b11110000;
    TRISB = 0b11110000;
    TRISC = 0b11110000; // set 0-4 AS INPUT and 4-7 as OUTPUT
    TRISA = 0x00;
    PORTA = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
}