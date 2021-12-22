;
; Curso De Assembly para PIC Aula 002
;
; MCU (MicroController Unit): PIC16F84A   Clock: 4MHz
;
; Author: Eng. Warner Rambo 	Data: 18/10/2021
;

	list p=16F84A 			; Microcontrolador Utilizado PIC16F84A

; --- Arquivos inclu�dos no projeto ---
	#include <p16f84a.inc>  ; Inclui o arquivo do PIC16F84A


; --- Fuse Bits ---
; Cristal oscilador externo de 4MHz, sem watchdog timer, com power up timer e sem prote��o de c�digo
	__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF

	