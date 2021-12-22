;
; Curso De Assembly para PIC Aula 011
;
; Acionar o led1 ligado em RB1 apartir de um botao1 ligado em RB0
; Acionar o led2 ligado em RB3 apartir de um botao2 ligado em RB2
;
; Obs.:
;
; Aciona LED1 ligado em RB1 e apaga LED2 ligado em RB3
; Aguarda 500 milisegundos
; Aciona LED2 ligado em RB3 e paga LED1 ligado em RB1
; Aguarda 500 milisegundos
;
;
; Cálculo do Ciclo de máquina
;
; Ciclo de Máquina = 1 / (Freq_Cristal / 4) = 1us (MicroSegundos)
; (4MHz / 4 Ciclos) = 1000000 Hz (PIC_Speed = 1MHz) 
; 1 / 1000000Hz = 0,000001s = 1 MicroSegundos
; 1 Ciclo = 1 MicroSegundos
; Cada instrução gasta 1 Ciclo, Instruções de salto gasta 2 ciclos
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

; --- Registradores de Uso Geral ---
 CBLOCK		H'0C' 			; Inicio da memória de usuário 
	
	TIME1 					; Registrador auxiliar de temporização
	TIME2 					; Registrador auxiliar de temporização
 
 ENDC						; Fim da memória de usuário

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
	bsf 	led1		 	; Liga LED1
	bcf 	led2			; Desliga LED2
	call 	Delay500ms		; Espera 500 milisegundos
	bcf 	led1			; Desliga LED1
	bsf 	led2			; Liga LED2
	call 	Delay500ms		; Espera 500 milisegundos

	goto 	loop 			; Volta para label loop
	


; --- Desenvolvimento das Sub-Rotinas ---

; Rotina de Delay de 500 milisegundos
Delay500ms:
	movlw 	D'200' 			; Move o valor 200 para W
	movwf 	TIME1			; Inicializa variável tempo1 (Endereço relativo)
Aux1:						; Já gastou 4 ciclos de máquina
	movlw 	D'250'			; Move o valor 250 para W
	movwf 	TIME2			; Inicializa variávem tempo2 (Endereço relativo)

Aux2:
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	decfsz 	TIME2			; Decrementa tempo2 até que seja igual a 0
	goto 	Aux2			; Vai para a label Aux2
							; GOTO gasta 2 ciclos de máquinas, logo foram 10 ciclos
							; 250 x 10 ciclos de máquina = 2500 ciclos
	decfsz 	TIME1			; Decrementa tempo1 até que seja igual a 0
	goto 	Aux1			; Vai para a label Aux1
							; Instruções acima gastou 3 ciclos de máquina
							; 2500 x 200 = 500000 ciclos de máquina
	return 					; Retorna para a chamada
							
end 						; Final do programa
	