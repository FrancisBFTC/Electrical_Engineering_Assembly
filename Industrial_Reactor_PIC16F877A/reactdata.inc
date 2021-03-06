; **************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor  : Wender Francis   			  Cria��o: 06/12/2021 	  Revis�o: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactdata.inc 
; Programa: Sistema de Reator Industrial para despache da prepara��o de produtos
;
;
; Descri��o: Este � um arquivo de inclus�o com defini��es de dados de entrada e sa�da,
; Relacionado as v�lvulas, sensores e motores. Os pinos de PORTB,PORTC e PORTD ser�o 
; Reservados para estes m�dulos por�m os bits dos registros FLAG1 & FLAG2 s�o para acionamentos
; espec�ficos equivalentes a temporiza��o e contagens, como as contagens dos registros 
; COUNT, CELSIUS e PSI onde COUNT faz contagens auxiliares de tempo junto com TMR0 para algumas
; V�lvulas, exceto V3 e V4 pois para V3 o registro CELSIUS faz a contagem de 100C� e para V4
; o registro PSI faz a contagem de 120psi, no final das contagens o sistema desaciona FLAGs 
; que vai determinar se TT & TP ser�o acionados, respectivamente. As outras vari�veis com o "_TEMP"
; s�o para armazenar/recuperar contextos dos dados durante a interrup��o de RB0/INT, Pino que � conectado
; no Bot�o STOP, isto porque o sistema � pausado, logo todos os registros precisam ser salvos antes
; deles serem apagados dentro da Interrup��o.
; **************************************************************************************************

#IFNDEF 	__REACTDATA_INC__
#DEFINE 	__REACTDATA_INC__	; Isto permite s� incluir se ainda n�o foi inclu�do


#define 	I0		PORTB	 ; I0 ser� as entradas de dados do sistema (Apenas RB0 e RB1)
#define 	Q0		PORTD	 ; Q0 ser� as 1� sa�das de dados do sistema
#define 	Q1 		PORTC	 ; Q1 ser� as 2� sa�das de dados do sistema

#define 	STOP 	I0,RB0	 ; STOP  -> PORTB,0 = Interrup��o (Bot�o) 
#define 	START   I0,RB1	 ; START -> PORTB,1 = Inicio  	  (Bot�o)

#define 	LH 		Q1,RC0   ; LH   -> PORTC,0 = Nivel Alto  (LED-GREEN)
#define 	LM 		Q1,RC1	 ; LM   -> PORTC,1 = Nivel M�dio (LED-RED)
#define 	LL 		Q1,RC2	 ; LL   -> PORTC,2 = Nivel Baixo (LED-YELLOW)
#define 	TT 		Q1,RC3	 ; TT   -> PORTC,3 = Temperatura (LED-BLUE)
#define 	TP 		Q1,RC4	 ; TP 	-> PORTC,4 = Press�o     (LED-YELLOW)
#define 	ST 		Q1,RC5	 ; ST 	-> PORTC,5 = Indica Reator Operando
#define 	FN 		Q1,RC6 	 ; FN 	-> PORTC,6 = Indica Reator Finalizando
#define 	PS 		Q1,RC7   ; PS 	-> PORTC,7 = Indica Reator Pausado

#define 	V1	 	Q0,RD0 	 ; V1   -> PORTD,0 = V�lvula 1 (Motor-DC)
#define 	V2		Q0,RD1	 ; V2   -> PORTD,1 = V�lvula 2 (Motor-DC)
#define 	V3		Q0,RD2   ; V3   -> PORTD,2 = V�lvula 3 (Motor-DC)
#define 	V4		Q0,RD3   ; V4   -> PORTD,3 = V�lvula 4 (Motor-DC)
#define 	V5		Q0,RD4   ; V5   -> PORTD,4 = V�lvula 5 (Motor-DC)
#define 	V6		Q0,RD5   ; V6   -> PORTD,5 = V�lvula 6 (Motor-DC)
#define 	MOT1	Q0,RD6   ; MOT1 -> PORTD,6 = Motor 1   (Motor-DC)
#define 	BOMB   	Q0,RD7   ; BOMB -> PORTD,7 = Bomba     (Motor-DC)	

#define 	C100 	FLAG1,0  ; Flag que indica 100 graus celsius de TT
#define 	T1	 	FLAG1,1	 ; Flag de Retardo de 1s entre V1 e V2
#define 	RT    	FLAG1,2	 ; Flag de Repouso de 3s para Temperatura TT
#define 	P1L 	FLAG1,3	 ; Flag do n�vel baixo do Produto1 para sensor LL
#define 	P1M 	FLAG1,4	 ; Flag do n�vel m�dio do Produto1 para sensor LM
#define 	P2H 	FLAG1,5	 ; Flag do n�vel alto do Produto2 para sensor LH
#define 	T12 	FLAG1,6	 ; Flag de Reten��o de 12s para MOT1
#define 	P3	 	FLAG1,7	 ; Flag de Reten��o de 3s para BOMBA do Produto3

#define 	LHON 	FLAG2,0  ; Flag que indica LH acionado
#define 	LMON	FLAG2,1  ; Flag que indica LM acionado
#define 	LLON 	FLAG2,2  ; Flag que indica LL acionado
#define 	RESC 	FLAG2,3  ; Flag que indica reservatorio secundario cheio
#define 	P120	FLAG2,4  ; Flag que indica 120 psi de press�o
#define 	EOP 	FLAG2,5  ; Flag que indica fim do programa

#define 	TIMER   PORTB,RB2	; Utilizado como sa�da para m�dulo timer de V2 no Proteus	 
#define 	RESETS 	PORTB,RB3	; Utilizado como sa�da para LED de RESET

CBLOCK 		H'20'			; Bloco da mem�ria de dados
	TEMP 					; Registro temporario p/ Instru��es booleanas
	W_TEMP					; Registro WORK temporario p/ Interrupcoes Gerais
	STATUS_TEMP				; Registro STATUS temporario p/ Interrupcoes Gerais
	PCLATH_TEMP				; Registro PCLATH temporario p/ Interrupcoes Gerais
	Q0_TEMP 				; Registro Q0 temporario (PORTD) p/ Interrup��o do STOP
	Q1_TEMP					; Registro Q1 temporario (PORTC) p/ Interrup��o do STOP
	FLAG1_TEMP				; Registro FLAG1 temporario (Para temporizadores) p/ Interrup��o do STOP
	FLAG2_TEMP				; Registro FLAG2 temporario (Para Acionamentos) p/ Interrup��o do STOP
	CELS_TEMP				; Registro contador CELSIUS temporario p/ Interrup��o do STOP
	PSI_TEMP				; Registro contador PSI temporario p/ Interrup��o do STOP
	TMR0_TEMP				; Registro TMR0 temporario p/ Interrup��o do STOP
	COUNT_TEMP				; Registro COUNT temporario p/ Interrup��o do STOP
	FLAG1 					; Registro de Flags para tempo e contagens
	FLAG2 					; Registro de Flags de acionamento
	COUNT 					; Contador de Tempo para valvulas (Exceto pra V3 e V4)
	CELSIUS					; Registro contador de Graus Celsius para TT
	PSI 					; Registro contador de Press�o Interna para TP
	DELAY1					; Registro para contagem de Delay na ISR p/ Interrup��o do STOP
	DELAY2					; Registro para contagem de Delay na ISR p/ Interrup��o do STOP
ENDC						; Fim do Bloco da mem�ria de dados

#ENDIF