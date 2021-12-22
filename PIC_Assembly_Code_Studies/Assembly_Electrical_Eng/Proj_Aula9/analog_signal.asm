list p=12F675

#include <p12F675.inc>

#define		banco0 	bcf STATUS,RP0
#define 	banco1  bsf STATUS,RP0

#define 	signal 	GPIO,GP0 	; Entrada GP0 do sinal anal�gico
#define 	led 	GPIO,GP5	; Sa�da GP5 do led e para oscilosc�pio

__config _INTRC_OSC_NOCLKOUT & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _CPD_OFF & _CP_ON & _MCLRE_OFF

CBLOCK  	H'20'
	
	FRONT 		; Fronteira
	CONT0

ENDC

	org 	H'0000'
	goto 	inicio

inicio:
	banco1
	bcf 	TRISIO,GP5
	movlw 	H'11'		; W = B'00010001'
	movwf 	ANSEL 		; Tempo de convers�o em 2us para 4MHz (BIT<6:4>) &
						; Apenas AN0 configurado como Anal�gico (BIT<0>)
	banco0

	movlw 	H'01'		; W = B'00000001'
	movwf 	ADCON0 		; Convers�o anal�gica se inicia (BIT<0>), Convers�o Completa (BIT<1>),
						; Canal 0 (BIT<3:2>), N�o-implementado (BIT<5:4>),
						; Tens�o de refer�ncia = VDD (BIT<6>) & justificado para Esquerda, em ADRESH (BIT<7>)
	movlw 	H'07'		; W = B'00000111'
	movwf 	CMCON 		; Desabilita os comparadores dos pinos
	

; Definir valor da fronteira entre regi�o inferior e superior

;    ADRESH 			  ADRESL
; 0 0 0 0 0 0 0 0     0 0 x x x x x x
; O valor da fronteira = D'128' ou B'10000000'

	movlw 	D'128' 		; Metade de 256 (Valor m�ximo de 5V)
	movwf 	FRONT

Converter:
	movlw 	D'100'		; Contador de tempo
	movwf 	CONT0

	bsf 	ADCON0,GO_DONE 		; M�dulo ADC ocupado

Aguarde2us:
	btfsc 	ADCON0,GO_DONE      ; O dado est� pronto?
	goto 	Aguarde2us			; N�o, est�o aguarde
								
	movf 	ADRESH,W		    ; Sim,Ent�o movimenta o sinal anal�gico para W
	andwf 	FRONT,W 			; Fa�a um AND entre a Fronteira e o sinal anal�gico

	btfsc 	STATUS,Z			; Verifica se a opera��o AND deu zero
	goto 	Inferior			; N�o deu (Esta abaixo da fronteira do sinal) ent�o vai para Inferior
	bsf 	led					; Sim, deu! (Esta acima da fronteira do sinal),Ent�o liga o LED
	goto 	Delay 			    ; Espera um tempo e procura pelo pr�ximo sinal


Inferior:
	bcf 	led 				; Desligue o LED

Delay:
	
	call 	Time300us			; Delay de 100 x 3 Ciclos = 300us
	call 	Time300us
	call 	Time300us

	goto 	Converter 			; Espera pelo pr�ximo sinal



Time300us:
	decfsz 	CONT0,F
	goto 	Time300us
	return 	


	


end