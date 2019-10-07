#DEFINE VAL 07H
#INCLUDE<P16F877A.INC>
    __CONFIG _DEBUG_OFF&_CP_ALL&_WRT_HALF&_CPD_ON&_LVP_OFF&_BODEN_OFF&_PWRTE_ON&_WDT_OFF&_HS_OSC
    ORG 00H ; set the location to write the program to
    BSF STATUS, RP0 ; set the bank to bank 1
    MOVLW 0FEH ; move the literal 0FEH to the W register (OFEH is the I/O status of the pins, 0 is output 1 is input therfore only bit zero is output)
    MOVWF TRISC ; move the W register to the TRISC memory address to set the I/O of the pins
    BCF STATUS, RP0 ; clear the bank bit so that we are back in bank 0
    BSF PORTC, 0 ; set bit zero of the port to high
    ; set counters to full
    MOVLW VAL
    MOVWF 0CH
    MOVLW VAL
    MOVWF 0DH
    MOVLW VAL
    MOVWF 0EH
    GOTO LOOP

RST_ZERO
    MOVLW VAL
    MOVWF 0CH
    GOTO LOOP
    
RST_ONE
    MOVLW VAL
    MOVWF 0DH
    GOTO LOOP
    
LOOP_ONE
    DECFSZ 0DH
	CALL RST_ZERO
    CALL LOOP_TWO

LOOP_TWO
    DECFSZ 0DH
	CALL RST_ONE
    BCF PORTC, 0
    
LOOP
    DECFSZ 0CH
	CALL LOOP_ONE
    END
    
    
