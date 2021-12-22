;
; Curso De Assembly para PIC Aula 005
;
; Acionar um led ligado em RB7 apartir de um botao ligado em RB0
;
; Obs.:
;
; LED ligado no modo Current Sourcing:
;
; '0' - Apaga
; '1' - Acende
;
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
	#define 	led1 	PORTB, RB7 		; Define na PORTB a Porta RB7 para led1

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
	movlw	H'7F' 			; WORK = H'7F' = B'01111111'
	movwf	TRISB 			; Apenas o pino RB7 como saída no TRISB
	bank0 					; Seleciona o Banco 0 de memória

	movlw 	H'7F'			; WORK = B'01111111'
	movwf 	PORTB 			; Define o pino RB7 de PORTB para HIGH

	goto 	start				; Trava aqui no loop infinito

end 						; Final do programa
	