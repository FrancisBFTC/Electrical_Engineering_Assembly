_config:
	BANK_SELECT 1
	
	movlw 	H'FF'
	movwf 	TRISD
	clrf 	TRISB

	BANK_SELECT 0

	clrf 	PORTA
	clrf 	PORTB
	clrf 	PORTC
	clrf 	PORTD
	clrf 	PORTE