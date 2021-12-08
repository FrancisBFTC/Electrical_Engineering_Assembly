; *****************************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor   : Wender Francis   			  Criação: 06/12/2021 	  Revisão: 07/12  
; Clock   : 4MHz 		  	              Arquivo: equations.inc 
; Programa: Sistema de Reator Industrial para despache da preparação de produtos
;
;
; Este é um arquivo de equações para depuração. Existe um outro arquivo principal que é
; chamado no ReactMain.asm que é o arquivo reactbools.inc, e são as macros dele que são
; chamadas no Main, elas estão encapsuladas em macros, então se torna algo meio complicado
; pra depuração porém a organização é maior e o contexto abstrato se torna mais compreensível
; ao se deparar com a sequência de macros. Mas de toda maneira, deixei aqui um arquivo extra 
; que é o equations.inc. Eles contém as mesmas equações que as do reactbools.inc porém de forma 
; direta, sem encapsulamento. Pra depurar no MPLAB passo a passo apenas sigas estas instruções abaixo:
;
; 1. Comente todas as chamadas de Macro no MAIN entre __INI_REACTOR__ e _END_REACTOR__, mas não comente estas duas.
; 2. Descomente a inclusão do equation.inc no MAIN e copie ela para abaixo do __INI_REACTOR__.
; 3. No arquivo reactmacr.inc, altere todos os DECIMAIS da Macro WHEN, sendo TMR0=255 e COUNT=1, PSI=1 e CELSIUS=1.
; 4. Os registros principais para depuração no Watch são: TMR0,PORTC,PORTD,WREG,COUNT,CELSIUS,PSI,FLAG1 e FLAG2.
;
; Obs.: A instrução N.3 é pelo motivo de não ser possível dar breakpoints dentro do Loop e a utilização de Macros
; dificulta isso mais ainda, pois a intenção seria depurar a temporização e isto ocorre milhões de vezes, o que é
; inviável depurar passo a passo (sem a chance de dar breakpoints), então TMR0 sendo 255 e os contadores sendo 1,
; apenas uma execução em sequência te levará para a interrupção e poderá ver o comportamento tanto da interrupção
; quanto da ativação de todos os módulos no tempo definido. Mas se for fazer isto, não se esqueça de voltar aos
; valores originais segurando CTRL+Z no reactmacr.inc e ReactMain.asm e remontando antes mesmo de executar no Proteus.
; ******************************************************************************************************************

#IFNDEF 	__EQUATIONS_INC__
#DEFINE 	__EQUATIONS_INC__	; Isto permite só incluir se ainda não foi incluído

	IN 		START
	OR 		ST
	OUT 	ST		; ST = START+ST
	
	IN 	 	START
	OR 	 	V1
	ANDN 	LH
	ANDN 	LM
	OUT 	V1   	; V1 = (START+V1).LH´.LM´
	
	WHEN 	T1
	IN 	 	V2
	OR 	 	LM
	ANDN 	T1
	AND 	LL
	ANDN 	LH
	ANDN 	V1
	ANDN 	V5
	OUT 	V2   	; V2 = (V2+LM).T1´.LL.LH´.V1´.V5´ | T1´->V5
	
	WHEN 	P1L
	IN	 	LL
	OR 		V1
	ANDN 	P1L
	OR		LL
	OUT 	LL		; LL = (LL+V1).P1L´+LL | P1L´->LL
	
	WHEN 	P1M
	IN	 	LM
	OR 		V1
	ANDN 	P1M
	AND 	LL
	OR 		LM
	OUT 	LM		; LM = (LM+V1).P1M´.LL+LM | P1M´->LM
	
	WHEN 	P2H
	IN	 	LH
	OR 		V2
	ANDN 	P2H
	AND 	LM
	OR 		LH
	OUT 	LH		; LH = (LH+V2).P2H´.LM+LH | P2H´->LH
	
	IN 		MOT1
	OR 		V2
	OUT 	MOT1	; MOT1 = MOT1+V2
	WHEN 	T12
	IN 		MOT1
	AND		T12
	OR 		V2
	OUT 	MOT1	; MOT1 = MOT1.T12+V2 | T12->MOT1
	
	IN 		TT
	ORN	 	MOT1
	AND		LL
	AND		LM
	AND		LH
	ANDN 	V2
	AND 	C100
	OUT		V3		; V3 = (TT+MOT1´).LL.LM.LH.V2´.C100
	
	WHEN 	C100
	IN	 	TT
	OR 		V3
	ANDN 	C100
	ANDN 	V5
	OUT 	TT		; TT = (TT+V3).C100´.V5´ | C100´->TT
	
	WHEN 	RT
	IN	 	V5
	OR 		TT
	ANDN 	RT
	AND 	LL
	OUT 	V5		; V5 = (V5+TT).RT´.LL | RT´->TT´->V5
	
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
	
	WHEN 	P120
	IN 		V4
	ORN 	LL
	AND 	RESC
	OR 		P120
	OUT 	V4		; V4 = ((V4+LL´).RESC+P120) | P120->V4
	
	IN	 	ST
	ANDN 	V1
	ANDN 	LL
	ANDN 	V5
	ANDN	V4
	ANDN 	P120
	OUT 	TP		; TP = ST.V1´.LL´.V5´.V4´.P120´
	
	IN 		BOMB
	OR 		TP
	OUT 	BOMB	; BOMB = (BOMB+TP)	
	WHEN 	BOMB
	IN 		BOMB
	AND		P3
	OUT 	BOMB	; BOMB = BOMB.P3 | TP->P3->BOMB
	
	IN 		V6		
	ORN 	ST
	AND 	EOP
	OR 		V6
	OUT 	V6		; V6 = ST´.EOP+V6
	
	IN 		TP
	AND		BOMB
	OR 		V5
	OR		V4
	OR 		V3
	OR 		V1
	OR 		MOT1
	OR		TT
	OUT 	ST		; ST = TP.BOMB+V5+V4+V3+V2+MOT1+TT
	
	IN	 	FN
	ORN		ST
	AND 	V6
	OR 		FN
	OUT 	FN		; FN = FN+ST´.V6+FN

	IN 		RESETS
	OR 		RESETS
	ANDN 	ST
	OUT 	RESETS	; RESETS = RESETS.ST´

#ENDIF
	