;
; Curso De Assembly para PIC Aula 006
;
; Acionar o led1 ligado em RB1 apartir de um botao1 ligado em RB0
; Acionar o led2 ligado em RB3 apartir de um botao2 ligado em RB2
;
; Obs.:
;
; LED ligado no modo Current Sourcing:
;
; '0' - Apaga
; '1' - Acende
;
; Botao ligado no modo transistor Pull up:
;
; '0' - pressionado
; '1' - não-pressionado
;
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

; --- Entrada de dados ---
	#define 	botao1	PORTB, RB0      ; Define na PORTB a Porta RB0 para botao1
	#define 	botao2 	PORTB, RB2 		; Define na PORTB a Porta RB2 para botao2

; --- Saida de dados ---
	#define 	led1 	PORTB, RB1 		; Define na PORTB a Porta RB1 para led1
	#define 	led2 	PORTB, RB3 		; Define na PORTB a Porta RB3 para led2	

; --- Fuse Bits ---
; Cristal oscilador externo de 4MHz, sem watchdog timer, com power up timer e sem protecao de codigo
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF


; --- Paginacao de memoria ---
	#define 	bank0 	bcf STATUS, RP0 	; Mnemonico que limpa o bit RP0 do Registrador STATUS (Selecionar Banco 0 - Instrucao Bit Clear File)
	#define 	bank1 	bsf STATUS, RP0 	; Mnemonico que seta o bit RP0 do Registrador STATUS (Selecionar Banco 1 - Instrucao Bit Set File)

; --- Vetor de RESET ---
	org 	H'0000'
	goto 	start

; --- Vetor de Interrupcao ---
	org 	H'0004'
	retfie

; --- Inicio de codigo ---
start:
	bank1					; Seleciona o Banco 1 de memória
	movlw 	H'FF' 			; WORK = B'11111111'
	movwf 	TRISA			; Todos os pinos como entrada no TRISA
	movlw	H'F5' 			; WORK = H'F5' = B'11110101'
	movwf	TRISB 			; Apenas o pino RB1 e RB3 como saída no TRISB
	bank0 					; Seleciona o Banco 0 de memória

	movlw 	H'F5'			; WORK = B'11110101'
	movwf 	PORTB 			; Limpa os pinos RB1 e RB3 de PORTB para Desligados (HIGH)
	
loop:
	call 	trata_but1
	call 	trata_but2

	goto 	loop 			; Volta para label loop
	

; --- Desenvolvimento das Sub-Rotinas

;Testa bit RB0 do registrador PORTB, Salte a linha abaixo se este Bit está limpo (For 0)
trata_but1:					; Sub-Rotina para tratar o botao 1
	btfsc 	botao1			; Botão foi pressionado?
	goto 	apaga_led1		; Não! Então desvia para apaga_led1
	bsf 	led1 			; Sim! Liga led 1
	return					; retorna da sub-rotina

apaga_led1:
	bcf 	led1 			; Apaga led 1
	return 					; retorna da sub-rotina


;Testa bit RB1 do registrador PORTB, Salte a linha abaixo se este Bit está limpo (For 0)
trata_but2:					; Sub-Rotina para tratar o botao 2
	btfsc 	botao2			; Botão foi pressionado?
	goto 	apaga_led2		; Não! Então desvia para apaga_led2
	bsf 	led2 			; Sim! Liga led 2
	return					; retorna da sub-rotina

apaga_led2:
	bcf 	led2 			; Apaga led 2
	return 					; retorna da sub-rotina

end 						; Final do programa
	