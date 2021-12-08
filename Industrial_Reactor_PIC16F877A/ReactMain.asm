; **************************************************************************************************************														   *
; Assembly Microcontrolador PIC16F877A																		   *
;																											   *                                                                              *
; Disciplina: Microprocessadores & Microcontroladores                                                		   *
; Autor   : Wender Francis   			  Cria��o: 06/12/2021 	  Revis�o: 07/12                               *
; Clock   : 4MHz 		  	              Arquivo: ReactMain.asm                                               *
; Programa: Sistema de Reator Industrial para despache da prepara��o de produtos                               *
;                                                                                                              *
;                                                                                                              *
; Descri��o: Abaixo h� um c�digo de macros das equa��es booleanas do Reator. As opera��es est�o                *
; abstra�das, pois todo o funcionamento � descrito por macros e defini��es nos arquivos de inclus�o            *
; inclu�das no "reactlib.inc", que por sua vez � inclu�da aqui. Neste c�digo do MAIN h� uma sequ�ncia          *
; l�gica e organizada do que o reator deve fazer, as pr�prias macros s�o descritivas em rela��o a isto         *
; por�m todo este sistema abaixo � executado por um loop, voltando novamente ao "In�cio das Opera��es          *
; do Reator", isto � feito at� que o bot�o RESET seja pressionado pelo usu�rio, ap�s isto o sistema            *
; � resetado, voltando a todas as configura��es iniciais. O Bot�o RESET est� conectado no MasterClear          *
; do Microcontrolador que reseta tudo quando identifica o n�vel l�gico 0, o sistema inicial liga o LED         *
; vermelho ao lado do Bot�o quando � resetado no final ou indicando que o reator est� ligado mas n�o           *
; est� operando ainda e s� ir� operar quando for pressionado o Bot�o START.                                    *
;                                                                                                              *
; O START inicia o processo cont�nuo e automatizado do reator criando uma fonte de reten��o que ser�           *
; utilizada nos �ltimos m�dulos. Esta fonte de reten��o � indicada pelo LED verde ao lado do Bot�o START.      * 
; J� o bot�o STOP � conectado no pino RB0 que tamb�m funciona como interrup��o externa, ent�o isto �           *
; configurado nos registros para o sistema interromper as atividades e entrar num loop infinito dentro da      *
; ISR esperando que o usu�rio pressione o STOP novamente, quando novamente pressionado, o sistema volta        *
; a operar de onde parou. O LED amarelo ao lado dos bot�es, indica finaliza��o do sistema, quando              *
; todas as atividades importantes foram feitas, � como se fosse um "aviso" de que o usu�rio poder�             *
; finalmente pressionar o RESET.                                                                               *
;                                                                                                              *
; Todas as atividades s�o automatizadas, isto �, durante a execu��o n�o ser� necess�rio fazer nada para que    *
; as opera��es aconte�am, desta forma estamos levando para um contexto real da aplica��o pois na vida real     *
; tudo ser� feito automaticamente do in�cio ao fim, a menos que seja implementado um sistema durante a         *
; interrup��o do STOP, onde o usu�rio poderia operar manualmente algo que fosse eminente, por isso foi         *
; decidido utilizar LEDs nos sensores no lugar de Bot�es e Motores DC nas v�lvulas no lugar de LEDs, pois      *
; assim o contexto real da aplica��o fica bem mais intuitivo. Os motores DC foram conectados em transistores,  *
; para garantir uma entrada de 5V nestes m�dulos e no circuito h� 4 temporizadores para mostrar o tempo        *
; de opera��o das v�lvulas.                                                                                    *
; **************************************************************************************************************

; ---------------------------------------------------------------------------------------------------
; Inclus�o da API do reator com todas as inclus�es --------------------------------------------------
#include 	<reactlib.inc>						; API do reator - Todas as inclus�es aqui
;#include 	<equations.inc>						; Inclua isto ap�s __INI_REACTOR, apenas se precisar
												; depurar e comente as macros abaixo tamb�m
; ---------------------------------------------------------------------------------------------------

; ---------------------------------------------------------------------------------------------------
; Opera��es cont�nuas e automatizadas do Reator -----------------------------------------------------

__INI_REACTOR__									; In�cio das Opera��es do Reator (MAIN)
	__REACTOR_ON								; Ligue as opera��es do Reator
	__OPEN_V1_UNTIL_LM							; Abra a v�lvula V1 at� que o sensor LM seja acionado
	__ACTIVATE_LL_WHEN_DETECTS_P1				; Acione o sensor LL quando detectar o P1
	__ACTIVATE_LM_WHEN_DETECTS_P1				; Acione o sensor LM quando detectar o P1
	__OPEN_V2_UNTIL_LH							; Abra a v�lvula V2 at� que o sensor LH seja acionado
	__ACTIVATE_LH_WHEN_DETECTS_P2				; Acione o sensor LH quando detectar o P2
	__TURNS_ON_MOTOR_UNTIL_12S					; Ative o MOTOR 1 at� chegar em 12 segundos
	__OPEN_V3_UNTIL_TT							; Abra a v�lvula V3 at� que o sensor TT seja acionado
	__ACTIVATE_TT_WHEN_DETECTS_100C				; Acione o sensor TT quando detectar 100 C�
	__OPEN_V5_WHEN_3S_UNTIL_LL_OFF				; Abra a v�lvula V5 ap�s 3 segundos at� LL desacionar
	__TURNS_OFF_LH_LM_LL_WHEN_V5				; Desacione LH,LM e LL enquanto V5 estiver ligado
	__OPEN_V4_UNTIL_TP							; Abra a v�lvula V4 at� que o sensor TP seja acionado
	__ACTIVATE_TP_WHEN_DETECTS_120PSI			; Acione o sensor TP quando detectar 120 psi
	__TURNS_ON_BOMB_UNTIL_5S					; Ative a Bomba at� chegar em 5 segundos
	__OPEN_V6_UNTIL_RESET						; Abra a v�lvula V6 at� que o usu�rio pressione RESET
	__TURNS_OFF_OPERATION_STATE					; Desligue o LED de Estado de Opera��o
	__TURNS_ON_FINALIZATION_STATE				; Ligue o LED de Estado de Finaliza��o
	__TURNS_OFF_RESET_STATE						; Desligue o LED de Estado de RESET
__END_REACTOR__									; FIM das Opera��es (LOOP - de volta ao MAIN)

; ---------------------------------------------------------------------------------------------------

END