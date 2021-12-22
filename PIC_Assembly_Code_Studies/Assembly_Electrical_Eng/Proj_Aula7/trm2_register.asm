; Aula de TMR0
;
;

list p = 16f628A
 
 #include <p16f628a.inc>

__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF

#define banco1 bsf STATUS,RP0
#define banco0 bcf STATUS,RP0

#define BOT    PORTB,RB0

; Registros de propÃ³sitos gerais
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

; Vetor de INTERRUPÃ‡ÃƒO
		org 	0004h

; Salvar Contexto (STATUS,W)
; no banco 0
	movwf 	W_TEMP    ; P. EX.: W = 0010 1011 entÃ£o W_TEMP = 0010 1011
	swapf 	STATUS,W  ; P. EX.: STATUS = 0001 0000 -> swap -> STATUS = 0000 0001
					  ; NÃ£o altera nenhum BIT do STATUS (movf & movwf altera)
	banco0
	movwf 	STATUS_TEMP  ; P. EX.: STATUS_TEMP = 0000 0001
; ---- ISR -------------------------
	
	btfss 	PIR1,TMR2IF
	goto 	sair
	bcf 	PIR1,TMR2IF
	
	movlw 	B'00000010'
	xorwf 	PORTB,F

; -----------------------------------
sair:
	swapf 	STATUS_TEMP,W  ; P. EX.: STATUS_TEMP = 0001 0000 --> W
	movwf 	STATUS
	swapf 	W_TEMP,F       ; P. EX.: W_TEMP = 1011 0010
	swapf 	W_TEMP,W       ; W = 0010 1011
						   ; Substitui a instruÃ§Ã£o movf W_TEMP, que modifica o BIT Z do STATUS
	
	retfie


; Fim da Interrupção

; PostScaler -> A cada N overflows, habilita interrupção
; PreScaler -> A cada N contagens, habilita interrupção
; T2CON -> Registrador que configura PostScaler BIT<6:3> e PreScaler BIT<1:0>
; Timer On BIT<2>
; Registros para configurar:
; 	PR2, PIE1, PIR1, INTCON, T2CON, TMR2
inicio:
	banco1
	movlw 	D'20'
	movwf 	PR2 		; Número limite final para TMR2
	movlw 	H'02'
	movwf 	PIE1		; Habilita Interrupção por Match entre TRM2 e PR2
	movlw 	H'00'
	movwf 	TRISB
	banco0

	bsf 	INTCON,GIE		; Habilita Interrupção Global
	bsf 	INTCON,PEIE 	; Habilita Interrupção periférica
	
	movlw 	B'00001101'
	movwf 	T2CON			; PostScaler = 1:2; PreScaler = 1:4;
	movlw 	H'00'
	movwf 	PIR1

	bcf 	PORTB,RB1
	
loop:
	movlw 	B'00000001'		; InversÃ£o do RB0
	xorwf 	PORTB,F
	goto 	loop
	

end