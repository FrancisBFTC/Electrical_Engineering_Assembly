; **************************************************************************************************************														   *
; Assembly Microcontrolador PIC16F877A																		   *
;																											   *                                                                              *
; Disciplina: Microprocessadores & Microcontroladores                                                		   *
; Autor   : Wender Francis   			  Criação: 06/12/2021 	  Revisão: 07/12                               *
; Clock   : 4MHz 		  	              Arquivo: ReactMain.asm                                               *
; Programa: Sistema de Reator Industrial para despache da preparação de produtos                               *
;                                                                                                              *
;                                                                                                              *
; Descrição: Abaixo há um código de macros das equações booleanas do Reator. As operações estão                *
; abstraídas, pois todo o funcionamento é descrito por macros e definições nos arquivos de inclusão            *
; incluídas no "reactlib.inc", que por sua vez é incluída aqui. Neste código do MAIN há uma sequência          *
; lógica e organizada do que o reator deve fazer, as próprias macros são descritivas em relação a isto         *
; porém todo este sistema abaixo é executado por um loop, voltando novamente ao "Início das Operações          *
; do Reator", isto é feito até que o botão RESET seja pressionado pelo usuário, após isto o sistema            *
; é resetado, voltando a todas as configurações iniciais. O Botão RESET está conectado no MasterClear          *
; do Microcontrolador que reseta tudo quando identifica o nível lógico 0, o sistema inicial liga o LED         *
; vermelho ao lado do Botão quando é resetado no final ou indicando que o reator está ligado mas não           *
; está operando ainda e só irá operar quando for pressionado o Botão START.                                    *
;                                                                                                              *
; O START inicia o processo contínuo e automatizado do reator criando uma fonte de retenção que será           *
; utilizada nos últimos módulos. Esta fonte de retenção é indicada pelo LED verde ao lado do Botão START.      * 
; Já o botão STOP é conectado no pino RB0 que também funciona como interrupção externa, então isto é           *
; configurado nos registros para o sistema interromper as atividades e entrar num loop infinito dentro da      *
; ISR esperando que o usuário pressione o STOP novamente, quando novamente pressionado, o sistema volta        *
; a operar de onde parou. O LED amarelo ao lado dos botões, indica finalização do sistema, quando              *
; todas as atividades importantes foram feitas, é como se fosse um "aviso" de que o usuário poderá             *
; finalmente pressionar o RESET.                                                                               *
;                                                                                                              *
; Todas as atividades são automatizadas, isto é, durante a execução não será necessário fazer nada para que    *
; as operações aconteçam, desta forma estamos levando para um contexto real da aplicação pois na vida real     *
; tudo será feito automaticamente do início ao fim, a menos que seja implementado um sistema durante a         *
; interrupção do STOP, onde o usuário poderia operar manualmente algo que fosse eminente, por isso foi         *
; decidido utilizar LEDs nos sensores no lugar de Botões e Motores DC nas válvulas no lugar de LEDs, pois      *
; assim o contexto real da aplicação fica bem mais intuitivo. Os motores DC foram conectados em transistores,  *
; para garantir uma entrada de 5V nestes módulos e no circuito há 4 temporizadores para mostrar o tempo        *
; de operação das válvulas.                                                                                    *
; **************************************************************************************************************

; ---------------------------------------------------------------------------------------------------
; Inclusão da API do reator com todas as inclusões --------------------------------------------------
#include 	<reactlib.inc>						; API do reator - Todas as inclusões aqui
;#include 	<equations.inc>						; Inclua isto após __INI_REACTOR, apenas se precisar
												; depurar e comente as macros abaixo também
; ---------------------------------------------------------------------------------------------------

; ---------------------------------------------------------------------------------------------------
; Operações contínuas e automatizadas do Reator -----------------------------------------------------

__INI_REACTOR__									; Início das Operações do Reator (MAIN)
	__REACTOR_ON								; Ligue as operações do Reator
	__OPEN_V1_UNTIL_LM							; Abra a válvula V1 até que o sensor LM seja acionado
	__ACTIVATE_LL_WHEN_DETECTS_P1				; Acione o sensor LL quando detectar o P1
	__ACTIVATE_LM_WHEN_DETECTS_P1				; Acione o sensor LM quando detectar o P1
	__OPEN_V2_UNTIL_LH							; Abra a válvula V2 até que o sensor LH seja acionado
	__ACTIVATE_LH_WHEN_DETECTS_P2				; Acione o sensor LH quando detectar o P2
	__TURNS_ON_MOTOR_UNTIL_12S					; Ative o MOTOR 1 até chegar em 12 segundos
	__OPEN_V3_UNTIL_TT							; Abra a válvula V3 até que o sensor TT seja acionado
	__ACTIVATE_TT_WHEN_DETECTS_100C				; Acione o sensor TT quando detectar 100 Cº
	__OPEN_V5_WHEN_3S_UNTIL_LL_OFF				; Abra a válvula V5 após 3 segundos até LL desacionar
	__TURNS_OFF_LH_LM_LL_WHEN_V5				; Desacione LH,LM e LL enquanto V5 estiver ligado
	__OPEN_V4_UNTIL_TP							; Abra a válvula V4 até que o sensor TP seja acionado
	__ACTIVATE_TP_WHEN_DETECTS_120PSI			; Acione o sensor TP quando detectar 120 psi
	__TURNS_ON_BOMB_UNTIL_5S					; Ative a Bomba até chegar em 5 segundos
	__OPEN_V6_UNTIL_RESET						; Abra a válvula V6 até que o usuário pressione RESET
	__TURNS_OFF_OPERATION_STATE					; Desligue o LED de Estado de Operação
	__TURNS_ON_FINALIZATION_STATE				; Ligue o LED de Estado de Finalização
	__TURNS_OFF_RESET_STATE						; Desligue o LED de Estado de RESET
__END_REACTOR__									; FIM das Operações (LOOP - de volta ao MAIN)

; ---------------------------------------------------------------------------------------------------

END