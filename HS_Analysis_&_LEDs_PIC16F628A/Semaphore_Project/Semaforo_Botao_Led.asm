; -------------------------------------------------------------------------
; Projeto Semaforo
; ITEMs A, B, C e D
; Assembly Microcontrolador PIC16F6284A
;
; Autor : Wender Francis  Data: 28/10/2021
; Clock : 4 MHz
; 
; Descrição: Programa aciona led verde e pisca led vermelho dentro do tempo de 1s
; (1Hz), quando um botão é pressionado, desliga o led verde e liga o led vermelho
; dentro do tempo de 20ms (50Hz). Após isso, O Led vermelho fica acionado por 2s e
; em seguida o led vermelho pisca a cada 250ms (2Hz), quando houver pressionamento
; de outro botão (Sensor que identifica pedrestre no final da faixa), o led vermelho
; desliga e começa a piscar novamente a cada 500ms (1Hz) e o led verde volta a ser acionado
; como no inicio do programa.
; -------------------------------------------------------------------------

; *** Definição do Microcontrolador Utilizado ***
list 	p=16F628A

; *** Inclusão das definições de endereços do PIC ***
#include	<p16f628a.inc>

; *** Configuração dos FUSE BITs ***
; Cristal oscilador externo de 4MHz, sem watchdog timer, com power up timer e sem protecao de codigo
__config 	_XT_OSC & _WDT_OFF & _PWRTE_ON & _CP_OFF

; *** Paginacao de memoria ***
#define  SelecionaBanco0  bcf  STATUS,RP0		; Limpa BIT RP0 da SFR STATUS
#define  SelecionaBanco1  bsf  STATUS,RP0		; Seta BIT RP0 da SFR STATUS

; *** Definições de Entradas e Saídas de Dados ***
#define  BOTAO_S    PORTB,RB0				; Pino RB0 da SFR PORTB
#define  SENSOR_C   PORTB,RB1				; Pino RB1 da SFR PORTB
#define	 LEDGREEN   PORTB,RB2 				; Pino RB2 da SFR PORTB
#define  LEDRED	    PORTB,RB3				; Pino RB3 da SFR PORTB	
#define  TIMER_ON   bsf PORTA,RA0			; Habilita o componente de Timer
#define  TIMER_OFF  bcf PORTA,RA0 		    ; Desabilita/pausa o componente de Timer

; *** Constantes Literais para Delay ***
F_50Hz 	 EQU 	D'4'
F_2Hz 	 EQU 	D'100'

; *** Alocação dos registradores de Uso Gerais ***
CBLOCK		H'20'			; Inicio da memoria de usuario no endereço 0x0C
	DELAY1					; Registrador Auxiliar para Delay
	DELAY2					; Registrador Auxiliar para Delay
	DELAY3					; Registrador Auxiliar para Delay
ENDC						; Final da memoria de usuario

; *** Vetor de RESET ***
	org 	H'0000'
	goto 	InicioPrograma	; Salta o Vetor de Interrupção, indo para o InicioPrograma

; *** Vetor de Interrupcao ***
	org 	H'0004'
	retfie


; *****************************************************************************************
; Inicio do Programa:
; 	Configura os Pinos para entrada & saída dos Botoes e LEDs, respectivamente.	
; *****************************************************************************************
InicioPrograma:
	SelecionaBanco1         ; Banco 1 de memória Selecionado para TRISA & TRISB  
	movlw 	H'FE' 			; WORK = B'11111110'
	movwf 	TRISA			; Todos os pinos como entrada no TRISA      
	movlw	H'F3' 			; WORK = H'F3' = B'11110011'
	movwf	TRISB 			; Pinos RB0 e RB1 como Entrada; Pinos RB2 e RB3 como saída
	SelecionaBanco0  		; Retorna ao Banco 0 de memória para o PORTB
	
	movlw 	H'00'			; WORK = B'00000000'
	movwf 	PORTA			; Define todos os pinos para LOW
	movlw 	H'F7'			; WORK = B'11110111'
	movwf 	PORTB 			; Define todos os Pinos para HIGH, exceto pino RB3
	
	bsf 	LEDGREEN		; Acione LED verde em RB2
	bsf 	SENSOR_C		; Sete Sensor para HIGH (Nivel alto)
	bsf 	BOTAO_S			; Sete Botao para HIGH (Nivel alto)
; *****************************************************************************************


; *****************************************************************************************
; Enquanto os veiculos passam pela via, o sinal verde é acionado e o sinal vermelho pisca
; numa frequência de 1Hz (1 segundo), até que um Botão seja pressionado pelo pedestre.
; OBS.: Botao conectado em resistor de pull-up, logo se pressionado, envia
; nivel logico baixo, ou seja, 0. (Circuito Inversor Por Processamento)
; *****************************************************************************************
; ITEM A - Veiculos em Trânsito
VeiculosPassando:
	bsf 	LEDRED				; Acione o sinal vermelho
	call 	Delay1Hz			; Chama um delay de 1Hz = 1 Segundo / 2 (2 estados)
	bcf 	LEDRED				; Desacione o sinal vermelho
	call 	Delay1Hz			; Tempo de 500ms
	btfsc	BOTAO_S       		; Testa o bit RB0 de PORTB, Salte abaixo se recebeu nível lógico 0
   	goto    VeiculosPassando 	; Se receber nível lógico 1, Volte e teste novamente
	
; ITEM B - Pedestre pressiona para atravessar	
PedestreBotao:					; Apos o pedestre pressionar o botao, execute esta Rotina
	bcf 	LEDGREEN 			; Desacione o sinal verde apos Delay
	movlw 	F_50Hz 				; Move o valor 4 para W
	movwf 	DELAY1				; Inicializa variável DELAY1 com 4 (Pra 50Hz no Delay)
	call   	DelayXHz			; Espere 10ms (1s / 50Hz / 2 -> 0,01s)	
	bsf 	LEDRED 				; Acione o sinal vermelho após Delay

; ITEM C - Pedestre atravessando
PedestrePassando:				; Nesta etapa, o pedestre esta atravessando a faixa
	call 	Delay1Hz			; Espere 500ms no delay
	call 	Delay1Hz			; Espere 500ms no delay (Já foi 1s do LED acionado)
PisqueSinal:
	bcf 	LEDRED				; Desacione o sinal vermelho
	movlw 	F_2Hz 				; Move o valor 100 para W
	movwf 	DELAY1				; Inicializa variável DELAY1 com 100 (Pra 2Hz no Delay)
	call   	DelayXHz			; Espere 250ms (1s / 2Hz / 2 -> 0,25s)
	bsf 	LEDRED 				; Acione o sinal vermelho
	movlw 	F_2Hz 				; Move o valor 100 para W
	movwf 	DELAY1				; Inicializa variável DELAY1 com 100 (Pra 2Hz no Delay)
	call   	DelayXHz			; Espere 250ms (1s / 2Hz / 2-> 0,25s)
	btfsc 	SENSOR_C
	goto 	PisqueSinal
	
; ITEM D - Pedestre atravessa & Veículos voltam a transitar
PedestreChegou:					; Pedestre atravessou a faixa apos o sensor identificar
	bcf 	LEDRED				; Desacione o sinal vermelho
	call 	Delay1Hz			; Espere 500ms
	bsf 	LEDGREEN			; Acione o sinal verde
	goto 	VeiculosPassando	; Volta pro inicio e reinicialize o sinal vermelho
; *****************************************************************************************	
	

; *** Desenvolvimento Das Sub-Rotinas ***

; ********************************************************************************************
; Calculo de Delay pelo Ciclo de Maquina:
;	Ciclo de Máquina = 1 / (Freq_Cristal / 4) = 1us (MicroSegundos);
; 	(4MHz / 4 Ciclos) = 1000000 Hz (PIC_Speed = 1MHz);
; 	1 / 1000000Hz = 0,000001s = 1 MicroSegundos;
; 	1 Ciclo = 1 MicroSegundos;
; 	Cada instrução gasta 1 Ciclo, Instruções de salto gasta 2 ciclos;
; 	Time2 executa 10 Ciclos 250 vezes, Time1 executa 200 vezes e
;	Delay1S executa 2 vezes, logo: 250 x 10 x 200 = 500000 Ciclos = 500000 MicroSegundos;
; 	OBS.: Devido a mais ciclos em excesso, o tempo pode ultrapassar valores desconsideraveis.
; *********************************************************************************************
Delay1Hz:					; Delay de 1 Segundo
	TIMER_ON				; Inicia o Temporizador
Time0:
	movlw 	D'200' 			; Move o valor 182 para W
	movwf 	DELAY1			; Inicializa variável DELAY1 com 200
Time1:						; Já gastou 5 ciclos de máquina (Em Excesso)
	movlw 	D'250'			; Move o valor 250 para W
	movwf 	DELAY2			; Inicializa variavel DELAY2 com 250
Time2:
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop
	nop
	nop
	decfsz 	DELAY2			; Decrementa DELAY2 até que seja igual a 0
	goto 	Time2			; Vai para a label Time2 se não for 0
							; OBS.: GOTO e RETURN gasta 2 ciclos de máquinas, logo foram 10 ciclos até aqui
							; 250 x 10 ciclos de máquina = 2500 ciclos
	decfsz 	DELAY1			; Decrementa DELAY1 até que seja igual a 0
	goto 	Time1			; Vai para a label Time1 se não for 0
							; OBS.: Instruções acima gastou 3 ciclos de máquina (Em Excesso)
							; 2500 x 200 = 500000 ciclos de máquina
	TIMER_OFF				; Pause o temporizador
	return 					; Retorna para a chamada
; *********************************************************************************************


; ********************************************************************************************
; 50Hz = 20ms / 2 estados = 10ms
; 2 Hz = 1s / 2 estados = 500ms
; DELAY1 = 4  -> 50Hz (8 / 2)
; DELAY1 = 100 -> 2Hz (200 / 2)
; Tudo depende do que estará em 'DELAY1'
; ********************************************************************************************
DelayXHz:					; Delay de 20 a 500 Milisegundos
	TIMER_ON				; Inicia o Temporizador
Time50_1:					; Já gastou 6 ciclos de máquina (Em Excesso)
	movlw 	D'250'			; Move o valor 250 para W
	movwf 	DELAY2			; Inicializa variavel DELAY2 com 250
Time50_2:
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina			
	nop						; Gasta 1 ciclo de máquina
	decfsz 	DELAY2			; Decrementa DELAY2 até que seja igual a 0
	goto 	Time50_2		; Vai para a label Time50_2 se não for 0
							; OBS.: GOTO gasta 2 ciclos de máquinas, logo foram 10 ciclos até aqui
							; 200 x 10 ciclos de máquina = 2000 ciclos
	decfsz 	DELAY1			; Decrementa DELAY1 até que seja igual a 0
	goto 	Time50_1		; Vai para a label Time50_1 se não for 0
							; OBS.: Instruções acima gastou 3 ciclos de máquina (Em Excesso)
							; 2000 x 10 = 20000 ciclos de máquina
							; 20000 Ciclos = 20ms = 0,02s = 1s / 50Hz.
	TIMER_OFF				; Pause o temporizador
	return 					; Retorna para a chamada
; ********************************************************************************************

							
end 						; Final do programa