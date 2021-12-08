; **************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor  : Wender Francis   			  Criação: 06/12/2021 	  Revisão: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactconf.inc 
; Programa: Sistema de Reator Industrial para despache da preparação de produtos
;
;
; Descrição: Primeiramente configura o microcontrolador sem proteções de memória, WatchDog Timer
; Desativado e Oscilação XT 4 MHz, depois inicia a configuração dos registros especiais, no qual
; configuramos as entradas em PORTB e Saídas em PORTC e PORTD. Vamos trabalhar com o Prescaler 256
; do TMR0 porque será configurado a interrupção por overflow do TMR0 para sistemas de tempo. Vou
; utilizar a contagem mais usada aqui que seria ((TMR0-6) x Prescaler), onde o 6 será definida para um
; contador, no entanto para PSI e CELSIUS, os valores vão simbolizar o que esta na prova, como 120 e 100
; pra ficar mais coerente. Interrupção Global e Periférica são habilitadas como sempre, porém a diferença
; é que no momento eu vou habilitar só a interrupção externa de RB0 para o Botão STOP. A Interrupção do TMR0
; (TMR0IE) vou deixar pra habilitar direto nas macros e desabilitar livremente. Decidi fazer isto porque tive
; muitos problemas em relação ao tempo contínuo, que poderia interromper o sistema em tempos indevidos
; em momentos diversos, então decidi habilitar ou desabilitar em locais específicos somente em situações
; necessárias. Justamente por isso habilitei no Banco 0, pois todo o código estar no Banco 0 e INTCON
; pode ser acessado por qualquer Banco, então pra não ficar mudando de Banco o tempo todo, decidi ativar
; logo no Banco 0. Depois disso só limpar todos os registros e saltar para o MAIN.
; **************************************************************************************************

#IFNDEF 	__REACTCONF_INC__
#DEFINE 	__REACTCONF_INC__	; Isto permite só incluir se ainda não foi incluído

; -----------------------------------------------------------------------------------------
; Palavra de Configuração e Definições do Microcontrolador --------------------------------
LIST p=16F877A				   ; Microcontrolador PIC16F877A

#include 	<p16f877a.inc>	   ; Inclue arquivo do microcontrolador

__config 	_XT_OSC &  _WDT_OFF & _LVP_OFF & _BOREN_OFF & _CPD_OFF & _CP_OFF
; -----------------------------------------------------------------------------------------


; -----------------------------------------------------------------------------------------
; Vetores da memória de Programa ----------------------------------------------------------
ORG 	H'0000'                ; Vetor de RESET
		goto 	INIT_CONF      ; Inicie as configurações

ORG 	H'0004'                ; Vetor de interrupção
		call 	VOID_INT       ; Chame a ISR 'VOID_INT'
retfie						   ; Retorna da Interrupção
; -----------------------------------------------------------------------------------------


; -----------------------------------------------------------------------------------------
; Inicialização das Configurações do PIC16F877A -------------------------------------------
INIT_CONF:
	BANK_SELECT 1 				; Seleciona Banco 1 de memoria
	movlw 	B'00000011'			; Mova para W o valor H'03'
	movwf 	TRISB				; TRISB = W   -> PORTB apenas RB0 e RB1 como entrada
	clrf 	TRISC				; Limpe TRISC -> PORTC tudo como saída
	clrf 	TRISD				; Limpe TRISD -> PORTD tudo como saída
	movlw 	B'11000111'			; Pull-Ups desabilitados, Int. Por subida em RB0 
	movwf 	OPTION_REG			; TMR0 Prescaler = 256 -> 111
	BANK_SELECT 0 				; Seleciona Banco 0 de memoria

	clrf 	INTCON				; Limpe o Registro INTCON
	bsf 	INTCON,GIE			; Habilita Interrupção Global
	bsf 	INTCON,PEIE		    ; Habilita Interrupção Periférica
	bsf 	INTCON,INTE		    ; Habilita Interrupção externa RB0
   ;bsf 	INTCON,TMR0IE 		; Não irei habilitar esta interrupção por aqui

	clrf 	PORTA				; Limpe o Registro PORTA
	clrf 	PORTB				; Limpe o Registro PORTB
	clrf 	PORTC				; Limpe o Registro PORTC
	clrf 	PORTD				; Limpe o Registro PORTD
	clrf 	PORTE				; Limpe o Registro PORTE
	clrf 	FLAG1				; Limpa os bits de Flags dos contadores
	clrf 	FLAG2				; Limpa os bits de Flags dos contadores
	clrf 	COUNT				; Limpa o Contador de tempo
	clrf 	CELSIUS				; Limpa o contador de Graus Celsius
	clrf 	PSI 				; Limpa o contador de pressão
	bsf 	RESETS				; Acione LED de RESET caso iniciar ou resetar
				
	goto 	R_MAIN 				; Salte para o MAIN do Reator
; -----------------------------------------------------------------------------------------

#ENDIF