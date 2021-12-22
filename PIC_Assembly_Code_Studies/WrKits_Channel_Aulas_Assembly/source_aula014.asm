;
; Curso De Assembly para PIC Aula 014
;
; Desvios condicionais
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
 CBLOCK		H'0C' 			; Inicio da memória de usuário 
	
	REG1 					; Registrador auxiliar de temporização
	REG2 					; Registrador auxiliar de temporização
 
 ENDC						; Fim da memória de usuário

; --- Vetor de RESET ---
	org 	H'0000'
	goto 	start

; --- Vetor de Interrupcao ---
	org 	H'0004'
	retfie

; --- Inicio de codigo ---
start:
	clrf 	REG1
	clrf 	REG2
	
loop:
	movlw 	D'10'			; W = 10
	movwf 	REG1			; REG1 = 10

Aux:
	decfsz 	REG1,F			; Decrementa REG1, é igual a zero?
	goto 	Aux 			; Não! Desvia para Aux
	movwf 	REG2
	btfsc 	REG2,7
	goto 	loop			; Sim! Desvia para loop

	goto 	loop 			; Volta para label loop
	


; --- Desenvolvimento das Sub-Rotinas ---

							
end 						; Final do programa
	

; Instruções Condicionais --------------------

; DECFSZ 	F,D
;
; Decrementa f (d = f - 1) e desvia se f = 0
;
; --------------------------------------------
;
; INCFSZ     F,D
;
; Incrementa f (d = f + 1) e desvia se f = 0
;
; --------------------------------------------
;
; BTFSC     F,B
;
; Testa Bit B de F, Desvia se for Zero
;
; --------------------------------------------
;
; BTFSS    F,B
;
; Testa Bit B de F, Desvia se for 1
;
; --------------------------------------------

; Desvia seria o mesmo que saltar a próxima linha