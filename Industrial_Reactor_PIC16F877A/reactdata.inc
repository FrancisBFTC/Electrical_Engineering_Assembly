; **************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor  : Wender Francis   			  Criação: 06/12/2021 	  Revisão: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactdata.inc 
; Programa: Sistema de Reator Industrial para despache da preparação de produtos
;
;
; Descrição: Este é um arquivo de inclusão com definições de dados de entrada e saída,
; Relacionado as válvulas, sensores e motores. Os pinos de PORTB,PORTC e PORTD serão 
; Reservados para estes módulos porém os bits dos registros FLAG1 & FLAG2 são para acionamentos
; específicos equivalentes a temporização e contagens, como as contagens dos registros 
; COUNT, CELSIUS e PSI onde COUNT faz contagens auxiliares de tempo junto com TMR0 para algumas
; Válvulas, exceto V3 e V4 pois para V3 o registro CELSIUS faz a contagem de 100Cº e para V4
; o registro PSI faz a contagem de 120psi, no final das contagens o sistema desaciona FLAGs 
; que vai determinar se TT & TP serão acionados, respectivamente. As outras variáveis com o "_TEMP"
; são para armazenar/recuperar contextos dos dados durante a interrupção de RB0/INT, Pino que é conectado
; no Botão STOP, isto porque o sistema é pausado, logo todos os registros precisam ser salvos antes
; deles serem apagados dentro da Interrupção.
; **************************************************************************************************

#IFNDEF 	__REACTDATA_INC__
#DEFINE 	__REACTDATA_INC__	; Isto permite só incluir se ainda não foi incluído


#define 	I0		PORTB	 ; I0 será as entradas de dados do sistema (Apenas RB0 e RB1)
#define 	Q0		PORTD	 ; Q0 será as 1ª saídas de dados do sistema
#define 	Q1 		PORTC	 ; Q1 será as 2ª saídas de dados do sistema

#define 	STOP 	I0,RB0	 ; STOP  -> PORTB,0 = Interrupção (Botão) 
#define 	START   I0,RB1	 ; START -> PORTB,1 = Inicio  	  (Botão)

#define 	LH 		Q1,RC0   ; LH   -> PORTC,0 = Nivel Alto  (LED-GREEN)
#define 	LM 		Q1,RC1	 ; LM   -> PORTC,1 = Nivel Médio (LED-RED)
#define 	LL 		Q1,RC2	 ; LL   -> PORTC,2 = Nivel Baixo (LED-YELLOW)
#define 	TT 		Q1,RC3	 ; TT   -> PORTC,3 = Temperatura (LED-BLUE)
#define 	TP 		Q1,RC4	 ; TP 	-> PORTC,4 = Pressão     (LED-YELLOW)
#define 	ST 		Q1,RC5	 ; ST 	-> PORTC,5 = Indica Reator Operando
#define 	FN 		Q1,RC6 	 ; FN 	-> PORTC,6 = Indica Reator Finalizando
#define 	PS 		Q1,RC7   ; PS 	-> PORTC,7 = Indica Reator Pausado

#define 	V1	 	Q0,RD0 	 ; V1   -> PORTD,0 = Válvula 1 (Motor-DC)
#define 	V2		Q0,RD1	 ; V2   -> PORTD,1 = Válvula 2 (Motor-DC)
#define 	V3		Q0,RD2   ; V3   -> PORTD,2 = Válvula 3 (Motor-DC)
#define 	V4		Q0,RD3   ; V4   -> PORTD,3 = Válvula 4 (Motor-DC)
#define 	V5		Q0,RD4   ; V5   -> PORTD,4 = Válvula 5 (Motor-DC)
#define 	V6		Q0,RD5   ; V6   -> PORTD,5 = Válvula 6 (Motor-DC)
#define 	MOT1	Q0,RD6   ; MOT1 -> PORTD,6 = Motor 1   (Motor-DC)
#define 	BOMB   	Q0,RD7   ; BOMB -> PORTD,7 = Bomba     (Motor-DC)	

#define 	C100 	FLAG1,0  ; Flag que indica 100 graus celsius de TT
#define 	T1	 	FLAG1,1	 ; Flag de Retardo de 1s entre V1 e V2
#define 	RT    	FLAG1,2	 ; Flag de Repouso de 3s para Temperatura TT
#define 	P1L 	FLAG1,3	 ; Flag do nível baixo do Produto1 para sensor LL
#define 	P1M 	FLAG1,4	 ; Flag do nível médio do Produto1 para sensor LM
#define 	P2H 	FLAG1,5	 ; Flag do nível alto do Produto2 para sensor LH
#define 	T12 	FLAG1,6	 ; Flag de Retenção de 12s para MOT1
#define 	P3	 	FLAG1,7	 ; Flag de Retenção de 3s para BOMBA do Produto3

#define 	LHON 	FLAG2,0  ; Flag que indica LH acionado
#define 	LMON	FLAG2,1  ; Flag que indica LM acionado
#define 	LLON 	FLAG2,2  ; Flag que indica LL acionado
#define 	RESC 	FLAG2,3  ; Flag que indica reservatorio secundario cheio
#define 	P120	FLAG2,4  ; Flag que indica 120 psi de pressão
#define 	EOP 	FLAG2,5  ; Flag que indica fim do programa

#define 	TIMER   PORTB,RB2	; Utilizado como saída para módulo timer de V2 no Proteus	 
#define 	RESETS 	PORTB,RB3	; Utilizado como saída para LED de RESET

CBLOCK 		H'20'			; Bloco da memória de dados
	TEMP 					; Registro temporario p/ Instruções booleanas
	W_TEMP					; Registro WORK temporario p/ Interrupcoes Gerais
	STATUS_TEMP				; Registro STATUS temporario p/ Interrupcoes Gerais
	PCLATH_TEMP				; Registro PCLATH temporario p/ Interrupcoes Gerais
	Q0_TEMP 				; Registro Q0 temporario (PORTD) p/ Interrupção do STOP
	Q1_TEMP					; Registro Q1 temporario (PORTC) p/ Interrupção do STOP
	FLAG1_TEMP				; Registro FLAG1 temporario (Para temporizadores) p/ Interrupção do STOP
	FLAG2_TEMP				; Registro FLAG2 temporario (Para Acionamentos) p/ Interrupção do STOP
	CELS_TEMP				; Registro contador CELSIUS temporario p/ Interrupção do STOP
	PSI_TEMP				; Registro contador PSI temporario p/ Interrupção do STOP
	TMR0_TEMP				; Registro TMR0 temporario p/ Interrupção do STOP
	COUNT_TEMP				; Registro COUNT temporario p/ Interrupção do STOP
	FLAG1 					; Registro de Flags para tempo e contagens
	FLAG2 					; Registro de Flags de acionamento
	COUNT 					; Contador de Tempo para valvulas (Exceto pra V3 e V4)
	CELSIUS					; Registro contador de Graus Celsius para TT
	PSI 					; Registro contador de Pressão Interna para TP
	DELAY1					; Registro para contagem de Delay na ISR p/ Interrupção do STOP
	DELAY2					; Registro para contagem de Delay na ISR p/ Interrupção do STOP
ENDC						; Fim do Bloco da memória de dados

#ENDIF