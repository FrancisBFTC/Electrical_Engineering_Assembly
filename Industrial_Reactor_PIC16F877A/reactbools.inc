; ********************************************************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor   : Wender Francis   			  Criação: 06/12/2021 	  Revisão: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactbools.inc 
; Programa: Sistema de Reator Industrial para despache da preparação de produtos
;
; Descrição: Este é um arquivo de Macros de Equações booleanas, composta de 17 equações, cada uma
; com sua respectiva macro (Exceto __INI_REACTOR__ e __END_REACTOR__ que é a label e o retorno do Main). 
; Estas macros são incluídas no "reactlib.inc" e utilizadas no arquivo "ReactMain.asm".
; As operações de cada macro são descritas a seguir ->
;
; __INI_REACTOR__ -> Inicio das equações booleanas do reator
; __END_REACTOR__ -> Fim das equações booleanas do reator e re-execução.
; __REACTOR_ON -> Armazena a retenção elétrica do reator através do botão START.
; __OPEN_V1_UNTIL_LM -> Liga a válvula V1 para despejar o produto1 até o sensor de nível médio ser ativado.
; __OPEN_V2_UNTIL_LH -> Liga a válvula V2 (após 1s) para despejar o produto2 até o sensor de nível alto ser ativado.
; __ACTIVATE_LL_WHEN_DETECTS_P1 -> Aciona o sensor de nível baixo quando detectar o Produto 1.
; __ACTIVATE_LM_WHEN_DETECTS_P1 -> Aciona o sensor de nível médio quando detectar o Produto 1.
; __ACTIVATE_LH_WHEN_DETECTS_P2 -> Aciona o sensor de nível alto quando detectar o Produto 2.
; __TURNS_ON_MOTOR_UNTIL_12S -> Liga o motor 1 até que se passe 12 segundos,após isto desligue.
; __OPEN_V3_UNTIL_TT -> Liga a válvula V3 para aquecer até o sensor TT ser ativado,após isto desligue.
; __ACTIVATE_TT_WHEN_DETECTS_100C -> Aciona o sensor TT quando detectar 100 Graus Celsius.
; __OPEN_V5_WHEN_3S_UNTIL_LL_OFF -> Liga a válvula V5, quando passar 3 segundos do repouso, até LL ser desativado, após isto desligue.
; __TURNS_OFF_LH_LM_LL_WHEN_V5 -> Desative os sensores LH,LM e LL quando V5 estiver ligado, o produto esta sendo despejado no 2ª reservatório.
; __OPEN_V4_UNTIL_TP -> Liga a válvula V4 até que TP seja ativado,isto é, TP detectar 120psi de Pressão interna,após isto desligue.
; __ACTIVATE_TP_WHEN_DETECTS_120PSI -> Aciona o sensor TP quando detectar 120psi de pressão interna
; __TURNS_ON_BOMB_UNTIL_5S -> Liga a Bomba para despejar o Produto 3 no 2ª reservatório, até que se passe 5 segundos,após isto desligue.
; __OPEN_V6_UNTIL_RESET -> Liga a válvula V6 para despachar a mistura do 2ª reservatório até que o usuário pressione o Botão RESET.
; __TURNS_OFF_OPERATION_STATE -> Desligue o LED de Estado de Operação, indicando que todo o processo contínuo finalizou.
; __TURNS_ON_FINALIZATION_STATE -> Ligue o LED de Finalização dos processos e espere pelo RESET do usuário.
; __TURNS_OFF_RESET_STATE -> Desligue o LED reset após o pressionamento do START
; ********************************************************************************************************************************************

#IFNDEF 	__REACTBOOLS_INC__
#DEFINE 	__REACTBOOLS_INC__	; Isto permite só incluir se ainda não foi incluído

; Inicio das Equações do Reator
__INI_REACTOR__ macro
	R_MAIN:
ENDM

; Fim das Equações do Reator
__END_REACTOR__ macro
	goto 	R_MAIN
ENDM

; Para salvar o nível lógico do Botao START.
; Isto Indica que o reator esta ligado
__REACTOR_ON		macro
	IN 		START
	OR 		ST
	OUT 	ST		; ST = START+ST
ENDM
	
; Se START foi pressionado e o Reator ligou e LM e LH desacionados, 
; Abre V1 e Fecha V1 quando LM esta acionado
__OPEN_V1_UNTIL_LM 	macro
	IN 	 	START
	OR 	 	V1
	ANDN 	LH
	ANDN 	LM
	OUT 	V1   	; V1 = (START+V1).LH´.LM´
ENDM

	
; Se LM e LL estão acionados e LH,V1 e V5 desacionado
; Abre V2, Quando tempo chegar a 1 segundo (Quando Flag T1 é limpada).
; Tal que T1 desligado implica que V5 ligado.
__OPEN_V2_UNTIL_LH 	macro
	WHEN 	T1
	IN 	 	V2
	OR 	 	LM
	ANDN 	T1
	AND 	LL
	ANDN 	LH
	ANDN 	V1
	ANDN 	V5
	OUT 	V2   	; V2 = (V2+LM).T1´.LL.LH´.V1´.V5´ | T1´->V5
ENDM	
	
; Quando P1L ser 0 e LL estiver desligado ou V1 ligado
; Aciona o Nível Baixo LL.
; Tal que P1L desligado implica que LL ligado.
__ACTIVATE_LL_WHEN_DETECTS_P1 	macro
	WHEN 	P1L
	IN	 	LL
	OR 		V1
	ANDN 	P1L
	OR		LL
	OUT 	LL		; LL = (LL+V1).P1L´+LL | P1L´->LL
ENDM
	
; Quando P1M ser 0 e LM estiver desligado ou V1 ligado
; Aciona o Nível Médio LM, caso LL já foi acionado.
; Tal que P1M desligado implica que LM ligado.
__ACTIVATE_LM_WHEN_DETECTS_P1 	macro
	WHEN 	P1M
	IN	 	LM
	OR 		V1
	ANDN 	P1M
	AND 	LL
	OR 		LM
	OUT 	LM		; LM = (LM+V1).P1M´.LL+LM | P1M´->LM
ENDM
	
; Quando P2H já chegar no nível 0 e LH estiver desligado ou V2 ligado
; Aciona o Nível Alto LH, caso LM já foi acionado.
; Tal que P2H desligado implica que LH ligado.
__ACTIVATE_LH_WHEN_DETECTS_P2 	macro
	WHEN 	P2H
	IN	 	LH
	OR 		V2
	ANDN 	P2H
	AND 	LM
	OR 		LH
	OUT 	LH		; LH = (LH+V2).P2H´.LM+LH | P2H´->LH
ENDM


; Liga MOT1 após V2 abrir mas não desliga MOT1
; Se V2 fechar no momento que LH estiver acionado.
; Quando MOT1 estiver ligado, Tempo de 12s é acionado. 
; Quando Tempo T12 for 0, então MOT1 é desligado, ou seja,
; T12 ligado implica MOT1 ligado . Enquanto que V2 garante que 
; MOT1 seja ligado.
__TURNS_ON_MOTOR_UNTIL_12S	macro
	IN 		MOT1
	OR 		V2
	OUT 	MOT1	; MOT1 = MOT1+V2
	WHEN 	T12
	IN 		MOT1
	AND		T12
	OR 		V2
	OUT 	MOT1	; MOT1 = MOT1.T12+V2 | T12->MOT1
ENDM


; Se LH,LM e LL estiverem acionados e
; TT, MOT1 e V2 estiverem desligados então Liga V3
; até que TT seja acionado. Flag C100 deve estar ativada.
__OPEN_V3_UNTIL_TT 	macro
	IN 		TT
	ORN	 	MOT1
	AND		LL
	AND		LM
	AND		LH
	ANDN 	V2
	AND 	C100
	OUT		V3		; V3 = (TT+MOT1´).LL.LM.LH.V2´.C100
ENDM
	
; Se TT está desligado ou V3 ligado e V5 desligado, faça a 
; contagem de até 100 graus celsius, Quando C100 valer 0,
; significa que é 100 graus, então aciona TT. Tal que,
; C100 desligado implica que TT ligado.
__ACTIVATE_TT_WHEN_DETECTS_100C	macro
	WHEN 	C100
	IN	 	TT
	OR 		V3
	ANDN 	C100
	ANDN 	V5
	OUT 	TT		; TT = (TT+V3).C100´.V5´ | C100´->TT
ENDM
	
; Ligue V5 quando o Repouso de TT passar 3 segundos (RT valer 0)
; e LL estiver acionado mas se LL se tornar 0, desligue V5.
; Tal que, RT desligado implica que TT desligado implicando V5 ligado.
__OPEN_V5_WHEN_3S_UNTIL_LL_OFF	macro
	WHEN 	RT
	IN	 	V5
	OR 		TT
	ANDN 	RT
	AND 	LL
	OUT 	V5		; V5 = (V5+TT).RT´.LL | RT´->TT´->V5
ENDM
	
; Desligue LH,LM ou LL se LHON,LMON ou LLON estiverem desligados,
; respectivamente, isto é, quando V5 e o respectivo sensor estiver ligado
; e o produto do nível atual chegar a 0, Ex.: LM é igual a LM desligado
; quando LMON for desligado se, e somente se, V5 e LM estiver ligado e 
; P1M for 0. Isto indiscretamente ativa RESC.
__TURNS_OFF_LH_LM_LL_WHEN_V5 macro
	WHEN 	V5
	IN	 	LH
	AND 	LHON
	OUT 	LH		; LH = LH´->LHON´<->(V5.LH.P2H´) | LH=LH.LHON
	IN	 	LM
	AND 	LMON
	OUT 	LM		; LM = LM´->LMON´<->(V5.LM.P1M´) | LM=LM.LMON
	IN	 	LL
	AND 	LLON
	OUT 	LL		; LL = LL´->LLON´<->(V5.LL.P1L´) | LL=LL.LLON
ENDM
	
; Quando V4 estiver desligado ou LL desligado e RESC ligado (Reservatorio
; Secundario cheio), entao liga V4 até identificar a pressão de 120 psi,
; ou seja, Quando P120 chegar a 0 e TP ser acionado, tal que,
; P120 ligado implica que V4 ligado.
__OPEN_V4_UNTIL_TP 	macro
	WHEN 	P120
	IN 		V4
	ORN 	LL
	AND 	RESC
	OR 		P120
	OUT 	V4		; V4 = ((V4+LL´).RESC+P120) | P120->V4
ENDM
	
; ST se manten ligado para ativar TP caso a pressão chegou a 120 psi,
; mas só ativará também se V1,LL,V5,V4 e P120 estiverem desligados, ou seja,
; O unico momento plausível para TP ser acionado. Pois no início, Quando ST
; liga, V1 já liga, logo TP nunca é acionado até o momento certo.
__ACTIVATE_TP_WHEN_DETECTS_120PSI macro
	IN	 	ST
	ANDN 	V1
	ANDN 	LL
	ANDN 	V5
	ANDN	V4
	ANDN 	P120
	OUT 	TP		; TP = ST.V1´.LL´.V5´.V4´.P120´
ENDM

; Liga a Bomba se TP estiver ligado.
; Quando a Bomba estiver ligada, liga P3, despeje o Produto P3 até que
; Passe 5s, quando terminar, P3 vale 0 e se for 0, desligue a Bomba.
; Tal que, TP ligada implica P3 ligada que implica Bomba ligada.
__TURNS_ON_BOMB_UNTIL_5S macro
	IN 		BOMB
	OR 		TP
	OUT 	BOMB	; BOMB = (BOMB+TP)	
	WHEN 	BOMB
	IN 		BOMB
	AND		P3
	OUT 	BOMB	; BOMB = BOMB.P3 | TP->P3->BOMB
ENDM

; V6 é a última condição e só liga com 1 condição: Se ST estiver desligado
; Pela primeira vez e EOP for ligado. EOP é uma Flag reservada a V6, que
; significa "END OF PROGRAM", setada após a Bomba. 
; Agora se espera o pressionamento do Botão RESET.
; OBS.: O Botão de RESET é ligado no MasterClear, então automaticamente ao pressionar,
; O Botão envia o nível lógico 0 para o pino MCLR, Reiniciando o sistema.
__OPEN_V6_UNTIL_RESET macro
	IN 		V6		
	ORN 	ST
	AND 	EOP
	OR 		V6
	OUT 	V6		; V6 = ST´.EOP+V6
ENDM

; Condição necessária para Retenção de desligamento da Bomba, pois TP sempre
; Liga a Bomba e TP só é ativo se ST esta ativo, logo ST só estará ativo em 
; todas as válvulas antes de V6 (Exceto V2 pois MOT1 já faz o papel). ST 
; simboliza o Estado de inicialização e execução do reator.
__TURNS_OFF_OPERATION_STATE  macro
	IN 		TP
	AND		BOMB
	OR 		V5
	OR		V4
	OR 		V3
	OR 		V1
	OR 		MOT1
	OR		TT
	OUT 	ST		; ST = TP.BOMB+V5+V4+V3+V2+MOT1+TT
ENDM

; Logo 	ST desligado e V6 ligado, dar margens pra acionar o LED de finalização FN
; (Ao lado do Botão START), O LED amarelo é ligado para indicar que o programa
; Esta na sua fase final e o usuário já pode pressionar o Botão de RESET.
__TURNS_ON_FINALIZATION_STATE macro
	IN	 	FN
	ORN		ST
	AND 	V6
	OR 		FN
	OUT 	FN		; FN = FN+ST´.V6+FN
ENDM

; Desligue o LED vermelho de RESET após START ser pressionado.
; No Vetor de RESET, ou sistema foi iniciado ou foi reiniciado, logo
; o led vermelho irá acender nestes dois momentos.
__TURNS_OFF_RESET_STATE  macro
	IN 		RESETS
	OR 		RESETS
	ANDN 	ST
	OUT 	RESETS	; RESETS = RESETS.ST´
ENDM


#ENDIF