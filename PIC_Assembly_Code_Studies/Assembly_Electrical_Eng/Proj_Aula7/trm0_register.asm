; Aula de TMR0
;
;

list p = 16f628A
 
 #include <p16f628a.inc>

__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF

#define banco1 bsf STATUS,RP0
#define banco0 bcf STATUS,RP0

#define BOT    PORTB,RB0

; Registros de propósitos gerais
cblock 		H'20'
	W_TEMP			;0x20
	STATUS_TEMP		;0x21
	REG1	
	CONTA
	CONTB
	aux1
	aux2
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

	btfss 		INTCON,T0IF		; Interrupção por TIMER0?
	goto 		sair			; Não, Saia!
	bcf 		INTCON,T0IF		; Sim, Limpe a flag!

	movlw 		D'6'  		; 256 - 6 = 250 Contagens, cada contagem com 4 ciclos
	movwf 		TMR0

	decfsz 		CONTA,F
	goto 		sair	; Não
	movlw 		D'50'	; Sim
	movwf 		CONTA						
	
	decfsz 		CONTB,F
	goto 		sair	; Não
	movlw 		D'10'	; Sim
	movwf 		CONTB

	movlw 		B'00000010'		; Inversão do RB1
	xorwf 		PORTB,F

; -----------------------------------
sair:
	swapf 	STATUS_TEMP,W  ; P. EX.: STATUS_TEMP = 0001 0000 --> W
	movwf 	STATUS
	swapf 	W_TEMP,F       ; P. EX.: W_TEMP = 1011 0010
	swapf 	W_TEMP,W       ; W = 0010 1011
						   ; Substitui a instrução movf W_TEMP, que modifica o BIT Z do STATUS
	
	retfie


; Fim da Interrupção


inicio:
	banco1
	movlw 	H'81'       ; Desabilita pull-ups, transição de subida, prescaler em 4
	movwf 	OPTION_REG 
	movlw 	H'00'
	movwf 	TRISB
	banco0

	movlw 	H'A0' 		; Habilitamos as interrupções por Overflow do TMR0
	movwf 	INTCON
	
	movlw 	D'50'
	movwf 	CONTA
	movlw 	D'10'
	movwf 	CONTB
	
	bcf 	PORTB,RB1

	movlw 	D'6'  		; 256 - 6 = 250 Contagens, cada contagem com 4 ciclos
	movwf 	TMR0
	
loop:
	movlw 	B'00000001'		; Inversão do RB0
	xorwf 	PORTB,F
	call 	Retardo500ms
	goto 	loop
	
Retardo500ms:
	movlw 	D'250'
	movwf 	aux1
dec1:	
	movlw 	D'200'
	movwf 	aux2
dec2:
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	decfsz 	aux2
	goto 	dec2
	decfsz 	aux1
	goto 	dec1
	return

end