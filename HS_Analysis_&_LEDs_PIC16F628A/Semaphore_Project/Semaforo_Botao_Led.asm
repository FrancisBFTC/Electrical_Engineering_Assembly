; -------------------------------------------------------------------------
; Projeto Semaforo
; ITEMs A, B, C e D
; Assembly Microcontrolador PIC16F6284A
;
; Autor : Wender Francis  Data: 28/10/2021
; Clock : 4 MHz
; 
; Descri��o: Programa aciona led verde e pisca led vermelho dentro do tempo de 1s
; (1Hz), quando um bot�o � pressionado, desliga o led verde e liga o led vermelho
; dentro do tempo de 20ms (50Hz). Ap�s isso, O Led vermelho fica acionado por 2s e
; em seguida o led vermelho pisca a cada 250ms (2Hz), quando houver pressionamento
; de outro bot�o (Sensor que identifica pedrestre no final da faixa), o led vermelho
; desliga e come�a a piscar novamente a cada 500ms (1Hz) e o led verde volta a ser acionado
; como no inicio do programa.
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
#define  BOTAO_S    PORTB,RB0				; Pino RB0 da SFR PORTB
#define  SENSOR_C   PORTB,RB1				; Pino RB1 da SFR PORTB
#define	 LEDGREEN   PORTB,RB2 				; Pino RB2 da SFR PORTB
#define  LEDRED	    PORTB,RB3				; Pino RB3 da SFR PORTB	
#define  TIMER_ON   bsf PORTA,RA0			; Habilita o componente de Timer
#define  TIMER_OFF  bcf PORTA,RA0 		    ; Desabilita/pausa o componente de Timer

; *** Constantes Literais para Delay ***
F_50Hz 	 EQU 	D'4'
F_2Hz 	 EQU 	D'100'

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
; 	Configura os Pinos para entrada & sa�da dos Botoes e LEDs, respectivamente.	
; *****************************************************************************************
InicioPrograma:
	SelecionaBanco1         ; Banco 1 de mem�ria Selecionado para TRISA & TRISB  
	movlw 	H'FE' 			; WORK = B'11111110'
	movwf 	TRISA			; Todos os pinos como entrada no TRISA      
	movlw	H'F3' 			; WORK = H'F3' = B'11110011'
	movwf	TRISB 			; Pinos RB0 e RB1 como Entrada; Pinos RB2 e RB3 como sa�da
	SelecionaBanco0  		; Retorna ao Banco 0 de mem�ria para o PORTB
	
	movlw 	H'00'			; WORK = B'00000000'
	movwf 	PORTA			; Define todos os pinos para LOW
	movlw 	H'F7'			; WORK = B'11110111'
	movwf 	PORTB 			; Define todos os Pinos para HIGH, exceto pino RB3
	
	bsf 	LEDGREEN		; Acione LED verde em RB2
	bsf 	SENSOR_C		; Sete Sensor para HIGH (Nivel alto)
	bsf 	BOTAO_S			; Sete Botao para HIGH (Nivel alto)
; *****************************************************************************************


; *****************************************************************************************
; Enquanto os veiculos passam pela via, o sinal verde � acionado e o sinal vermelho pisca
; numa frequ�ncia de 1Hz (1 segundo), at� que um Bot�o seja pressionado pelo pedestre.
; OBS.: Botao conectado em resistor de pull-up, logo se pressionado, envia
; nivel logico baixo, ou seja, 0. (Circuito Inversor Por Processamento)
; *****************************************************************************************
; ITEM A - Veiculos em Tr�nsito
VeiculosPassando:
	bsf 	LEDRED				; Acione o sinal vermelho
	call 	Delay1Hz			; Chama um delay de 1Hz = 1 Segundo / 2 (2 estados)
	bcf 	LEDRED				; Desacione o sinal vermelho
	call 	Delay1Hz			; Tempo de 500ms
	btfsc	BOTAO_S       		; Testa o bit RB0 de PORTB, Salte abaixo se recebeu n�vel l�gico 0
   	goto    VeiculosPassando 	; Se receber n�vel l�gico 1, Volte e teste novamente
	
; ITEM B - Pedestre pressiona para atravessar	
PedestreBotao:					; Apos o pedestre pressionar o botao, execute esta Rotina
	bcf 	LEDGREEN 			; Desacione o sinal verde apos Delay
	movlw 	F_50Hz 				; Move o valor 4 para W
	movwf 	DELAY1				; Inicializa vari�vel DELAY1 com 4 (Pra 50Hz no Delay)
	call   	DelayXHz			; Espere 10ms (1s / 50Hz / 2 -> 0,01s)	
	bsf 	LEDRED 				; Acione o sinal vermelho ap�s Delay

; ITEM C - Pedestre atravessando
PedestrePassando:				; Nesta etapa, o pedestre esta atravessando a faixa
	call 	Delay1Hz			; Espere 500ms no delay
	call 	Delay1Hz			; Espere 500ms no delay (J� foi 1s do LED acionado)
PisqueSinal:
	bcf 	LEDRED				; Desacione o sinal vermelho
	movlw 	F_2Hz 				; Move o valor 100 para W
	movwf 	DELAY1				; Inicializa vari�vel DELAY1 com 100 (Pra 2Hz no Delay)
	call   	DelayXHz			; Espere 250ms (1s / 2Hz / 2 -> 0,25s)
	bsf 	LEDRED 				; Acione o sinal vermelho
	movlw 	F_2Hz 				; Move o valor 100 para W
	movwf 	DELAY1				; Inicializa vari�vel DELAY1 com 100 (Pra 2Hz no Delay)
	call   	DelayXHz			; Espere 250ms (1s / 2Hz / 2-> 0,25s)
	btfsc 	SENSOR_C
	goto 	PisqueSinal
	
; ITEM D - Pedestre atravessa & Ve�culos voltam a transitar
PedestreChegou:					; Pedestre atravessou a faixa apos o sensor identificar
	bcf 	LEDRED				; Desacione o sinal vermelho
	call 	Delay1Hz			; Espere 500ms
	bsf 	LEDGREEN			; Acione o sinal verde
	goto 	VeiculosPassando	; Volta pro inicio e reinicialize o sinal vermelho
; *****************************************************************************************	
	

; *** Desenvolvimento Das Sub-Rotinas ***

; ********************************************************************************************
; Calculo de Delay pelo Ciclo de Maquina:
;	Ciclo de M�quina = 1 / (Freq_Cristal / 4) = 1us (MicroSegundos);
; 	(4MHz / 4 Ciclos) = 1000000 Hz (PIC_Speed = 1MHz);
; 	1 / 1000000Hz = 0,000001s = 1 MicroSegundos;
; 	1 Ciclo = 1 MicroSegundos;
; 	Cada instru��o gasta 1 Ciclo, Instru��es de salto gasta 2 ciclos;
; 	Time2 executa 10 Ciclos 250 vezes, Time1 executa 200 vezes e
;	Delay1S executa 2 vezes, logo: 250 x 10 x 200 = 500000 Ciclos = 500000 MicroSegundos;
; 	OBS.: Devido a mais ciclos em excesso, o tempo pode ultrapassar valores desconsideraveis.
; *********************************************************************************************
Delay1Hz:					; Delay de 1 Segundo
	TIMER_ON				; Inicia o Temporizador
Time0:
	movlw 	D'200' 			; Move o valor 182 para W
	movwf 	DELAY1			; Inicializa vari�vel DELAY1 com 200
Time1:						; J� gastou 5 ciclos de m�quina (Em Excesso)
	movlw 	D'250'			; Move o valor 250 para W
	movwf 	DELAY2			; Inicializa variavel DELAY2 com 250
Time2:
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop
	nop
	nop
	decfsz 	DELAY2			; Decrementa DELAY2 at� que seja igual a 0
	goto 	Time2			; Vai para a label Time2 se n�o for 0
							; OBS.: GOTO e RETURN gasta 2 ciclos de m�quinas, logo foram 10 ciclos at� aqui
							; 250 x 10 ciclos de m�quina = 2500 ciclos
	decfsz 	DELAY1			; Decrementa DELAY1 at� que seja igual a 0
	goto 	Time1			; Vai para a label Time1 se n�o for 0
							; OBS.: Instru��es acima gastou 3 ciclos de m�quina (Em Excesso)
							; 2500 x 200 = 500000 ciclos de m�quina
	TIMER_OFF				; Pause o temporizador
	return 					; Retorna para a chamada
; *********************************************************************************************


; ********************************************************************************************
; 50Hz = 20ms / 2 estados = 10ms
; 2 Hz = 1s / 2 estados = 500ms
; DELAY1 = 4  -> 50Hz (8 / 2)
; DELAY1 = 100 -> 2Hz (200 / 2)
; Tudo depende do que estar� em 'DELAY1'
; ********************************************************************************************
DelayXHz:					; Delay de 20 a 500 Milisegundos
	TIMER_ON				; Inicia o Temporizador
Time50_1:					; J� gastou 6 ciclos de m�quina (Em Excesso)
	movlw 	D'250'			; Move o valor 250 para W
	movwf 	DELAY2			; Inicializa variavel DELAY2 com 250
Time50_2:
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina
	nop						; Gasta 1 ciclo de m�quina			
	nop						; Gasta 1 ciclo de m�quina
	decfsz 	DELAY2			; Decrementa DELAY2 at� que seja igual a 0
	goto 	Time50_2		; Vai para a label Time50_2 se n�o for 0
							; OBS.: GOTO gasta 2 ciclos de m�quinas, logo foram 10 ciclos at� aqui
							; 200 x 10 ciclos de m�quina = 2000 ciclos
	decfsz 	DELAY1			; Decrementa DELAY1 at� que seja igual a 0
	goto 	Time50_1		; Vai para a label Time50_1 se n�o for 0
							; OBS.: Instru��es acima gastou 3 ciclos de m�quina (Em Excesso)
							; 2000 x 10 = 20000 ciclos de m�quina
							; 20000 Ciclos = 20ms = 0,02s = 1s / 50Hz.
	TIMER_OFF				; Pause o temporizador
	return 					; Retorna para a chamada
; ********************************************************************************************

							
end 						; Final do programa