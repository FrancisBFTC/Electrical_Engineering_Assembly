
_Serial_Send:

	MOVF        FARG_Serial_Send_val+0, 0 
	MOVWF       TXREG+0 
	CALL        _Serial_Wait+0, 0
L_end_Serial_Send:
	RETURN      0
; end of _Serial_Send

_Serial_Write:

	CLRF        Serial_Write_i_L0+0 
	CLRF        Serial_Write_i_L0+1 
L_Serial_Write0:
	MOVF        FARG_Serial_Write_string+0, 0 
	MOVWF       FARG_strlen_s+0 
	MOVF        FARG_Serial_Write_string+1, 0 
	MOVWF       FARG_strlen_s+1 
	CALL        _strlen+0, 0
	MOVLW       128
	XORWF       Serial_Write_i_L0+1, 0 
	MOVWF       R2 
	MOVLW       128
	XORWF       R1, 0 
	SUBWF       R2, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__Serial_Write20
	MOVF        R0, 0 
	SUBWF       Serial_Write_i_L0+0, 0 
L__Serial_Write20:
	BTFSC       STATUS+0, 0 
	GOTO        L_Serial_Write1
	MOVF        Serial_Write_i_L0+0, 0 
	ADDWF       FARG_Serial_Write_string+0, 0 
	MOVWF       FSR0 
	MOVF        Serial_Write_i_L0+1, 0 
	ADDWFC      FARG_Serial_Write_string+1, 0 
	MOVWF       FSR0H 
	MOVF        POSTINC0+0, 0 
	MOVWF       TXREG+0 
	CALL        _Serial_Wait+0, 0
	INFSNZ      Serial_Write_i_L0+0, 1 
	INCF        Serial_Write_i_L0+1, 1 
	GOTO        L_Serial_Write0
L_Serial_Write1:
L_end_Serial_Write:
	RETURN      0
; end of _Serial_Write

_Serial_Wait:

L_Serial_Wait3:
	BTFSC       TRMT_bit+0, BitPos(TRMT_bit+0) 
	GOTO        L_Serial_Wait4
	GOTO        L_Serial_Wait3
L_Serial_Wait4:
L_end_Serial_Wait:
	RETURN      0
; end of _Serial_Wait

_interrupt:

	BTFSS       TMR0IF_bit+0, BitPos(TMR0IF_bit+0) 
	GOTO        L_interrupt5
	BCF         TMR0IF_bit+0, BitPos(TMR0IF_bit+0) 
	INCF        _Counter+0, 1 
	MOVF        _Counter+0, 0 
	XORLW       153
	BTFSS       STATUS+0, 2 
	GOTO        L_interrupt6
	CLRF        FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _LM35_Value+0 
	BSF         RD0_bit+0, BitPos(RD0_bit+0) 
	BCF         RD0_bit+0, BitPos(RD0_bit+0) 
	CLRF        _Counter+0 
	MOVLW       2
	MOVWF       TMR0L+0 
	MOVLW       128
	ANDWF       R0, 0 
	MOVWF       R3 
	MOVLW       7
	MOVWF       R2 
	MOVF        R3, 0 
	MOVWF       R17 
	MOVF        R2, 0 
L__interrupt24:
	BZ          L__interrupt25
	RRCF        R17, 1 
	BCF         R17, 7 
	ADDLW       255
	GOTO        L__interrupt24
L__interrupt25:
	MOVLW       64
	ANDWF       R0, 0 
	MOVWF       R3 
	MOVLW       6
	MOVWF       R2 
	MOVF        R3, 0 
	MOVWF       R16 
	MOVF        R2, 0 
L__interrupt26:
	BZ          L__interrupt27
	RRCF        R16, 1 
	BCF         R16, 7 
	ADDLW       255
	GOTO        L__interrupt26
L__interrupt27:
	MOVF        R16, 0 
	XORWF       R17, 0 
	MOVWF       R4 
	MOVLW       16
	ANDWF       R0, 0 
	MOVWF       R3 
	MOVF        R3, 0 
	MOVWF       R15 
	RRCF        R15, 1 
	BCF         R15, 7 
	RRCF        R15, 1 
	BCF         R15, 7 
	RRCF        R15, 1 
	BCF         R15, 7 
	RRCF        R15, 1 
	BCF         R15, 7 
	MOVF        R15, 0 
	XORWF       R4, 0 
	MOVWF       R14 
	MOVF        R14, 0 
	MOVWF       __a+0 
	MOVLW       8
	ANDWF       R0, 0 
	MOVWF       R3 
	MOVF        R3, 0 
	MOVWF       R13 
	RRCF        R13, 1 
	BCF         R13, 7 
	RRCF        R13, 1 
	BCF         R13, 7 
	RRCF        R13, 1 
	BCF         R13, 7 
	MOVLW       4
	ANDWF       R0, 0 
	MOVWF       R3 
	MOVF        R3, 0 
	MOVWF       R12 
	RRCF        R12, 1 
	BCF         R12, 7 
	RRCF        R12, 1 
	BCF         R12, 7 
	MOVF        R12, 0 
	XORWF       R13, 0 
	MOVWF       R2 
	MOVLW       1
	ANDWF       R0, 0 
	MOVWF       R11 
	MOVF        R11, 0 
	XORWF       R2, 0 
	MOVWF       R10 
	MOVF        R10, 0 
	MOVWF       __d+0 
	MOVLW       32
	ANDWF       R0, 0 
	MOVWF       R3 
	MOVLW       5
	MOVWF       R2 
	MOVF        R3, 0 
	MOVWF       R9 
	MOVF        R2, 0 
L__interrupt28:
	BZ          L__interrupt29
	RRCF        R9, 1 
	BCF         R9, 7 
	ADDLW       255
	GOTO        L__interrupt28
L__interrupt29:
	MOVF        R9, 0 
	XORWF       R17, 0 
	MOVWF       R2 
	MOVF        R15, 0 
	XORWF       R2, 0 
	MOVWF       R8 
	MOVF        R8, 0 
	MOVWF       __b+0 
	MOVLW       2
	ANDWF       R0, 0 
	MOVWF       R1 
	MOVF        R1, 0 
	MOVWF       R7 
	RRCF        R7, 1 
	BCF         R7, 7 
	MOVF        R7, 0 
	XORWF       R13, 0 
	MOVWF       R0 
	MOVF        R11, 0 
	XORWF       R0, 0 
	MOVWF       R6 
	MOVF        R6, 0 
	MOVWF       __e+0 
	MOVF        R9, 0 
	XORWF       R16, 0 
	MOVWF       R0 
	MOVF        R15, 0 
	XORWF       R0, 0 
	MOVWF       R5 
	MOVF        R5, 0 
	MOVWF       __c+0 
	MOVF        R7, 0 
	XORWF       R12, 0 
	MOVWF       R0 
	MOVF        R11, 0 
	XORWF       R0, 0 
	MOVWF       R4 
	MOVF        R4, 0 
	MOVWF       __f+0 
	MOVLW       6
	MOVWF       R0 
	MOVF        R17, 0 
	MOVWF       R3 
	MOVF        R0, 0 
L__interrupt30:
	BZ          L__interrupt31
	RLCF        R3, 1 
	BCF         R3, 0 
	ADDLW       255
	GOTO        L__interrupt30
L__interrupt31:
	MOVLW       5
	MOVWF       R2 
	MOVF        R16, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R2, 0 
L__interrupt32:
	BZ          L__interrupt33
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	ADDLW       255
	GOTO        L__interrupt32
L__interrupt33:
	MOVF        R0, 0 
	IORWF       R3, 1 
	MOVLW       4
	MOVWF       R2 
	MOVF        R9, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R2, 0 
L__interrupt34:
	BZ          L__interrupt35
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	ADDLW       255
	GOTO        L__interrupt34
L__interrupt35:
	MOVF        R0, 0 
	IORWF       R3, 1 
	MOVLW       3
	MOVWF       R2 
	MOVF        R15, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R2, 0 
L__interrupt36:
	BZ          L__interrupt37
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	ADDLW       255
	GOTO        L__interrupt36
L__interrupt37:
	MOVF        R0, 0 
	IORWF       R3, 1 
	MOVF        R14, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVF        R0, 0 
	IORWF       R3, 1 
	MOVF        R8, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVF        R0, 0 
	IORWF       R3, 0 
	MOVWF       R2 
	MOVF        R5, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R0, 0 
	IORWF       R2, 0 
	MOVWF       R3 
	MOVF        R3, 0 
	MOVWF       _A_+0 
	MOVLW       6
	MOVWF       R0 
	MOVF        R13, 0 
	MOVWF       _B_+0 
	MOVF        R0, 0 
L__interrupt38:
	BZ          L__interrupt39
	RLCF        _B_+0, 1 
	BCF         _B_+0, 0 
	ADDLW       255
	GOTO        L__interrupt38
L__interrupt39:
	MOVLW       5
	MOVWF       R2 
	MOVF        R12, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R2, 0 
L__interrupt40:
	BZ          L__interrupt41
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	ADDLW       255
	GOTO        L__interrupt40
L__interrupt41:
	MOVF        R0, 0 
	IORWF       _B_+0, 1 
	MOVLW       4
	MOVWF       R2 
	MOVF        R7, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R2, 0 
L__interrupt42:
	BZ          L__interrupt43
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	ADDLW       255
	GOTO        L__interrupt42
L__interrupt43:
	MOVF        R0, 0 
	IORWF       _B_+0, 1 
	MOVLW       3
	MOVWF       R2 
	MOVF        R11, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R2, 0 
L__interrupt44:
	BZ          L__interrupt45
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	ADDLW       255
	GOTO        L__interrupt44
L__interrupt45:
	MOVF        R0, 0 
	IORWF       _B_+0, 1 
	MOVF        R10, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVF        R0, 0 
	IORWF       _B_+0, 1 
	MOVF        R6, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	RLCF        R0, 1 
	BCF         R0, 0 
	RLCF        R1, 1 
	MOVF        R0, 0 
	IORWF       _B_+0, 1 
	MOVF        R4, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R0, 0 
	IORWF       _B_+0, 1 
	MOVF        R3, 0 
	MOVWF       FARG_Serial_Send_val+0 
	CALL        _Serial_Send+0, 0
L_interrupt7:
	BTFSC       RD1_bit+0, BitPos(RD1_bit+0) 
	GOTO        L_interrupt8
	GOTO        L_interrupt7
L_interrupt8:
	MOVF        _B_+0, 0 
	MOVWF       FARG_Serial_Send_val+0 
	CALL        _Serial_Send+0, 0
L_interrupt9:
	BTFSC       RD2_bit+0, BitPos(RD2_bit+0) 
	GOTO        L_interrupt10
	GOTO        L_interrupt9
L_interrupt10:
L_interrupt6:
	GOTO        L_interrupt11
L_interrupt5:
	BTFSS       INT0IF_bit+0, BitPos(INT0IF_bit+0) 
	GOTO        L_interrupt12
	BCF         INT0IF_bit+0, BitPos(INT0IF_bit+0) 
	BTFSC       RD1_bit+0, BitPos(RD1_bit+0) 
	GOTO        L_interrupt13
	MOVF        _A_+0, 0 
	MOVWF       FARG_Serial_Send_val+0 
	CALL        _Serial_Send+0, 0
	GOTO        L_interrupt14
L_interrupt13:
	MOVF        _B_+0, 0 
	MOVWF       FARG_Serial_Send_val+0 
	CALL        _Serial_Send+0, 0
L_interrupt14:
L_interrupt12:
L_interrupt11:
L_end_interrupt:
L__interrupt23:
	RETFIE      1
; end of _interrupt

_main:

	MOVLW       36
	MOVWF       TXSTA+0 
	MOVLW       128
	MOVWF       RCSTA+0 
	MOVLW       25
	MOVWF       SPBRG+0 
	CLRF        BAUDCON+0 
	MOVLW       240
	MOVWF       INTCON+0 
	MOVLW       196
	MOVWF       INTCON2+0 
	MOVLW       199
	MOVWF       T0CON+0 
	MOVLW       1
	MOVWF       TRISB+0 
	MOVLW       192
	MOVWF       TRISC+0 
	MOVLW       255
	MOVWF       TRISA+0 
	MOVLW       6
	MOVWF       TRISD+0 
	CLRF        PORTA+0 
	CLRF        PORTB+0 
	CLRF        PORTC+0 
	CLRF        PORTD+0 
	MOVLW       1
	MOVWF       ADCON0+0 
	MOVLW       14
	MOVWF       ADCON1+0 
	MOVLW       130
	MOVWF       R12, 0
	MOVLW       221
	MOVWF       R13, 0
L_main15:
	DECFSZ      R13, 1, 1
	BRA         L_main15
	DECFSZ      R12, 1, 1
	BRA         L_main15
	NOP
	NOP
	MOVLW       152
	MOVWF       _Counter+0 
	MOVLW       255
	MOVWF       TMR0L+0 
L_main16:
	GOTO        L_main16
L_end_main:
	GOTO        $+0
; end of _main
