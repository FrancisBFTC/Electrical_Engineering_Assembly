; **************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor  : Wender Francis   			  Cria��o: 06/12/2021 	  Revis�o: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactconf.inc 
; Programa: Sistema de Reator Industrial para despache da prepara��o de produtos
;
;
; Descri��o: Primeiramente configura o microcontrolador sem prote��es de mem�ria, WatchDog Timer
; Desativado e Oscila��o XT 4 MHz, depois inicia a configura��o dos registros especiais, no qual
; configuramos as entradas em PORTB e Sa�das em PORTC e PORTD. Vamos trabalhar com o Prescaler 256
; do TMR0 porque ser� configurado a interrup��o por overflow do TMR0 para sistemas de tempo. Vou
; utilizar a contagem mais usada aqui que seria ((TMR0-6) x Prescaler), onde o 6 ser� definida para um
; contador, no entanto para PSI e CELSIUS, os valores v�o simbolizar o que esta na prova, como 120 e 100
; pra ficar mais coerente. Interrup��o Global e Perif�rica s�o habilitadas como sempre, por�m a diferen�a
; � que no momento eu vou habilitar s� a interrup��o externa de RB0 para o Bot�o STOP. A Interrup��o do TMR0
; (TMR0IE) vou deixar pra habilitar direto nas macros e desabilitar livremente. Decidi fazer isto porque tive
; muitos problemas em rela��o ao tempo cont�nuo, que poderia interromper o sistema em tempos indevidos
; em momentos diversos, ent�o decidi habilitar ou desabilitar em locais espec�ficos somente em situa��es
; necess�rias. Justamente por isso habilitei no Banco 0, pois todo o c�digo estar no Banco 0 e INTCON
; pode ser acessado por qualquer Banco, ent�o pra n�o ficar mudando de Banco o tempo todo, decidi ativar
; logo no Banco 0. Depois disso s� limpar todos os registros e saltar para o MAIN.
; **************************************************************************************************

#IFNDEF 	__REACTCONF_INC__
#DEFINE 	__REACTCONF_INC__	; Isto permite s� incluir se ainda n�o foi inclu�do

; -----------------------------------------------------------------------------------------
; Palavra de Configura��o e Defini��es do Microcontrolador --------------------------------
LIST p=16F877A				   ; Microcontrolador PIC16F877A

#include 	<p16f877a.inc>	   ; Inclue arquivo do microcontrolador

__config 	_XT_OSC &  _WDT_OFF & _LVP_OFF & _BOREN_OFF & _CPD_OFF & _CP_OFF
; -----------------------------------------------------------------------------------------


; -----------------------------------------------------------------------------------------
; Vetores da mem�ria de Programa ----------------------------------------------------------
ORG 	H'0000'                ; Vetor de RESET
		goto 	INIT_CONF      ; Inicie as configura��es

ORG 	H'0004'                ; Vetor de interrup��o
		call 	VOID_INT       ; Chame a ISR 'VOID_INT'
retfie						   ; Retorna da Interrup��o
; -----------------------------------------------------------------------------------------


; -----------------------------------------------------------------------------------------
; Inicializa��o das Configura��es do PIC16F877A -------------------------------------------
INIT_CONF:
	BANK_SELECT 1 				; Seleciona Banco 1 de memoria
	movlw 	B'00000011'			; Mova para W o valor H'03'
	movwf 	TRISB				; TRISB = W   -> PORTB apenas RB0 e RB1 como entrada
	clrf 	TRISC				; Limpe TRISC -> PORTC tudo como sa�da
	clrf 	TRISD				; Limpe TRISD -> PORTD tudo como sa�da
	movlw 	B'11000111'			; Pull-Ups desabilitados, Int. Por subida em RB0 
	movwf 	OPTION_REG			; TMR0 Prescaler = 256 -> 111
	BANK_SELECT 0 				; Seleciona Banco 0 de memoria

	clrf 	INTCON				; Limpe o Registro INTCON
	bsf 	INTCON,GIE			; Habilita Interrup��o Global
	bsf 	INTCON,PEIE		    ; Habilita Interrup��o Perif�rica
	bsf 	INTCON,INTE		    ; Habilita Interrup��o externa RB0
   ;bsf 	INTCON,TMR0IE 		; N�o irei habilitar esta interrup��o por aqui

	clrf 	PORTA				; Limpe o Registro PORTA
	clrf 	PORTB				; Limpe o Registro PORTB
	clrf 	PORTC				; Limpe o Registro PORTC
	clrf 	PORTD				; Limpe o Registro PORTD
	clrf 	PORTE				; Limpe o Registro PORTE
	clrf 	FLAG1				; Limpa os bits de Flags dos contadores
	clrf 	FLAG2				; Limpa os bits de Flags dos contadores
	clrf 	COUNT				; Limpa o Contador de tempo
	clrf 	CELSIUS				; Limpa o contador de Graus Celsius
	clrf 	PSI 				; Limpa o contador de press�o
	bsf 	RESETS				; Acione LED de RESET caso iniciar ou resetar
				
	goto 	R_MAIN 				; Salte para o MAIN do Reator
; -----------------------------------------------------------------------------------------

#ENDIF