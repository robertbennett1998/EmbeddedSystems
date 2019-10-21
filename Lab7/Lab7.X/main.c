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

inline void SetLowNibble(uchar* val, uchar newVal)
{
    *val = (*val & 0xF0) | newVal;
}

inline void SetHighNibble(uchar* val, uchar newVal)
{
    *val = (*val & 0x0F) | newVal << 4;
}

#define RS RA1
#define RW RA2
#define Enable RA3

/*
 * RS - RA1 - High for data input - Low for instruction
 * RW - RA2 - High is read - Low is write
 * Enable RA3
 * Data: RD0 - RD7 (RD4 - RD7 is used for 4 bit)
 */

void delay(){int i;for(i=0;i<5000;i++);}

inline void WriteCommand(uchar cmd)
{
    TRISD = 0x00;
    RS = 0;
    RW = 0; 
    PORTD = cmd;   
    Enable = 0;
    delay();
    Enable = 1;
    PORTD = 0;
}
inline void SetDisplayMode(bool displayOn, bool cursorOn, bool cursorBlink)
{
    WriteCommand(0b00001000 | displayOn << 2 | cursorOn << 1 | cursorBlink);  
}

//If display two lines is true, display large font will be ignored
inline void SetDisplayResolution(bool displayTwoLines, bool displayLargeFont)
{
    WriteCommand(0b00110000 | displayTwoLines << 3 | displayLargeFont << 2);
}

inline void ClearDisplay()
{
    WriteCommand(0x1);
}

void ShiftCursor(bool right)
{
    WriteCommand(0b00010000 | right << 2);
}

void ShiftDisplay(bool right)
{
    WriteCommand(0b00011000 | right << 2);
}

inline void MoveCursorToStart()
{
    WriteCommand(0x02);
}

void SetDdramAddress(uchar addr)
{
    WriteCommand(0b10000000 | addr);
}

inline void SetCursorPosition(bool secondLine, uchar pos)
{
    MoveCursorToStart();
    if (pos > 39)
        pos = 39;
    
    if (secondLine)
        pos += 40;
    
    SetDdramAddress(pos);
}

inline void WriteCharacter(char c)
{
    RS = 1;
    RW = 0;
    PORTD = c;
    Enable = 0;
    delay();
    Enable = 1;
}

inline void Initialise();   
void main(void)
{
    Initialise();

    SetDisplayMode(true, true, true);  
    ClearDisplay();
    SetDisplayResolution(true, false);
    WriteCharacter('a');
    WriteCharacter('b');
    ShiftCursor(true);
    WriteCharacter('c');
    SetCursorPosition(true, 0);
    while (true)
    {
        SetDisplayMode(true, true, true);
    }
        
    return;
}

inline void Initialise()
{
    // 1: input 0: output
    ADCON1 = 0x07;
    TRISA = 0b00000000;
    TRISD = 0b00000000;
    PORTA = 0x00;
    PORTD = 0x00;
}