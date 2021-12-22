
_main:

	CLRF       TRISA+0
	CLRF       PORTA+0
L_main0:
	MOVLW      13
	SUBWF      PORTA+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_main1
	INCF       PORTA+0, 0
	MOVWF      R0+0
	MOVF       R0+0, 0
	MOVWF      PORTA+0
	CALL       _mydelay+0
	GOTO       L_main0
L_main1:
	CALL       _mydelay+0
	CLRF       PORTA+0
	BSF        RA4_bit+0, BitPos(RA4_bit+0)
L_end_main:
	GOTO       $+0
; end of _main

_mydelay:

	MOVLW      200
	MOVWF      _time1+0
Aux1:
	MOVLW      250
	MOVWF      _time2+0
Aux2:
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	DECFSZ     _time2+0, 1
	GOTO       Aux2
	DECFSZ     _time1+0, 1
	GOTO       Aux1
L_end_mydelay:
	RETURN
; end of _mydelay
