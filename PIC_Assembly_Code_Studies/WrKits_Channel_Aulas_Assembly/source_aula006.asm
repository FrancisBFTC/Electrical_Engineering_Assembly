;
; Curso De Assembly para PIC Aula 006
;
; Acionar um led1 ligado em RB7 apartir de um botao ligado em RB0
;
; Obs.:
;
; LED ligado no modo Current Sourcing:
;
; '0' - Apaga
; '1' - Acende
;
; Botão ligado no modo transistor Pull up:
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

; --- Saida de dados ---
	#define 	led1 	PORTB, RB1 		; Define na PORTB a Porta RB1 para led1

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
	movlw	H'FD' 			; WORK = H'FD' = B'11111101'
	movwf	TRISB 			; Apenas o pino RB7 como saída no TRISB
	bank0 					; Seleciona o Banco 0 de memória

	movlw 	H'FD'			; WORK = B'11111101'
	movwf 	PORTB 			; Define o pino RB1 de PORTB para HIGH
	
loop:
	;Testa bit RB0 do registrador PORTB, Salte a linha abaixo se este Bit está limpo (For 0)
	btfsc 	botao1			; Botão foi pressionado?
	goto 	apaga_led1		; Não! Então desvia para apaga_led1
	bsf 	led1 			; Sim! Liga led 1
	goto 	loop			; Volta para a label loop

apaga_led1:
	bcf 	led1 			; Apaga led 1
	goto 	loop 			; Volta para label loop

end 						; Final do programa
	