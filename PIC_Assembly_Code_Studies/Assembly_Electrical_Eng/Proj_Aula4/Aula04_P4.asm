 list p = 16f628A
 
 #include <p16f628a.inc>

__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF

#define banco1 bsf STATUS,RP0
#define banco0 bcf STATUS,RP0

cblock 	H'20'   ; Endereço inicial do PIC
	Am
	Bm
	C0
endc

		org 00h
		goto inicio

inicio:

	movlw 	D'6'
	movwf	Am
	movlw 	D'8'
	movwf 	Bm
	movlw 	D'0'
	movfw 	C0
	call 	Multiply

	goto 	$

Multiply:
	movf 	Am, W
	movwf 	C0 			; Am manda pra C0
DecB:
	decf 	Bm, F		; D = 0 -> WORK; D = 1 -> FILE
	btfsc 	STATUS,Z
	return
	movf 	Am, W
	addwf 	C0, F
	goto 	DecB	


	end