; DIGIT DEFS
#DEFINE VAL_DIGIT_ZERO 0xC0
#DEFINE VAL_DIGIT_ONE 0xF9
#DEFINE VAL_DIGIT_TWO 0xA4
#DEFINE VAL_DIGIT_THREE 0xB0
#DEFINE VAL_DIGIT_FOUR 0x99
#DEFINE VAL_DIGIT_FIVE 0x92
#DEFINE VAL_DIGIT_SIX 0x82
#DEFINE VAL_DIGIT_SEVEN 0xF8
#DEFINE VAL_DIGIT_EIGHT 0x80
#DEFINE VAL_DIGIT_NINE 0x98
    
#DEFINE VAL_FIRST_DIGIT_POS 0x3E
#DEFINE VAL_SECOND_DIGIT_POS 0x3D
#DEFINE VAL_THIRD_DIGIT_POS 0x3B
#DEFINE VAL_FOURTH_DIGIT_POS 0x37
#DEFINE VAL_FIFTH_DIGIT_POS 0x2F
#DEFINE VAL_SIXTH_DIGIT_POS 0x1F

#DEFINE MEM_DIGIT_START 0x30
#DEFINE MEM_COUNTER MEM_DIGIT_START + 10
#DEFINE MEM_INNER_DELAY MEM_DIGIT_START + 11
#DEFINE VAL_INNER_DELAY 0xFF
#DEFINE MEM_OUTER_DELAY MEM_DIGIT_START + 12
#DEFINE VAL_OUTER_DELAY 0xFF

#define MEM_IF_OP_ONE MEM_DIGIT_START + 13
#define MEM_IF_OP_TWO MEM_DIGIT_START + 14
#define MEM_IF_TRUE_BRANCH MEM_DIGIT_START + 15
#define MEM_IF_FALSE_BRANCH MEM_DIGIT_START + 16
    
#DEFINE MEM_DELAY_LENGTH_MS MEM_DIGIT_START + 13

#INCLUDE<P16F877A.INC>
    __CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC
    ORG 00H    
MAIN: 
    CALL CONFIGURE_MEMORY
    
    BSF STATUS, RP0 ; SELECT BANK 1
    BCF STATUS, RP1

    MOVLW 06H ; SET OUTS TO DIGITAL
    MOVWF ADCON1

    MOVLW B'11000000' ; SET FIRST SIX PORTS TO OUTPUT
    MOVWF TRISA 

    ; SEGMENTS
    MOVLW 00H ; SET ALL PINS TO OUTPUT
    MOVWF TRISD

    BCF STATUS, RP0 ; SELECT BANK
    BCF STATUS, RP1

    MOVLW VAL_FIRST_DIGIT_POS 
    MOVWF PORTA

    GOTO LOOP_ZERO_TO_NINE
    
SET_DIGIT:
    MOVLW MEM_DIGIT_START
    ADDWF MEM_COUNTER, W
    MOVWF FSR
    MOVF INDF, W
    MOVWF PORTD
    RETURN

COUNT:
    DECFSZ MEM_COUNTER, 1
	RETURN
    MOVLW 0x09
    MOVWF MEM_COUNTER
    
DELAY:
    MOVLW VAL_OUTER_DELAY
    MOVWF MEM_OUTER_DELAY
    
OUTER_DELAY_LOOP:
    MOVLW VAL_INNER_DELAY
    MOVWF MEM_INNER_DELAY

INNER_DELAY_LOOP:
    DECFSZ MEM_INNER_DELAY, F
    GOTO INNER_DELAY_LOOP
    
    DECFSZ MEM_OUTER_DELAY, F
    GOTO OUTER_DELAY_LOOP
    RETURN

LOOP_ZERO_TO_NINE:
    CALL SET_DIGIT
    CALL DELAY
    CALL COUNT
    GOTO LOOP_ZERO_TO_NINE
    
IF_EQUALS:
    MOVF MEM_IF_OP_ONE, W
    SUBWF MEM_IF_OP_TWO, W
    BTFSC STATUS, 2   
    RETURN

CONFIGURE_MEMORY:
    BCF STATUS, RP0 ; SELECT BANK 0
    BCF STATUS, RP1
    
    MOVLW 0x09
    MOVWF MEM_COUNTER
    
    ; MOVE ALL DIGITS INTO MEMORY
    MOVLW VAL_DIGIT_ZERO
    MOVWF MEM_DIGIT_START
    
    MOVLW VAL_DIGIT_ONE
    MOVWF MEM_DIGIT_START + 1
    
    MOVLW VAL_DIGIT_TWO
    MOVWF MEM_DIGIT_START + 2
    
    MOVLW VAL_DIGIT_THREE
    MOVWF MEM_DIGIT_START + 3
    
    MOVLW VAL_DIGIT_FOUR
    MOVWF MEM_DIGIT_START + 4
    
    MOVLW VAL_DIGIT_FIVE
    MOVWF MEM_DIGIT_START + 5
    
    MOVLW VAL_DIGIT_SIX
    MOVWF MEM_DIGIT_START + 6
    
    MOVLW VAL_DIGIT_SEVEN
    MOVWF MEM_DIGIT_START + 7
    
    MOVLW VAL_DIGIT_EIGHT
    MOVWF MEM_DIGIT_START + 8
    
    MOVLW VAL_DIGIT_NINE
    MOVWF MEM_DIGIT_START + 9
    
    RETURN
    
LOOP:
    NOP
    GOTO LOOP
    END