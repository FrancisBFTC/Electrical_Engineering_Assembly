
_main:

	CLRF       main_i_L0+0
	MOVLW      63
	MOVWF      main_digits_L0+0
	MOVLW      6
	MOVWF      main_digits_L0+1
	MOVLW      91
	MOVWF      main_digits_L0+2
	MOVLW      79
	MOVWF      main_digits_L0+3
	MOVLW      102
	MOVWF      main_digits_L0+4
	MOVLW      109
	MOVWF      main_digits_L0+5
	MOVLW      125
	MOVWF      main_digits_L0+6
	MOVLW      7
	MOVWF      main_digits_L0+7
	MOVLW      127
	MOVWF      main_digits_L0+8
	MOVLW      111
	MOVWF      main_digits_L0+9
	CLRF       TRISB+0
	CLRF       PORTB+0
L_main0:
	CLRF       main_i_L0+0
L_main2:
	MOVF       main_i_L0+0, 0
	SUBLW      9
	BTFSS      STATUS+0, 0
	GOTO       L_main3
	MOVF       main_i_L0+0, 0
	ADDLW      main_digits_L0+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      PORTB+0
	MOVLW      6
	MOVWF      R11+0
	MOVLW      19
	MOVWF      R12+0
	MOVLW      173
	MOVWF      R13+0
L_main5:
	DECFSZ     R13+0, 1
	GOTO       L_main5
	DECFSZ     R12+0, 1
	GOTO       L_main5
	DECFSZ     R11+0, 1
	GOTO       L_main5
	NOP
	NOP
	INCF       main_i_L0+0, 1
	GOTO       L_main2
L_main3:
	GOTO       L_main0
L_end_main:
	GOTO       $+0
; end of _main
