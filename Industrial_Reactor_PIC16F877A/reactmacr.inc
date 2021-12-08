; **************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor   : Wender Francis  			  Criação: 06/12/2021 	  Revisão: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactmacr.inc 
; Programa: Sistema de Reator Industrial para despache da preparação de produtos
;
;
; Descrição: Este é um arquivo de Macros para seleção de bancos, salvamento de contextos durante
; a interrupção e operações lógicas OR, AND, XOR, NOT, ORN, ANDN, XORN e as operações auxiliares 
; IN e OUT. IN é a entrada do dado (INPUT) que serve para carregar um valor lógico inicial 0 ou 1
; de um módulo específico e OUT é a saída do dado (OUTPUT) que serve para enviar o resultado final
; para um módulo específico. O OUT de fato vai alterar o estado do módulo, tornando-o ativo ou inativo.
; Já a operação WHEN é algo novo, pois ela simboliza o "tempo de operação" de um módulo. O WHEN 
; Opera em 10 módulos diferentes, sendo eles: LL,LM,LH,V2,MOT1,V3,TT,V5,V4 e BOMB. Os módulos com tempo
; de retenção de acionamento são MOT1 e BOMB, Já de V2 é o tempo de retardo para ser ligado após o 
; o desligamento de V1, porém tem os tempos adicionais utilizados em outros módulos, como LL,LM e LH. 
; Entre o acionamento destes sensores, existe um intervalo de tempo para simbolizar que os produtos estão 
; sendo despejados, vamos dizer que num contexto real, o "líquido" do produto tem um tempo natural para 
; encher o reservatório até chegar naquele nível, então eu presumir este tempo através do WHEN. V3 é 
; responsável por aquecer o reservatório até 100 graus e V4 por dar a pressão interna de 120psi, então também 
; presumir que tanto a temperatura como a pressão, poderiam ser determinados por um timer. V5 faz o processo 
; inverso de V1 e V2, enquanto que V1 e V2 "aciona" os sensores de baixo para cima (Enchendo o reservatório), V5 
; "desaciona" os sensores de cima para baixo, ou seja, despachando a mistura do 1ª reservatório para
; o 2ª reservatório, portanto V5 também terá o mesmo tempo que V1 e V2 teve para despachar o líquido.
; Os únicos módulos que não tem um timer definido é V1 e V6, porque V1 não depende definitivamente do Tempo
; mas sim, do sensor LM. Enquanto que V6 também não depende do tempo mas sim, do usuário. V6 fica ligado
; até que o usuário pressione RESET, após isto, todo o sistema é reiniciado, esperando novamente que
; o usuário pressione START para recomeçar tudo.
; **************************************************************************************************

#IFNDEF 	__REACTMACR_INC__
#DEFINE 	__REACTMACR_INC__	; Isto permite só incluir se ainda não foi incluído


; --- Macro para seleção de bancos de memoria ------------------------------------------------------
; --------------------------------------------------------------------------------------------------
BANK_SELECT   macro  	num		; Nome da Macro
	IF 	num==0					; Argumento é igual ao Banco 0?
		bcf 	STATUS,RP0		; Sim, limpe o bit RP0 de STATUS
		bcf 	STATUS,RP1		; Sim, limpe o bit RP1 de STATUS
	ENDIF
	IF 	num==1					; Argumento é igual ao Banco 1?
		bsf 	STATUS,RP0		; Sim, sete o bit RP0 de STATUS
		bcf 	STATUS,RP1		; Sim, limpe o bit RP1 de STATUS
	ENDIF
	IF 	num==2					; Argumento é igual ao Banco 2?
		bcf 	STATUS,RP0		; Sim, limpe o bit RP0 de STATUS
		bsf 	STATUS,RP1		; Sim, sete o bit RP1 de STATUS
	ENDIF
	IF 	num==3					; Argumento é igual ao Banco 3?
		bsf 	STATUS,RP0		; Sim, sete o bit RP0 de STATUS
		bsf 	STATUS,RP1		; Sim, sete o bit RP1 de STATUS
	ENDIF
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Macro para Armazenamento de Contextos --------------------------------------------------------
; --------------------------------------------------------------------------------------------------
STORE_CONTEXT 	macro 	 	 	; Macro STORE_CONTEXT (0 argumento)
	movwf    W_TEMP				; Salve W em W_TEMP 
	swapf    STATUS,W      		; Faça uma troca de Nibbles de STATUS, mova para W
	clrf     STATUS   			; Limpe o conteúdo de STATUS
	movwf    STATUS_TEMP   		; Salve W em STATUS_TEMP
	movf     PCLATH, W   		; Mova PCLATH para W
	movwf    PCLATH_TEMP   		; Salve W em PCLATH_TEMP
	clrf     PCLATH      		; Limpe o conteúdo de PCLATH
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Macro para Recuperação de Contextos ----------------------------------------------------------
; --------------------------------------------------------------------------------------------------
RESTORE_CONTEXT 	macro 	  	; Macro RESTORE_CONTEXT (0 argumento)
IEND:						  	; Label para fim da interrupcao
	movf     PCLATH_TEMP, W   	; Mova PCLATH_TEMP para W
	movwf    PCLATH      	  	; Recupere o conteúdo de PCLATH
	swapf    STATUS_TEMP,W    	; Faça uma troca de Nibbles de STATUS_TEMP, mova para W   
	movwf    STATUS      	  	; Recupere o conteúdo de STATUS
	swapf    W_TEMP,F 		  	; Faça uma troca de Nibbles de W_TEMP, salva nele mesmo
	swapf    W_TEMP,W    	  	; Faça uma troca de Nibbles de W_TEMP, recupera W
ENDM						  	; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Entrada de um dado lógico --------------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
IN		macro	reg,bit			; Macro IN: Argumentos reg e bit
	clrw						; Limpe o conteudo de W -> W = 0
	btfsc 	reg,bit				; O BIT 'bit' do registro 'reg' é 0?
	movlw 	H'01'				; Não, Salve 1 em W
	nop							; Sim, W permanece em 0
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Carrega um dado lógico negado ----------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
NOT		macro	reg,bit			; Macro NOT: Argumentos reg e bit
	clrw						; Limpe o conteúdo de W -> W = 0
	btfss 	reg,bit				; O BIT 'bit' do registro 'reg' é 1?
	movlw 	H'01'				; Não, Muda 0 para 1 em W
	nop							; Sim, W permanece em 0
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Operação OR com o dado em TEMP ---------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
OR		macro	reg,bit			; Macro OR: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfsc 	reg,bit				; O dado atual é 0?
	movlw 	H'01'				; Não, Então W será 1
	iorwf 	TEMP,W				; Sim, Salva em W um OR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Operação OR negado com o dado em TEMP --------------------------------------------------------
; --------------------------------------------------------------------------------------------------
ORN		macro	reg,bit			; Macro ORN: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfss 	reg,bit				; O dado atual é 1?
	movlw 	H'01'				; Não, Então W será 1
	iorwf 	TEMP,W				; Sim, Salva em W um OR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Operação AND com o dado em TEMP --------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
AND		macro	reg,bit			; Macro AND: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfsc 	reg,bit				; O dado atual é 0?
	movlw 	H'01'				; Não, Então W será 1
	andwf 	TEMP,W				; Sim, Salva em W um AND de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Operação AND negado com o dado em TEMP -------------------------------------------------------
; --------------------------------------------------------------------------------------------------
ANDN	macro	reg,bit			; Macro ANDN: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	clrw						; Limpa o W, W = 0
	btfss 	reg,bit				; O dado atual é 1?
	movlw 	H'01'				; Não, Então W será 1
	andwf 	TEMP,W				; Sim, Salva em W um AND de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Operação XOR com o dado em TEMP --------------------------------------------------------------
; --------------------------------------------------------------------------------------------------
XOR	    macro   reg,bit			; Macro XOR: Argumentos reg e bit
	movwf	TEMP				; Salva o dado anterior em TEMP
	clrw 						; Limpa o W, W = 0
	btfsc   reg,bit				; O dado atual é 0?
	movlw	H'01'				; Não, Então W será 1
	xorwf	TEMP,W 				; Sim, Salva em W um XOR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Operação XOR negada com o dado em TEMP -------------------------------------------------------
; --------------------------------------------------------------------------------------------------
XORN	  macro   reg,bit		; Macro XORN: Argumentos reg e bit
	movwf	TEMP				; Salva o dado anterior em TEMP
	clrw 						; Limpa o W, W = 0
	btfss   reg,bit				; O dado atual é 1?
	movlw	H'01'				; Não, Então W será 1
	xorwf	TEMP,W 				; Sim, Salva em W um XOR de W com o dado anterior
ENDM							; Fim da Macro
; --------------------------------------------------------------------------------------------------

; --- Saída de um resultado no bit do registro -----------------------------------------------------
; --------------------------------------------------------------------------------------------------
OUT 	macro 	reg,bit			; Macro OUT: Argumentos reg e bit
	movwf 	TEMP				; Salva o dado anterior em TEMP
	andlw 	H'01'				; Faça um AND de W com 1
	btfss 	STATUS,Z			; O resultado do AND deu 0?
	goto 	$ + 3				; Não, Então vai para a 3ª instrução após esta linha
	bcf 	reg,bit				; Sim, Então limpe o BIT 'bit' do registro 'reg'
	goto 	$ + 2				; Vai para a 2ª instrução após esta linha porque o AND deu 0
	bsf 	reg,bit				; Sete o BIT 'bit' do registro 'reg' porque no AND deu 1
	movf	TEMP,W				; Salve o dado anterior de TEMP para W
ENDM							; Fim da macro
; --------------------------------------------------------------------------------------------------


; --- Macro para tempo de operação -----------------------------------------------------------------
; Esta macro com o seu nome 'WHEN' exprime uma noção de "tempo", pois é quando algo ocorre. Ela faz
; exatamente isto, ela é uma condição para um ou dois módulos : se este(s) módulo(s) está ligado ou 
; desligado, a responsabilidade do WHEN é iniciar uma temporização, setando uma FLAG específica para
; aquele módulo dizendo que a contagem de tempo está ativa, quando o tempo finaliza, a FLAG que foi
; setada é agora limpada, onde o valor da FLAG será utilizada para determinar o resultado do módulo.
; Após a FLAG ser limpada, o temporizador fica inativo.
; --------------------------------------------------------------------------------------------------
WHEN   macro 	reg,bit					; Macro WHEN: Argumentos reg e bit

	; Retardo de 1 segundo para ligar V2
	if 	reg==FLAG1 & bit==1				; Se argumento for a FLAG T1 então faça o código abaixo...
			btfsc 	LH					; Se LH estiver inativo, salte 1 instrução
			goto 	ENDV1				; Não inicie o temporizador, Vai para o fim
			btfss 	LM					; Se LM estiver ativo, salte 1 instrução
			goto 	ENDV1				; Não inicie o temporizador, Vai para o fim
			btfsc 	V1					; Se V1 estiver desligado salte 1 instrução
			goto 	ENDV1				; Não inicie o temporizador, Vai para o fim
			btfsc 	V2					; Se V2 estiver desligado salte 1 instrução
			goto 	ENDV1				; Não inicie o temporizador, Vai para o fim	
			btfsc 	V5					; Se V5 estiver desligado salte 1 instrução
			goto 	ENDV1				; Não inicie o temporizador, Vai para o fim			
			btfsc 	T1					; Se T1 estiver inativo, inicie o temporizador
			goto 	ENDV10				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'11'				; Mova o valor 11 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 11 até 255 (245 contagens)
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador
			bsf 	T1					; Ative a Flag T1 pra indicar que a temporização está ativa
			bsf 	TIMER 				; Ligue o display de tempo do Proteus
		ENDV10:							; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'16'				; Mova o valor 16 para W
			xorwf 	COUNT,W				; Compare COUNT com 16 (COUNT vai de 0 a 16)
			btfss 	STATUS,Z			; Z=1? ou 16 xor COUNT = 0? Se sim, significa que COUNT=16
			goto 	ENDV1				; Se não, ainda não chegou no tempo então vai para o fim
			bcf 	TIMER 				; Desligue o display de tempo do Proteus
			bcf 	T1					; Se sim, então desative a Flag T1
			clrf 	COUNT				; Zere o conteúdo de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto
		ENDV1:							; Fim da Execução
	endif								; Fim da Condição

	; Tempo de 2,2 segundos para ativar LL (Tempo imaginário de enchimento)
	if 	reg==FLAG1 & bit==3				; Se argumento for a FLAG P1L então faça o código abaixo...
			btfsc 	LL					; Se LL estiver inativo salte 1 instrução
			goto 	ENDLL				; Não inicie o temporizador, Vai para o fim
			btfss 	V1					; Se V1 estiver ativo, salte 1 instrução
			goto 	ENDLL				; Não inicie o temporizador, Vai para o fim
			btfsc 	P1L					; Se P1L estiver inativo, inicie o temporizador
			goto 	ENDLL0				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 6 até 255 (250 contagens)
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador
			bsf 	P1L					; Ative a Flag P1L pra indicar que a temporização está ativa
		ENDLL0:							; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'35'				; Mova o valor 35 para W
			xorwf 	COUNT,W				; Compare COUNT com 35 (COUNT vai de 0 a 35)
			btfss 	STATUS,Z			; Z=1? ou 35 xor COUNT = 0? Se sim, significa que COUNT=35
			goto 	ENDLL				; Se não, ainda não chegou no tempo então vai para o fim
			bcf 	P1L					; Se sim, então desative a Flag P1L
			bsf 	LLON				; Ative a Flag LLON pra indicar que LL está ativo
			clrf 	COUNT				; Zere o conteúdo de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto
		ENDLL:							; Fim da Execução
	endif								; Fim da Condição
	
	; Tempo de 2,2 segundos para ativar LM (Tempo imaginário de enchimento)
	if 	reg==FLAG1 & bit==4				; Se argumento for a FLAG P1M então faça o código abaixo...
			btfsc 	LM					; Se LM estiver inativo salte 1 instrução
			goto 	ENDLM				; Não inicie o temporizador, Vai para o fim
			btfss 	LL					; Se LL estiver ativo, salte 1 instrução
			goto 	ENDLM				; Não inicie o temporizador, Vai para o fim
			btfss 	V1					; Se V1 estiver ativo, salte 1 instrução
			goto 	ENDLM				; Não inicie o temporizador, Vai para o fim
			btfsc 	P1M					; Se P1M estiver inativo, inicie o temporizador
			goto 	ENDLM0				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W	
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 6 até 255 (250 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador 	
			bsf 	P1M					; Ative a Flag P1M pra indicar que a temporização está ativa
		ENDLM0:							; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'35'				; Mova o valor 35 para W	
			xorwf 	COUNT,W				; Compare COUNT com 35 (COUNT vai de 0 a 35)		
			btfss 	STATUS,Z			; Z=1? ou 35 xor COUNT = 0? Se sim, significa que COUNT=35	
			goto 	ENDLM				; Se não, ainda não chegou no tempo então vai para o fim	
			bcf 	P1M					; Se sim, então desative a Flag P1M 	
			bsf 	LMON				; Ative a Flag LMON pra indicar que LM está ativo
			clrf 	COUNT				; Zere o conteúdo de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto 
		ENDLM:							; Fim da Execução
	endif								; Fim da Condição

	; Tempo de 2,2 segundos para ativar LH
	if 	reg==FLAG1 & bit==5				; Se argumento for a FLAG P1H então faça o código abaixo...
			btfsc 	LH					; Se LH estiver inativo salte 1 instrução
			goto 	ENDLH				; Não execute o temporizador, Vai para o fim
			btfss 	LM					; Se LM estiver ativo, salte 1 instrução
			goto 	ENDLH				; Não execute o temporizador, Vai para o fim
			btfss 	V2					; Se V2 estiver ativo, salte 1 instrução
			goto 	ENDLH				; Não execute o temporizador, Vai para o fim
			btfsc 	P2H					; Se P1H estiver inativo, inicie o temporizador
			goto 	ENDLH0				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 6 até 255 (250 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador 
			bsf 	P2H					; Ative a Flag P2H pra indicar que a temporização está ativa
		ENDLH0:							; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'35'				; Mova o valor 35 para W	
			xorwf 	COUNT,W				; Compare COUNT com 35 (COUNT vai de 0 a 35)		
			btfss 	STATUS,Z			; Z=1? ou 35 xor COUNT = 0? Se sim, significa que COUNT=35
			goto 	ENDLH				; Se não, ainda não chegou no tempo então vai para o fim	
			bcf 	P2H					; Se sim, então desative a Flag P2H  
			bsf 	LHON				; Ative a Flag LHON pra indicar que LH está ativo
			clrf 	COUNT				; Zere o conteúdo de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto 	
		ENDLH:							; Fim da Execução
	endif								; Fim da Condição
	
	; Tempo de retenção de 12 segundos para desligar MOT1
	if 	reg==FLAG1 & bit==6				; Se argumento for a FLAG T12 então faça o código abaixo...		
			btfss   MOT1				; Se MOT1 estiver ligado salte 1 instrução	
			goto 	MOT1OFF				; Se não, Não execute o temporizador, Vai para o fim 
			btfss 	LH					; Se LH estiver ativo salte 1 instrução
			goto 	MOT1OFF				; Se não, Não execute o temporizador, Vai para o fim
			btfss 	LM					; Se LM estiver ativo, salte 1 instrução
			goto 	MOT1OFF				; Não execute o temporizador, Vai para o fim
			btfsc 	T12					; Se T12 estiver inativo, inicie o temporizador		
			goto 	COUNTER				; Se já estiver ativo, só verifique se já chegou no tempo.		
			movlw 	D'6'				; Mova o valor 6 para W			
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 6 até 255 (250 contagens)	
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador  
			bsf 	T12					; Ative a Flag T12 pra indicar que a temporização está ativa
		COUNTER:						; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'153'				; Mova o valor 153 para W	
			xorwf 	COUNT,W				; Compare COUNT com 153 (COUNT vai de 0 a 153)	
			btfss 	STATUS,Z			; Z=1? ou 153 xor COUNT = 0? Se sim, significa que COUNT=153	
			goto 	MOT1OFF				; Se não, ainda não chegou no tempo então vai para o fim	
			bcf 	T12					; Se sim, então desative a Flag T12   
			clrf 	COUNT				; Zere o conteúdo de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto	
		MOT1OFF:						; Fim da Execução
	endif								; Fim da Condição

	; Tempo de 3,9 segundos para chegar a 100Cº
	if 	reg==FLAG1 & bit==0				; Se argumento for a FLAG C100 então faça o código abaixo...	
			btfsc 	V5					; Se V5 estiver desligado salte 1 instrução	
			goto 	ENDTT				; Se não, Não execute o temporizador, Vai para o fim
			btfsc 	TT					; Se LH estiver inativo salte 1 instrução
			goto 	ENDTT				; Se não, Não execute o temporizador, Vai para o fim
			btfss 	LH					; Se LH estiver ativo salte 1 instrução
			goto 	ENDTT				; Se não, Não execute o temporizador, Vai para o fim
			btfsc 	MOT1				; Se MOT1 estiver desligado salte 1 instrução	
			goto 	ENDTT				; Se não, Não execute o temporizador, Vai para o fim
			btfsc 	C100				; Se C100 estiver inativo, inicie o temporizador
			goto 	ENDTT0				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'100'				; Mova o valor 100 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 100 até 255 (156 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador  
			bsf 	C100				; Ative a Flag C100 pra indicar que a temporização está ativa
		ENDTT0:							; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'100'				; Mova o valor 100 para W	
			xorwf 	CELSIUS,W			; Compare CELSIUS com 100Cº (CELSIUS vai de 0 a 100)	
			btfss 	STATUS,Z			; Z=1? ou 100 xor CELSIUS = 0? Se sim, significa que CELSIUS=100
			goto 	ENDTT				; Se não, ainda não chegou no tempo então vai para o fim
			bcf 	C100				; Se sim, então desative a Flag C100   
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto 
		ENDTT:							; Fim da Execução, OBS.: Aqui não limpo CELSIUS, para fins de depuração.
	endif								; Fim da Condição
	
	; Tempo de 3 segundos do repouso do aquecimento
	if 	reg==FLAG1 & bit==2				; Se argumento for a FLAG RT então faça o código abaixo...
			btfss 	TT					; Se TT estiver ativo salte 1 instrução	
			goto 	ENDR				; Não execute o temporizador, Vai para o fim
			btfsc 	V5					; Se V5 estiver desligado salte 1 instrução
			goto 	ENDR				; Não execute o temporizador, Vai para o fim
			btfsc 	RT					; Se RT estiver inativo, inicie o temporizador
			goto 	ENDR0				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 6 até 255 (250 contagens)			
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador   
			bsf 	RT					; Ative a Flag RT pra indicar que a temporização está ativa
		ENDR0:							; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'47'				; Mova o valor 47 para W	
			xorwf 	COUNT,W				; Compare COUNT com 47 (COUNT vai de 0 a 47)	
			btfss 	STATUS,Z			; Z=1? ou 47 xor COUNT = 0? Se sim, significa que COUNT=47
			goto 	ENDR				; Se não, ainda não chegou no tempo então vai para o fim	
			bcf 	RT					; Se sim, então desative a Flag RT   
			clrf 	COUNT				; Zere o conteúdo de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto  
		ENDR:							; Fim da Execução
	endif								; Fim da Condição

	; Tempo de 2,2 segundos para desativar LH,LM e LL nesta ordem
	if 	reg==Q0 & bit==4				; Se argumento for V5 então faça o código abaixo...
			btfsc 	V5					; Se V5 estiver desligado salte 1 instrução
			goto 	STARTV5				; Se estiver ligado, Começe as verificações
			goto 	ENDV5				; Salte para o fim e não faça nada
		STARTV5:						; VERIFICAÇÃO DOS SENSORES ------------------------
			btfsc 	LH					; LH está ativado?
			goto 	LHSTART				; Sim, então verifique o tempo de LH
			btfsc 	LM					; Não, LM está ativado?
			goto 	LMSTART				; Sim, então verifique o tempo de LM
			btfsc 	LL					; Não, LL está ativado?
			goto 	LLSTART				; Sim, então verifique o tempo de LL
			goto 	ENDV5				; Não, Então saia e não faça nada
		LHSTART:						; VERIFICAÇÃO DO TEMPO DE LH -----------------------
			btfss 	P2H					; A Flag P2H está acionada?
			goto 	STARTTIMER			; Não, então inicie o temporizador
			movlw 	D'35'				; Sim, então mova 35 para W
			xorwf 	COUNT,W				; Compara COUNT com 35
			btfss 	STATUS,Z			; Z=1? ou É igual a 35?
			goto 	ENDV5				; Não, então vai para o fim
			clrf 	COUNT				; Sim, então limpe o contador
			bcf 	INTCON,TMR0IE 		; desative a interrupção do TMR0
			bcf 	P2H					; desacione a Flag P2H
			bcf 	P1L					; desacione a Flag P1L
			bcf 	P1M					; desacione a Flag P1M
			bcf 	LHON				; desacione a Flag LHON indicando que LH está inativo
			goto 	ENDV5				; Finalize as operações
		LMSTART:						; VERIFICAÇÃO DO TEMPO DE LM -----------------------
			btfss 	P1M					; A Flag P1M está acionada?
			goto 	STARTTIMER			; Não, então inicie o temporizador
			movlw 	D'35'				; Sim, então mova 35 para W	
			xorwf 	COUNT,W				; Compara COUNT com 35	
			btfss 	STATUS,Z			; Z=1? ou É igual a 35?
			goto 	ENDV5				; Não, então vai para o fim	
			clrf 	COUNT				; Sim, então limpe o contador
			bcf 	INTCON,TMR0IE 		; desative a interrupção do TMR0 
			bcf 	P2H					; desacione a Flag P2H
			bcf 	P1L					; desacione a Flag P1L
			bcf 	P1M					; desacione a Flag P1M
			bcf 	LMON				; desacione a Flag LMON indicando que LM está inativo
			goto 	ENDV5				; Finalize as operações
		LLSTART:						; VERIFICAÇÃO DO TEMPO DE LL -----------------------
			btfss 	P1L					; A Flag P1L está acionada?
			goto 	STARTTIMER			; Não, então inicie o temporizador
			movlw 	D'35'				; Sim, então mova 35 para W		
			xorwf 	COUNT,W				; Compara COUNT com 35		
			btfss 	STATUS,Z			; Z=1? ou É igual a 35?
			goto 	ENDV5				; Não, então vai para o fim		
			clrf 	COUNT				; Sim, então limpe o contador
			bcf 	INTCON,TMR0IE 		; desative a interrupção do TMR0  
			bcf 	P2H					; desacione a Flag P2H
			bcf 	P1L					; desacione a Flag P1L
			bcf 	P1M					; desacione a Flag P1M
			bcf 	LLON				; desacione a Flag LLON indicando que LL está inativo
			bsf 	RESC				; acione a Flag RESC indicando que o 2ª reservatorio esta cheio
			goto 	ENDV5				; Finalize as operações
		STARTTIMER:						; INICIALIZAÇÃO DO TEMPORIZADOR ----------------------
			movlw 	D'6'				; Mova o valor 6 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 6 até 255 (250 contagens)	
			bsf 	INTCON,TMR0IE 		; Ative o temporizador   
			bsf 	P1L					; Acione a Flag P1L
			bsf 	P1M					; Acione a Flag P1M
			bsf 	P2H					; Acione a Flag P2H
		ENDV5:							; Fim da Execução
	endif								; Fim da Condição

	; Tempo de 4,1 segundos para chegar a 120psi
	if 	reg==FLAG2 & bit==4				; Se argumento for a FLAG P120 então faça o código abaixo...
			btfss 	V4					; Se V4 estiver ligado salte 1 instrução	
			goto 	ENDV4				; Não execute o temporizador, Vai para o fim
			btfsc 	P120				; Se P120 estiver inativo salte 1 instrução
			goto 	ENDRES				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'120'				; Mova o valor 120 para W		
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 120 até 255 (136 contagens)		
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador   
			bsf 	P120				; Ative a Flag P120 pra indicar que a temporização está ativa
		ENDRES:							; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'120'				; Mova o valor 120 para W	
			xorwf 	PSI,W				; Compare PSI com 120psi (PSI vai de 0 a 120)	
			btfss 	STATUS,Z			; Z=1? ou 120 xor PSI = 0? Se sim, significa que PSI=120
			goto 	ENDV4				; Se não, ainda não chegou no tempo então vai para o fim	
			bcf 	P120 				; Se sim, então desative a Flag P120
			bcf 	RESC				; Desative a FLAG RESC
			bcf 	INTCON,TMR0IE		; Desativa o temporizador por enquanto 
		ENDV4:							; Fim da Execução, OBS.: Aqui não limpo PSI, para fins de depuração.
	endif								; Fim da Condição

	; Tempo de retenção de 5 segundos para desligar BOMB 
	if 	reg==Q0 & bit==7				; Se argumento for BOMB então faça o código abaixo...	
			btfss   BOMB				; Se BOMB estiver ligado salte 1 instrução		
			goto 	BOMBOFF				; Não execute o temporizador, Vai para o fim 
			btfsc 	P3					; Se P3 estiver inativo, inicie o temporizador		
			goto 	COUNTP3				; Se já estiver ativo, só verifique se já chegou no tempo.
			movlw 	D'6'				; Mova o valor 6 para W
			movwf 	TMR0				; Mova W para TMR0, TMR0 contará de 6 até 255 (250 contagens)	
			bsf 	INTCON,TMR0IE 		; Inicie o temporizador  
			bsf 	P3					; Ative a Flag P3 pra indicar que a temporização está ativa	
		COUNTP3:						; INICIO DA VERIFICAÇÃO DO TEMPO ----------------------------
			movlw 	D'78'				; Mova o valor 78 para W
			xorwf 	COUNT,W				; Compare COUNT com 78 (COUNT vai de 0 a 78)	
			btfss 	STATUS,Z			; Z=1? ou 78 xor COUNT = 0? Se sim, significa que COUNT=78
			goto 	BOMBOFF				; Se não, ainda não chegou no tempo então vai para o fim
			bcf 	P3					; Se sim, então desative a Flag P3 
			clrf 	COUNT				; Zere o conteúdo de COUNT
			bcf 	INTCON,TMR0IE 		; Desativa o temporizador por enquanto  
			bsf 	EOP					; Ative a Flag EOP para indicar que será "Fim do Programa"
		BOMBOFF:						; Fim da Execução
	endif								; Fim da Condição
ENDM
; --------------------------------------------------------------------------------------------------

#ENDIF
