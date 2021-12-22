; -------------------------------------------------------------------------
; Assembly Microcontrolador PIC16F6284A
;
; Autor : Wender Francis   Data: 28/10/2021
; Clock : 4 MHz
; 
; Descri��o: Programa que aciona um LED ao pressionar um botao apos o Delay
; de 2 Segundos. Botao conectado no pino RA4 e Led conectado no pino RB0.
; -------------------------------------------------------------------------

; *** Defini��o do Microcontrolador Utilizado ***
list 	p=16F628A

; *** Inclus�o das defini��es de endere�os do PIC ***
#include	<p16f628a.inc>

; *** Configura��o dos FUSE BITs ***
; Cristal oscilador externo de 4MHz, sem watchdog timer, com power up timer e sem protecao de codigo
__config 	_XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF

; *** Paginacao de memoria ***
#define  SelecionaBanco0  bcf  STATUS,RP0		; Limpa BIT RP0 da SFR STATUS
#define  SelecionaBanco1  bsf  STATUS,RP0		; Seta BIT RP0 da SFR STATUS

; *** Defini��es de Entradas e Sa�das de Dados ***
#define  BOTAO     PORTA,RA4				; Pino RA4 da SFR PORTA
#define  LED       PORTB,RB0				; Pino RB0 da SFR PORTB
#define  TIMER_ON   bsf PORTB,RB1			; Habilita o componente de Timer
#define  TIMER_OFF  bcf PORTB,RB1 		    ; Desabilita/pausa o componente de Timer

; *** Aloca��o dos registradores de Uso Gerais ***
CBLOCK		H'20'			; Inicio da memoria de usuario no endere�o 0x0C
	DELAY1					; Registrador Auxiliar para Delay
	DELAY2					; Registrador Auxiliar para Delay
	DELAY3					; Registrador Auxiliar para Delay
ENDC						; Final da memoria de usuario

; *** Vetor de RESET ***
	org 	H'0000'
	goto 	InicioPrograma	; Salta o Vetor de Interrup��o, indo para o InicioPrograma

; *** Vetor de Interrupcao ***
	org 	H'0004'
	retfie


; *****************************************************************************************
; Inicio do Programa:
; 	Configura os Pinos para entrada & sa�da do Botao e do LED, respectivamente.	
; *****************************************************************************************
InicioPrograma:
	SelecionaBanco1         ; Banco 1 de mem�ria Selecionado para TRISA & TRISB  
	movlw 	H'FF' 			; WORK = B'11111111'
	movwf 	TRISA			; Todos os pinos como entrada no TRISA      
	movlw	H'FC' 			; WORK = H'FC' = B'11111100'
	movwf	TRISB 			; Apenas o pino RB0 e RB1 como Sa�da no TRISB (RB1 vai habilitar o Timer)
	SelecionaBanco0  		; Retorna ao Banco 0 de mem�ria para o PORTB
	
	movlw 	H'FF'			; WORK = B'11111111'
	movwf 	PORTA			; Define todos os pinos para HIGH
	movlw 	H'FC'			; WORK = B'11111100'
	movwf 	PORTB 			; Define os pinos RB0 e RB1 de PORTB para Desligados
	bcf 	LED				; Desacione o LED em RB0


; *****************************************************************************************
; Espere pelo Botao ser pressionado, caso sim, espere 2s e Acione o LED
; OBS.: Botao conectado em resistor de pull-up, logo se pressionado, envia
; nivel logico baixo, ou seja, 0. (Circuito Inversor Por Processamento)
; *****************************************************************************************
EspereBotao:
	btfsc	BOTAO      		; Testa o bit RA4 de PORTA, Salte abaixo se recebeu n�vel l�gico 0
   	goto    EspereBotao		; Se receber n�vel l�gico 1, Volte e teste novamente
	call   	Delay2S			; Chama um Delay de 2 Segundos	(500ms x 4)	
	bsf 	LED 			; Acione o LED em RB0 Ap�s Delay
	goto 	$				; Trave o programa nesse loop infinito



; ********************************************************************************************
; Calculo de Delay pelo Ciclo de Maquina:
;	Ciclo de M�quina = 1 / (Freq_Cristal / 4) = 1us (MicroSegundos);
; 	(4MHz / 4 Ciclos) = 1000000 Hz (PIC_Speed = 1MHz);
; 	1 / 1000000Hz = 0,000001s = 1 MicroSegundos;
; 	1 Ciclo = 1 MicroSegundos;
; 	Cada instru��o gasta 1 Ciclo, Instru��es de salto gasta 2 ciclos;
; 	Time2 executa 10 Ciclos 250 vezes, Time1 executa 200 vezes e
;	Delay2S executa 4 vezes, logo: 250 x 10 x 200 x 4 = 2000000 Ciclos = 2000000 MicroSegundos;
; 	OBS.: Devido a mais ciclos em excesso, o tempo pode ultrapassar valores desconsideraveis.
; *********************************************************************************************
Delay2S:
	movlw 	D'4'			; Move o valor 4 para W
	movwf 	DELAY3			; Inicializa variavel DELAY3 com 4
	TIMER_ON				; Inicia o Temporizador
Time0:
	movlw 	D'200' 			; Move o valor 200 para W
	movwf 	DELAY1			; Inicializa vari�vel DELAY1 com 200
Time1:						; J� gastou 6 ciclos de m�quina (Em Excesso)
	movlw 	D'250'			; Move o valor 250 para W
	movwf 	DELAY2			; Inicializa variavel DELAY2 com 250
Time2:
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	decfsz 	DELAY2			; Decrementa DELAY2 at� que seja igual a 0
	goto 	Time2			; Vai para a label Time2 se n�o for 0
							; OBS.: GOTO gasta 2 ciclos de m�quinas, logo foram 10 ciclos at� aqui
							; 250 x 10 ciclos de m�quina = 2500 ciclos
	decfsz 	DELAY1			; Decrementa DELAY1 at� que seja igual a 0
	goto 	Time1			; Vai para a label Time1 se n�o for 0
							; OBS.: Instru��es acima gastou 3 ciclos de m�quina (Em Excesso)
							; 2500 x 200 = 500000 ciclos de m�quina
	decfsz 	DELAY3			; Decrementa DELAY3 at� que seja igual a 0		
	goto 	Time0			; Repita o Delay de 500ms tudo novamente (Vai repetir 4 vezes)
							; 500000 Ciclos x 4 = 2000000 Ciclos = 2000ms = 2s
	TIMER_OFF				; Pause o temporizador
	return 					; Retorna para a chamada
							
end 						; Final do programa