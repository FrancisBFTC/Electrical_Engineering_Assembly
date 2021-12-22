 list p = 16f628A
 
 #include <p16f628a.inc>

__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF

#define banco1 bsf STATUS,RP0
#define banco0 bcf STATUS,RP0
#define led1 	PORTA,RA2
#define led2 	PORTA,RA3
#define bot 	PORTB,RB0

		org 00h
		goto inicio

inicio:
	banco1
	movlw 	H'00'
	movwf 	OPTION_REG    ; Habilita os pull-ups
	movlw 	H'00'
	movwf 	TRISB
	movlw 	H'01'
	movwf 	TRISB
	banco0
	
	movlw 	H'07'
	movwf 	CMCON		; Desabilita os comparadores de PORTA

	bcf 	led1
	bcf 	led2
	

loop:
	btfss 	bot
	goto 	desligar
	bsf 	led1
	bcf 	led2
	goto 	loop
desligar:
	bcf 	led1
	bsf 	led2
	goto 	loop
	
	end