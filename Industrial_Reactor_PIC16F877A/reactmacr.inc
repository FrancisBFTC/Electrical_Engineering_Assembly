; **************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor   : Wender Francis  			  Cria��o: 06/12/2021 	  Revis�o: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactmacr.inc 
; Programa: Sistema de Reator Industrial para despache da prepara��o de produtos
;
;
; Descri��o: Este � um arquivo de Macros para sele��o de bancos, salvamento de contextos durante
; a interrup��o e opera��es l�gicas OR, AND, XOR, NOT, ORN, ANDN, XORN e as opera��es auxiliares 
; IN e OUT. IN � a entrada do dado (INPUT) que serve para carregar um valor l�gico inicial 0 ou 1
; de um m�dulo espec�fico e OUT � a sa�da do dado (OUTPUT) que serve para enviar o resultado final
; para um m�dulo espec�fico. O OUT de fato vai alterar o estado do m�dulo, tornando-o ativo ou inativo.
; J� a opera��o WHEN � algo novo, pois ela simboliza o "tempo de opera��o" de um m�dulo. O WHEN 
; Opera em 10 m�dulos diferentes, sendo eles: LL,LM,LH,V2,MOT1,V3,TT,V5,V4 e BOMB. Os m�dulos com tempo
; de reten��o de acionamento s�o MOT1 e BOMB, J� de V2 � o tempo de retardo para ser ligado ap�s o 
; o desligamento de V1, por�m tem os tempos adicionais utilizados em outros m�dulos, como LL,LM e LH. 
; Entre o acionamento destes sensores, existe um intervalo de tempo para simbolizar que os produtos est�o 
; sendo despejados, vamos dizer que num contexto real, o "l�quido" do produto tem um tempo natural para 
; encher o reservat�rio at� chegar naquele n�vel, ent�o eu presumir este tempo atrav�s do WHEN. V3 � 
; respons�vel por aquecer o reservat�rio at� 100 graus e V4 por dar a press�o interna de 120psi, ent�o tamb�m 
; presumir que tanto a temperatura como a press�o, poderiam ser determinados por um timer. V5 faz o processo 
; inverso de V1 e V2, enquanto que V1 e V2 "aciona" os sensores de baixo para cima (Enchendo o reservat�rio), V5 
; "desaciona" os sensores de cima para baixo, ou seja, despachando a mistura do 1� reservat�rio para
; o 2� reservat�rio, portanto V5 tamb�m ter� o mesmo tempo que V1 e V2 teve para despachar o l�quido.
; Os �nicos m�dulos que n�o tem um timer definido � V1 e V6, porque V1 n�o depende definitivamente do Tempo
; mas sim, do sensor LM. Enquanto que V6 tamb�m n�o depende do tempo mas sim, do usu�rio. V6 fica ligado
; at� que o usu�rio pressione RESET, ap�s isto, todo o sistema � reiniciado, esperando novamente que
; o usu�rio pressione START para recome�ar tudo.
; **************************************************************************************************

#IFNDEF 	__REACTMACR_INC__
#DEFINE 	__REACTMACR_INC__	; Isto permite s� incluir se ainda n�o foi inclu�do


; --- Macro para sele��o de bancos de memoria ------------------------------------------------------
; --------------------------------------------------------------------------------------------------
BANK_SELECT   macro  	num		; Nome da Macro
	IF 	num==0					; Argumento � igual ao Banco 0?
		bcf 	STATUS,RP0		; Sim, limpe o bit RP0 de STATUS
		bcf 	STATUS,RP1		; Sim, limpe o bit RP1 de STATUS
	ENDIF
	IF 	num==1					; Argumento � igual ao Banco 1?
		bsf 	STATUS,RP0		; Sim, sete o bit RP0 de STATUS
		bcf 	STATUS,RP1		; Sim, limpe o bit RP1 de STATUS
	ENDIF
	IF 	num==2					; Argumento � igual ao Banco 2?
		bcf 	STATUS,RP0		; Sim, limpe o bit RP0 de STATUS
		bsf 	STATUS,RP1		; Sim, sete o bit RP1 de STATUS
	ENDIF
	IF 	num==3					; Argumento � igual ao Banco 3?
		bsf 	STATUS,RP0		; Sim, sete o bit RP0 de STATUS
		bsf 	STATUS,RP1		; Sim, sete o bit RP1 de STATUS
	ENDIF
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Macro para Armazenamento de Contextos --------------------------------------------------------
; --------------------------------------------------------------------------------------------------
STORE_CONTEXT 	macro 	 	 	; Macro STORE_CONTEXT (0 argumento)
	movwf    W_TEMP				; Salve W em W_TEMP 
	swapf    STATUS,W      		; Fa�a uma troca de Nibbles de STATUS, mova para W
	clrf     STATUS   			; Limpe o conte�do de STATUS
	movwf    STATUS_TEMP   		; Salve W em STATUS_TEMP
	movf     PCLATH, W   		; Mova PCLATH para W
	movwf    PCLATH_TEMP   		; Salve W em PCLATH_TEMP
	clrf     PCLATH      		; Limpe o conte�do de PCLATH
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Macro para Recupera��o de Contextos ----------------------------------------------------------
; --------------------------------------------------------------------------------------------------
RESTORE_CONTEXT 	macro 	  	; Macro RESTORE_CONTEXT (0 argumento)
IEND:						  	; Label para fim da interrupcao
	movf     PCLATH_TEMP, W   	; Mova PCLATH_TEMP para W
	movwf    PCLATH      	  	; Recupere o conte�do de PCLATH
	swapf    STATUS_TEMP,W    	; Fa�a uma troca de Nibbles de STATUS_TEMP, mova para W   
	movwf    STATUS      	  	; Recupere o conte�do de STATUS
	swapf    W_TEMP,F 		  	; Fa�a uma troca de Nibbles de W_TEMP, salva nele mesmo
	swapf    W_TEMP,W    	  	; Fa�a uma troca de Nibbles de W_TEMP, recupera W
ENDM						  	; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Entrada de um dado l�gico --------------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
IN		macro	reg,bit			; Macro IN: Argumentos reg e bit
	clrw						; Limpe o conteudo de W -> W = 0
	btfsc 	reg,bit				; O BIT 'bit' do registro 'reg' � 0?
	movlw 	H'01'				; N�o, Salve 1 em W
	nop							; Sim, W permanece em 0
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Carrega um dado l�gico negado ----------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
NOT		macro	reg,bit			; Macro NOT: Argumentos reg e bit
	clrw						; Limpe o conte�do de W -> W = 0
	btfss 	reg,bit				; O BIT 'bit' do registro 'reg' � 1?
	movlw 	H'01'				; N�o, Muda 0 para 1 em W
	nop							; Sim, W permanece em 0
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Opera��o OR com o dado em TEMP ---------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
OR		macro	reg,bit			; Macro OR: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfsc 	reg,bit				; O dado atual � 0?
	movlw 	H'01'				; N�o, Ent�o W ser� 1
	iorwf 	TEMP,W				; Sim, Salva em W um OR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Opera��o OR negado com o dado em TEMP --------------------------------------------------------
; --------------------------------------------------------------------------------------------------
ORN		macro	reg,bit			; Macro ORN: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfss 	reg,bit				; O dado atual � 1?
	movlw 	H'01'				; N�o, Ent�o W ser� 1
	iorwf 	TEMP,W				; Sim, Salva em W um OR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Opera��o AND com o dado em TEMP --------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
AND		macro	reg,bit			; Macro AND: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfsc 	reg,bit				; O dado atual � 0?
	movlw 	H'01'				; N�o, Ent�o W ser� 1
	andwf 	TEMP,W				; Sim, Salva em W um AND de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Opera��o AND negado com o dado em TEMP -------------------------------------------------------
; --------------------------------------------------------------------------------------------------
ANDN	macro	reg,bit			; Macro ANDN: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfss 	reg,bit				; O dado atual � 1?
	movlw 	H'01'				; N�o, Ent�o W ser� 1
	andwf 	TEMP,W				; Sim, Salva em W um AND de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Opera��o XOR com o dado em TEMP --------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
XOR	    macro   reg,bit			; Macro XOR: Argumentos reg e bit
	movwf	TEMP				; Salva o dado anterior em TEMP
	clrw 						; Limpa o W, W = 0
	btfsc   reg,bit				; O dado atual � 0?
	movlw	H'01'				; N�o, Ent�o W ser� 1
	xorwf	TEMP,W 				; Sim, Salva em W um XOR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Opera��o XOR negada com o dado em TEMP -------------------------------------------------------
; --------------------------------------------------------------------------------------------------
XORN	  macro   reg,bit		; Macro XORN: Argumentos reg e bit
	movwf	TEMP				; Salva o dado anterior em TEMP
	clrw 						; Limpa o W, W = 0
	btfss   reg,bit				; O dado atual � 1?
	movlw	H'01'				; N�o, Ent�o W ser� 1
	xorwf	TEMP,W 				; Sim, Salva em W um XOR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Sa�da de um resultado no bit do registro -----------------------------------------------------
; --------------------------------------------------------------------------------------------------
OUT 	macro 	reg,bit			; Macro OUT: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	andlw 	H'01'				; Fa�a um AND de W com 1
	btfss 	STATUS,Z			; O resultado do AND deu 0?
	goto 	$ + 3				; N�o, Ent�o vai para a 3� instru��o ap�s esta linha
	bcf 	reg,bit				; Sim, Ent�o limpe o BIT 'bit' do registro 'reg'
	goto 	$ + 2				; Vai para a 2� instru��o ap�s esta linha porque o AND deu 0
	bsf 	reg,bit				; Sete o BIT 'bit' do registro 'reg' porque no AND deu 1
	movf	TEMP,W				; Salve o dado anterior de TEMP para W
ENDM							; Fim da macro
; --------------------------------------------------------------------------------------------------


; --- Macro para tempo de opera��o -----------------------------------------------------------------
; Esta macro com o seu nome 'WHEN' exprime uma no��o de "tempo", pois � quando algo ocorre. Ela faz
; exatamente isto, ela � uma condi��o para um ou dois m�dulos : se este(s) m�dulo(s) est� ligado ou 
; desligado, a responsabilidade do WHEN � iniciar uma temporiza��o, setando uma FLAG espec�fica para
; aquele m�dulo dizendo que a contagem de tempo est� ativa, quando o tempo finaliza, a FLAG que foi
; setada � agora limpada, onde o valor da FLAG ser� utilizada para determinar o resultado do m�dulo.
; Ap�s a FLAG ser limpada, o temporizador fica inativo.
; --------------------------------------------------------------------------------------------------
WHEN   macro 	reg,bit					; Macro WHEN: Argumentos reg e bit

	; Retardo de 1 segundo para ligar V2
	if 	reg==FLAG1 & bit==1				; Se argumento for a FLAG T1 ent�o fa�a o c�digo abaixo...
			btfsc 	LH					; Se LH estiver inativo, salte 1 instru��o
			goto 	ENDV1				; N�o inicie o temporizador, Vai para o fim
			btfss 	LM					; Se LM estiver ativo, salte 1 instru��o
			goto 	ENDV1				; N�o inicie o temporizador, Vai para o fim
			btfsc 	V1					; Se V1 estiver desligado salte 1 instru��o
			goto 	ENDV1				; N�o inicie o temporizador, Vai para o fim
			btfsc 	V2					; Se V2 estiver desligado salte 1 instru��o
			goto 	ENDV1				; N�o inicie o temporizador, Vai para o fim	
			btfsc 	V5					; Se V5 estiver desligado salte 1 instru��o
			goto 	ENDV1				; N�o inicie o temporizador, Vai para o fim			
			btfsc 	T1					; Se T1 estiver inativo, inicie o temporizador
			goto 	ENDV10				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'11'				; Mova o valor 11 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 11 at� 255 (245 contagens)
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador
			bsf 	T1					; Ative a Flag T1 pra indicar que a temporiza��o est� ativa
			bsf 	TIMER 				; Ligue o display de tempo do Proteus
		ENDV10:							; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'16'				; Mova o valor 16 para W
			xorwf 	COUNT,W				; Compare COUNT com 16 (COUNT vai de 0 a 16)
			btfss 	STATUS,Z			; Z=1? ou 16 xor COUNT = 0? Se sim, significa que COUNT=16
			goto 	ENDV1				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim
			bcf 	TIMER 				; Desligue o display de tempo do Proteus
			bcf 	T1					; Se sim, ent�o desative a Flag T1
			clrf 	COUNT				; Zere o conte�do de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto
		ENDV1:							; Fim da Execu��o
	endif								; Fim da Condi��o

	; Tempo de 2,2 segundos para ativar LL (Tempo imagin�rio de enchimento)
	if 	reg==FLAG1 & bit==3				; Se argumento for a FLAG P1L ent�o fa�a o c�digo abaixo...
			btfsc 	LL					; Se LL estiver inativo salte 1 instru��o
			goto 	ENDLL				; N�o inicie o temporizador, Vai para o fim
			btfss 	V1					; Se V1 estiver ativo, salte 1 instru��o
			goto 	ENDLL				; N�o inicie o temporizador, Vai para o fim
			btfsc 	P1L					; Se P1L estiver inativo, inicie o temporizador
			goto 	ENDLL0				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 6 at� 255 (250 contagens)
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador
			bsf 	P1L					; Ative a Flag P1L pra indicar que a temporiza��o est� ativa
		ENDLL0:							; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'35'				; Mova o valor 35 para W
			xorwf 	COUNT,W				; Compare COUNT com 35 (COUNT vai de 0 a 35)
			btfss 	STATUS,Z			; Z=1? ou 35 xor COUNT = 0? Se sim, significa que COUNT=35
			goto 	ENDLL				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim
			bcf 	P1L					; Se sim, ent�o desative a Flag P1L
			bsf 	LLON				; Ative a Flag LLON pra indicar que LL est� ativo
			clrf 	COUNT				; Zere o conte�do de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto
		ENDLL:							; Fim da Execu��o
	endif								; Fim da Condi��o
	
	; Tempo de 2,2 segundos para ativar LM (Tempo imagin�rio de enchimento)
	if 	reg==FLAG1 & bit==4				; Se argumento for a FLAG P1M ent�o fa�a o c�digo abaixo...
			btfsc 	LM					; Se LM estiver inativo salte 1 instru��o
			goto 	ENDLM				; N�o inicie o temporizador, Vai para o fim
			btfss 	LL					; Se LL estiver ativo, salte 1 instru��o
			goto 	ENDLM				; N�o inicie o temporizador, Vai para o fim
			btfss 	V1					; Se V1 estiver ativo, salte 1 instru��o
			goto 	ENDLM				; N�o inicie o temporizador, Vai para o fim
			btfsc 	P1M					; Se P1M estiver inativo, inicie o temporizador
			goto 	ENDLM0				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W	
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 6 at� 255 (250 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador 	
			bsf 	P1M					; Ative a Flag P1M pra indicar que a temporiza��o est� ativa
		ENDLM0:							; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'35'				; Mova o valor 35 para W	
			xorwf 	COUNT,W				; Compare COUNT com 35 (COUNT vai de 0 a 35)		
			btfss 	STATUS,Z			; Z=1? ou 35 xor COUNT = 0? Se sim, significa que COUNT=35	
			goto 	ENDLM				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim	
			bcf 	P1M					; Se sim, ent�o desative a Flag P1M 	
			bsf 	LMON				; Ative a Flag LMON pra indicar que LM est� ativo
			clrf 	COUNT				; Zere o conte�do de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto 
		ENDLM:							; Fim da Execu��o
	endif								; Fim da Condi��o

	; Tempo de 2,2 segundos para ativar LH
	if 	reg==FLAG1 & bit==5				; Se argumento for a FLAG P1H ent�o fa�a o c�digo abaixo...
			btfsc 	LH					; Se LH estiver inativo salte 1 instru��o
			goto 	ENDLH				; N�o execute o temporizador, Vai para o fim
			btfss 	LM					; Se LM estiver ativo, salte 1 instru��o
			goto 	ENDLH				; N�o execute o temporizador, Vai para o fim
			btfss 	V2					; Se V2 estiver ativo, salte 1 instru��o
			goto 	ENDLH				; N�o execute o temporizador, Vai para o fim
			btfsc 	P2H					; Se P1H estiver inativo, inicie o temporizador
			goto 	ENDLH0				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 6 at� 255 (250 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador 
			bsf 	P2H					; Ative a Flag P2H pra indicar que a temporiza��o est� ativa
		ENDLH0:							; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'35'				; Mova o valor 35 para W	
			xorwf 	COUNT,W				; Compare COUNT com 35 (COUNT vai de 0 a 35)		
			btfss 	STATUS,Z			; Z=1? ou 35 xor COUNT = 0? Se sim, significa que COUNT=35
			goto 	ENDLH				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim	
			bcf 	P2H					; Se sim, ent�o desative a Flag P2H  
			bsf 	LHON				; Ative a Flag LHON pra indicar que LH est� ativo
			clrf 	COUNT				; Zere o conte�do de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto 	
		ENDLH:							; Fim da Execu��o
	endif								; Fim da Condi��o
	
	; Tempo de reten��o de 12 segundos para desligar MOT1
	if 	reg==FLAG1 & bit==6				; Se argumento for a FLAG T12 ent�o fa�a o c�digo abaixo...		
			btfss   MOT1				; Se MOT1 estiver ligado salte 1 instru��o	
			goto 	MOT1OFF				; Se n�o, N�o execute o temporizador, Vai para o fim 
			btfss 	LH					; Se LH estiver ativo salte 1 instru��o
			goto 	MOT1OFF				; Se n�o, N�o execute o temporizador, Vai para o fim
			btfss 	LM					; Se LM estiver ativo, salte 1 instru��o
			goto 	MOT1OFF				; N�o execute o temporizador, Vai para o fim
			btfsc 	T12					; Se T12 estiver inativo, inicie o temporizador		
			goto 	COUNTER				; Se j� estiver ativo, s� verifique se j� chegou no tempo.		
			movlw 	D'6'				; Mova o valor 6 para W			
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 6 at� 255 (250 contagens)	
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador  
			bsf 	T12					; Ative a Flag T12 pra indicar que a temporiza��o est� ativa
		COUNTER:						; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'153'				; Mova o valor 153 para W	
			xorwf 	COUNT,W				; Compare COUNT com 153 (COUNT vai de 0 a 153)	
			btfss 	STATUS,Z			; Z=1? ou 153 xor COUNT = 0? Se sim, significa que COUNT=153	
			goto 	MOT1OFF				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim	
			bcf 	T12					; Se sim, ent�o desative a Flag T12   
			clrf 	COUNT				; Zere o conte�do de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto	
		MOT1OFF:						; Fim da Execu��o
	endif								; Fim da Condi��o

	; Tempo de 3,9 segundos para chegar a 100C�
	if 	reg==FLAG1 & bit==0				; Se argumento for a FLAG C100 ent�o fa�a o c�digo abaixo...	
			btfsc 	V5					; Se V5 estiver desligado salte 1 instru��o	
			goto 	ENDTT				; Se n�o, N�o execute o temporizador, Vai para o fim
			btfsc 	TT					; Se LH estiver inativo salte 1 instru��o
			goto 	ENDTT				; Se n�o, N�o execute o temporizador, Vai para o fim
			btfss 	LH					; Se LH estiver ativo salte 1 instru��o
			goto 	ENDTT				; Se n�o, N�o execute o temporizador, Vai para o fim
			btfsc 	MOT1				; Se MOT1 estiver desligado salte 1 instru��o	
			goto 	ENDTT				; Se n�o, N�o execute o temporizador, Vai para o fim
			btfsc 	C100				; Se C100 estiver inativo, inicie o temporizador
			goto 	ENDTT0				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'100'				; Mova o valor 100 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 100 at� 255 (156 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador  
			bsf 	C100				; Ative a Flag C100 pra indicar que a temporiza��o est� ativa
		ENDTT0:							; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'100'				; Mova o valor 100 para W	
			xorwf 	CELSIUS,W			; Compare CELSIUS com 100C� (CELSIUS vai de 0 a 100)	
			btfss 	STATUS,Z			; Z=1? ou 100 xor CELSIUS = 0? Se sim, significa que CELSIUS=100
			goto 	ENDTT				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim
			bcf 	C100				; Se sim, ent�o desative a Flag C100   
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto 
		ENDTT:							; Fim da Execu��o, OBS.: Aqui n�o limpo CELSIUS, para fins de depura��o.
	endif								; Fim da Condi��o
	
	; Tempo de 3 segundos do repouso do aquecimento
	if 	reg==FLAG1 & bit==2				; Se argumento for a FLAG RT ent�o fa�a o c�digo abaixo...
			btfss 	TT					; Se TT estiver ativo salte 1 instru��o	
			goto 	ENDR				; N�o execute o temporizador, Vai para o fim
			btfsc 	V5					; Se V5 estiver desligado salte 1 instru��o
			goto 	ENDR				; N�o execute o temporizador, Vai para o fim
			btfsc 	RT					; Se RT estiver inativo, inicie o temporizador
			goto 	ENDR0				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 6 at� 255 (250 contagens)			
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador   
			bsf 	RT					; Ative a Flag RT pra indicar que a temporiza��o est� ativa
		ENDR0:							; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'47'				; Mova o valor 47 para W	
			xorwf 	COUNT,W				; Compare COUNT com 47 (COUNT vai de 0 a 47)	
			btfss 	STATUS,Z			; Z=1? ou 47 xor COUNT = 0? Se sim, significa que COUNT=47
			goto 	ENDR				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim	
			bcf 	RT					; Se sim, ent�o desative a Flag RT   
			clrf 	COUNT				; Zere o conte�do de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto  
		ENDR:							; Fim da Execu��o
	endif								; Fim da Condi��o

	; Tempo de 2,2 segundos para desativar LH,LM e LL nesta ordem
	if 	reg==Q0 & bit==4				; Se argumento for V5 ent�o fa�a o c�digo abaixo...
			btfsc 	V5					; Se V5 estiver desligado salte 1 instru��o
			goto 	STARTV5				; Se estiver ligado, Come�e as verifica��es
			goto 	ENDV5				; Salte para o fim e n�o fa�a nada
		STARTV5:						; VERIFICA��O DOS SENSORES ------------------------
			btfsc 	LH					; LH est� ativado?
			goto 	LHSTART				; Sim, ent�o verifique o tempo de LH
			btfsc 	LM					; N�o, LM est� ativado?
			goto 	LMSTART				; Sim, ent�o verifique o tempo de LM
			btfsc 	LL					; N�o, LL est� ativado?
			goto 	LLSTART				; Sim, ent�o verifique o tempo de LL
			goto 	ENDV5				; N�o, Ent�o saia e n�o fa�a nada
		LHSTART:						; VERIFICA��O DO TEMPO DE LH -----------------------
			btfss 	P2H					; A Flag P2H est� acionada?
			goto 	STARTTIMER			; N�o, ent�o inicie o temporizador
			movlw 	D'35'				; Sim, ent�o mova 35 para W
			xorwf 	COUNT,W				; Compara COUNT com 35
			btfss 	STATUS,Z			; Z=1? ou � igual a 35?
			goto 	ENDV5				; N�o, ent�o vai para o fim
			clrf 	COUNT				; Sim, ent�o limpe o contador
			bcf 	INTCON,TMR0IE 		; desative a interrup��o do TMR0
			bcf 	P2H					; desacione a Flag P2H
			bcf 	P1L					; desacione a Flag P1L
			bcf 	P1M					; desacione a Flag P1M
			bcf 	LHON				; desacione a Flag LHON indicando que LH est� inativo
			goto 	ENDV5				; Finalize as opera��es
		LMSTART:						; VERIFICA��O DO TEMPO DE LM -----------------------
			btfss 	P1M					; A Flag P1M est� acionada?
			goto 	STARTTIMER			; N�o, ent�o inicie o temporizador
			movlw 	D'35'				; Sim, ent�o mova 35 para W	
			xorwf 	COUNT,W				; Compara COUNT com 35	
			btfss 	STATUS,Z			; Z=1? ou � igual a 35?
			goto 	ENDV5				; N�o, ent�o vai para o fim	
			clrf 	COUNT				; Sim, ent�o limpe o contador
			bcf 	INTCON,TMR0IE 		; desative a interrup��o do TMR0 
			bcf 	P2H					; desacione a Flag P2H
			bcf 	P1L					; desacione a Flag P1L
			bcf 	P1M					; desacione a Flag P1M
			bcf 	LMON				; desacione a Flag LMON indicando que LM est� inativo
			goto 	ENDV5				; Finalize as opera��es
		LLSTART:						; VERIFICA��O DO TEMPO DE LL -----------------------
			btfss 	P1L					; A Flag P1L est� acionada?
			goto 	STARTTIMER			; N�o, ent�o inicie o temporizador
			movlw 	D'35'				; Sim, ent�o mova 35 para W		
			xorwf 	COUNT,W				; Compara COUNT com 35		
			btfss 	STATUS,Z			; Z=1? ou � igual a 35?
			goto 	ENDV5				; N�o, ent�o vai para o fim		
			clrf 	COUNT				; Sim, ent�o limpe o contador
			bcf 	INTCON,TMR0IE 		; desative a interrup��o do TMR0  
			bcf 	P2H					; desacione a Flag P2H
			bcf 	P1L					; desacione a Flag P1L
			bcf 	P1M					; desacione a Flag P1M
			bcf 	LLON				; desacione a Flag LLON indicando que LL est� inativo
			bsf 	RESC				; acione a Flag RESC indicando que o 2� reservatorio esta cheio
			goto 	ENDV5				; Finalize as opera��es
		STARTTIMER:						; INICIALIZA��O DO TEMPORIZADOR ----------------------
			movlw 	D'6'				; Mova o valor 6 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 6 at� 255 (250 contagens)	
			bsf 	INTCON,TMR0IE 		; Ative o temporizador   
			bsf 	P1L					; Acione a Flag P1L
			bsf 	P1M					; Acione a Flag P1M
			bsf 	P2H					; Acione a Flag P2H
		ENDV5:							; Fim da Execu��o
	endif								; Fim da Condi��o

	; Tempo de 4,1 segundos para chegar a 120psi
	if 	reg==FLAG2 & bit==4				; Se argumento for a FLAG P120 ent�o fa�a o c�digo abaixo...
			btfss 	V4					; Se V4 estiver ligado salte 1 instru��o	
			goto 	ENDV4				; N�o execute o temporizador, Vai para o fim
			btfsc 	P120				; Se P120 estiver inativo salte 1 instru��o
			goto 	ENDRES				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'120'				; Mova o valor 120 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 120 at� 255 (136 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador   
			bsf 	P120				; Ative a Flag P120 pra indicar que a temporiza��o est� ativa
		ENDRES:							; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'120'				; Mova o valor 120 para W	
			xorwf 	PSI,W				; Compare PSI com 120psi (PSI vai de 0 a 120)	
			btfss 	STATUS,Z			; Z=1? ou 120 xor PSI = 0? Se sim, significa que PSI=120
			goto 	ENDV4				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim	
			bcf 	P120 				; Se sim, ent�o desative a Flag P120
			bcf 	RESC				; Desative a FLAG RESC
			bcf 	INTCON,TMR0IE		; Desativa o temporizador por enquanto 
		ENDV4:							; Fim da Execu��o, OBS.: Aqui n�o limpo PSI, para fins de depura��o.
	endif								; Fim da Condi��o

	; Tempo de reten��o de 5 segundos para desligar BOMB 
	if 	reg==Q0 & bit==7				; Se argumento for BOMB ent�o fa�a o c�digo abaixo...	
			btfss   BOMB				; Se BOMB estiver ligado salte 1 instru��o		
			goto 	BOMBOFF				; N�o execute o temporizador, Vai para o fim 
			btfsc 	P3					; Se P3 estiver inativo, inicie o temporizador		
			goto 	COUNTP3				; Se j� estiver ativo, s� verifique se j� chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contar� de 6 at� 255 (250 contagens)	
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador  
			bsf 	P3					; Ative a Flag P3 pra indicar que a temporiza��o est� ativa	
		COUNTP3:						; INICIO DA VERIFICA��O DO TEMPO ----------------------------
			movlw 	D'78'				; Mova o valor 78 para W
			xorwf 	COUNT,W				; Compare COUNT com 78 (COUNT vai de 0 a 78)	
			btfss 	STATUS,Z			; Z=1? ou 78 xor COUNT = 0? Se sim, significa que COUNT=78
			goto 	BOMBOFF				; Se n�o, ainda n�o chegou no tempo ent�o vai para o fim
			bcf 	P3					; Se sim, ent�o desative a Flag P3 
			clrf 	COUNT				; Zere o conte�do de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto  
			bsf 	EOP					; Ative a Flag EOP para indicar que ser� "Fim do Programa"
		BOMBOFF:						; Fim da Execu��o
	endif								; Fim da Condi��o
ENDM
; --------------------------------------------------------------------------------------------------

#ENDIF
