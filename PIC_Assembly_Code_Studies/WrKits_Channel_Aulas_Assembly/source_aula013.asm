;
; Curso De Assembly para PIC Aula 013
;
; Instru��es L�gicas
;
; MCU (MicroController Unit): PIC16F84A   Clock: 4MHz
;
; www.wrkits.com.br 	| 	www.facebook.com/wrkits 	| 	www.youtube.com/canalwrkits
;
; Author: Eng. Warner Rambo 	Data: 18/10/2021
;

	list p=16F84A 			; Microcontrolador Utilizado PIC16F84A

; --- Arquivos inclusoes no projeto ---
	#include <p16f84a.inc>  ; Inclui o arquivo do PIC16F84A

; --- Fuse Bits ---
; Cristal oscilador externo de 4MHz, sem watchdog timer, com power up timer e sem protecao de codigo
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF


; --- Paginacao de memoria ---
	#define 	bank0 	bcf STATUS, RP0 	; Mnemonico que limpa o bit RP0 do Registrador STATUS (Selecionar Banco 0 - Instrucao Bit Clear File)
	#define 	bank1 	bsf STATUS, RP0 	; Mnemonico que seta o bit RP0 do Registrador STATUS (Selecionar Banco 1 - Instrucao Bit Set File)

; --- Registradores de Uso Geral ---
 CBLOCK		H'0C' 			; Inicio da mem�ria de usu�rio 
	
	REG1 					; Registrador auxiliar de temporiza��o
	REG2 					; Registrador auxiliar de temporiza��o
 
 ENDC						; Fim da mem�ria de usu�rio

; --- Vetor de RESET ---
	org 	H'0000'
	goto 	start

; --- Vetor de Interrupcao ---
	org 	H'0004'
	retfie

; --- Inicio de codigo ---
start:
	bank1					; Seleciona o Banco 1 de mem�ria
	movlw 	H'FF' 			; WORK = B'11111111'
	movwf 	TRISA			; Todos os pinos como entrada no TRISA
	movlw	H'F5' 			; WORK = H'F5' = B'11110101'
	movwf	TRISB 			; Apenas o pino RB1 e RB3 como sa�da no TRISB
	bank0 					; Seleciona o Banco 0 de mem�ria

	clrf 	REG1
	clrf 	REG2
	
loop:
	movlw 	B'11110000'		; W = 'F0'
	andlw 	B'10100001'		; W = W and B'10100001'
	
	xorwf 	REG1,F			; REG1 = W xor REG1
	comf 	REG1,F
	
	goto 	loop 			; Volta para label loop
	


; --- Desenvolvimento das Sub-Rotinas ---

							
end 						; Final do programa
	

; Instru��es L�gicas ------------------------

; Opera��o L�gica AND
;
; ANDLW		K    
;
; W = W and K
; Afeta a Flag Z do STATUS
; --------------------------------------------
;
; ANDWF     F,D
;
; D = W and F
; D = 0 (W) ou D = 1 (F)
; Afeta a Flag Z do STATUS
; --------------------------------------------
; Complemento (Opera��o de nega��o)
;
; COMF		F,D
;
; D = not F
; D = 0 (W) ou D = 1 (F)
; Afeta a Flag Z do STATUS
; --------------------------------------------
; Opera��o L�gica OR
;
; IORLW 	K
;
; W = W or K
; Afeta a Flag Z do STATUS
; --------------------------------------------
; Opera��o L�gica OR
;
; IORWF 	F,D
;
; D = W or F
; D = 0 (W) ou D = 1 (F)
; Afeta a Flag Z do STATUS
; --------------------------------------------
; Opera��o L�gica XOR
;
; XORLW 	K
;
; W = W xor K
; Afeta a Flag Z do STATUS
; --------------------------------------------
; Opera��o L�gica XOR
;
; XORWF 	F,D
;
; D = W xor F
; D = 0 (W) ou D = 1 (F)
; Afeta a Flag Z do STATUS
; --------------------------------------------