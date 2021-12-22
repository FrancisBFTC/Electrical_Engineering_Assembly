 list p = 16f628A
 
 #include <p16f628a.inc>

__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF

#define banco1 bsf STATUS,RP0
#define banco0 bcf STATUS,RP0
#define led_d1 	PORTB,RB1
#define led_d2 	PORTA,RA4

; Registros de propósitos gerais
cblock 		H'20'
	W_TEMP			;0x20
	STATUS_TEMP		;0x21

endc

; Vetor de RESET
		org 	0000h
		goto 	inicio

; Vetor de INTERRUPÇÃO
		org 	0004h

; Salvar Contexto (STATUS,W)
; no banco 0
	movwf 	W_TEMP    ; P. EX.: W = 0010 1011 então W_TEMP = 0010 1011
	swapf 	STATUS,W  ; P. EX.: STATUS = 0001 0000 -> swap -> STATUS = 0000 0001
					  ; Não altera nenhum BIT do STATUS (movf & movwf altera)
	banco0
	movwf 	STATUS_TEMP  ; P. EX.: STATUS_TEMP = 0000 0001
; ---- ISR -------------------------

; tratamento da interrupção

	btfss 	INTCON,INTF  ; verifique se o BIT INTF do Registro INTCON é igual a 1
	goto 	sair
	bcf 	INTCON,INTF  ; Limpe o BIT da interrupção

	;comf  	PORTA,F
	;comf 	PORTB,F
	xorlw 	B'00001000'
	xorwf 	PORTA,F
	xorlw 	B'00000010'
	xorwf 	PORTB,F
	
sair:
; -----------------------------------
	swapf 	STATUS_TEMP,W  ; P. EX.: STATUS_TEMP = 0001 0000 --> W
	movwf 	STATUS
	swapf 	W_TEMP,F       ; P. EX.: W_TEMP = 1011 0010
	swapf 	W_TEMP,W       ; W = 0010 1011
						   ; Substitui a instrução movf W_TEMP, que modifica o BIT Z do STATUS
	
	retfie




; Fim da Interrupção

inicio:
	banco1
	movlw 	B'11000000'   ; OPTION_REG = 0xC0
	movwf 	OPTION_REG    ; Desabilitou os pull-ups/Interrupção por transição de subida (rising)
	movlw 	B'00000000'   ; TRISA = 0x00
	movwf 	TRISA		  ; Todos como saída
	movlw 	H'01'		  ; TRISB = 0x01
	movwf 	TRISB		  ; Apenas RB0/INT como entrada
	banco0
	
	movlw 	B'10010000'   ; INTCON = 0x90; Global Int e Extern Int
	movwf 	INTCON		  ; Habilita as insterrupções externas
	;movlw 	H'07'
	;movwf 	CMCON		; Desabilita os comparadores de PORTA

	bsf 	led_d2		; Ligar LED D2
	bsf 	led_d1 		; Ligar LED D1
loop:

	movlw 	D'50'
	goto 	loop
	
end