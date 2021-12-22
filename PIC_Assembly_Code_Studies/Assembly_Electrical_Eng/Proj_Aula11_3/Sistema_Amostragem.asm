; **************************************************************************************************
; Prova N.2 - Quest�o 1 e 2
; Assembly Microcontrolador PIC12F675
;
; Curso   : Eng. El�trica da IFBA 
; Autora  : Dirlene Napole�o   Data: 19/11/2021
; Clock   : 4MHz (Oscila��o interna)  Arquivo : Sistema_Amostragem.asm 
; Programa: Classifica��o de sinais anal�gicos e calculo da �rea de onda 
;
; O QUE ESTE PROGRAMA FAZ?
; 
; No inicio, ele configura o registrador de Interrup��o por Overflow do TMR0
; TMR0 Prescaler, configura o pino 0 para entrada analogica e habilita o
; conversor A/D. Ap�s isto, o programa contabiliza 20ms atrav�s de WaitDeltaT,
; ocorre uma interrup��o e completa o resto de tempo dentro da ISR.
; Na ISR espera pela Convers�o do sinal analogico e apos receber o dado de 10 bits, 
; uma decodifica��o � feita para garantir o valor decimal igual ao que foi coletado.
; Verifica a ordem do sinal pra armazenar o valor no respectivo registro e depois efetua
; a soma deste valor com o valor anterior, isto � feito at� 10 sinais coletados a cada 20ms.
; A partir do 10� sinal, uma multiplica��o � feita por DeltaT e o valor da �rea abaixo  
; da forma de onda � concedido (Aproxima��o de parti��o regular). As 10 categorias de sinais
; s�o coletadas no per�odo de 200ms. B�nus: 3 destes sinais s�o comparados para ligar 3 LEDs.
;
; CONSIDERA��ES IMPORTANTES:
;
; 1. Dentro da pasta da Prova existe um arquivo chamado "ProvaN2_INFO.txt" que eu
; mostro as configura��es manuais do Pwlin, as configura��es dos registros, os
; sinais anal�gicos estudados correspondentes ao ADRESH e ADRESL, a explica��o
; do c�lculo da �rea e defini��es de vari�veis da Quest�o 2 e a l�gica do porque
; deu o valor concedido da �rea.
; 2. No arquivo "libprog.inc", adicionada ao projeto eu configurei a palavra
; de configura��o, Criei defini��es dos pinos, Criei as macros de sele��o de
; Banco e macros para armazenamento do contexto na interrup��o, Criei as 21
; GPRs para este programa e tamb�m fiz uma explica��o de cada GPR no inicio da lib
; com seus respectivos endere�os e tamb�m expliquei a l�gica do c�lculo da �rea. 
; **************************************************************************************************

list p=12F675   							; Microcontrolador a ser utilizado

#include <p12f675.inc> 						; Arquivo de inclus�o do Microcontrolador
#include <libprog.inc>						; Configura��o,Defini��es de pinos, macros e vari�veis


; **** Vetor de RESET ****
	ORG 	0x0000
	goto 	_Start

; **** Vetor de INTERRUP��O ****
	ORG 	0x0004
; Inicio da Interrup��o causada por TMR0 ******************************************************
_StartINT:
	STORE_CONTEXT				; Armazena os contextos STATUS e W
	
	btfss 	INTCON,T0IF			; Ouve uma interrupcao de OverFlow do TIMER0?
	goto 	END_INT				; N�o! Entao saia da interrupcao
	bcf 	INTCON,T0IF			; Sim! Ent�o limpe a Flag setada pela interrupcao

	Fill32:						; CONT = 6,logo 6 x 3 ciclos = 18
		decfsz 	CONT,F			; Complete 18 ciclos pra chegar a 19968us
		goto 	Fill32			; As outras instru��es v�o se encarregar de completar o resto

	movlw 	D'6'				; Renova CONT com o valor 6, isto agora vai pra W
	movwf 	CONT				; CONT = 6. Isto e preciso pra calcular o resto de tempo de 32 ciclos
	movlw 	D'100'  			; 256 - 100 = 156 Contagens
	movwf 	TMR0				; 156 x 128 x 1us = 19968us (faltando 32 para 20ms)
								; Do 1� TMR0 at� este TMR0 deu 20000 ciclos

; Come�e a convers�o do sinal analogico r(t) ----------------------------------------------
Conversion:
	bsf 	ADCON0,GO_DONE 		; Diga que estar em processo de convers�o
WaitConv:						; Inicie o Loop de conversao
	btfsc 	ADCON0, GO_DONE		; Convers�o finalizou? (BIT<1> = 0?)
	goto 	WaitConv			; N�o! Ent�o, Espera convers�o.
								; Sim! Ent�o comece a decodificacao do sinal

; O dado analogico esta em ADRESH:ADRESL
; ------------------------------------------------------------------------------------------

; Decodificacao do sinal -------------------------------------------------------------------
; Consiste em pegar o valor de 10 bits de ADRESH:ADRESL e mescla-lo em 1
; so registrador de 8 bits, apos isto � dividido por 2 para saber a representacao
; decimal coerente ao sinal analogico
Decoding:
	movf 	ADRESH,W 			; Enviamos os 8 bits de ADRESH para W
	movwf  	DEC1 				; Movemos o conteudo de W pra um registro temporario
	bcf 	STATUS,C			; Limpe a Carry Flag de STATUS pra n�o atrapalhar na rota��o
	rlf 	DEC1,F 				; Deslocamos todos os bits para esquerda 1 vez de ADRESH
	bcf 	STATUS,C			; Limpe novamente a Carry Flag de STATUS
	rlf 	DEC1,F 				; Deslocamos novamente todos os bits para esquerda de ADRESH
	_BANK 1						; Selecione o Banco 1, pois ADRESL � do Banco 1
	movf 	ADRESL,W 			; Enviamos os 2 bits de ADRESL para W (os 6 bits menos significativo nao sao usados)
	_BANK 0						; Volte ao Banco 0 novamente
	movwf 	DEC2 				; Recupere o conteudo de ADRESL em W para DEC2
	rlf 	DEC2,F 				; Desloque 1 bit para a esquerda s� pra gerar a Carry Flag (Agora precisamos dela)
	rlf 	DEC2,F				; Desloque 1 bit para a esquerda
	rlf 	DEC2,F				; Desloque +1 bit para a esquerda. Pronto! Enviamos o ADRESL pro LSBs
	movf 	DEC1,W				; Salvamos DEC1 em W pois abaixo vamos mesclar isto.
	xorwf 	DEC2,W				; Mescle DEC1 e DEC2, salve em W
	movwf 	V_TEMP				; Movemos o dado analogico para o registro temporario da Voltagem.
	bcf 	STATUS,C			; Limpe a Carry Flag pra nao nos atrapalhar
	rrf 	V_TEMP,F			; rotacione 1 bit pra direita, ou, V_TEMP / 2; Permite ver o sinal coerente

; Pronto! A partir daqui V_TEMP esta com o sinal analogico decodificado!
; ou seja, o que era ADRESH:ADRESL, agora esta dentro de 8 bits
; ------------------------------------------------------------------------------------------

; Armazenamento dos sinais analogicos ------------------------------------------------------
; Aqui sera armazenado cada sinal entregue do Pwlin para o modulo ADC
; Nos seus respectivos registros, Ex.: _V1, _V2, _V3, etc... E depois fara a soma
; 1� Etapa -> Verificar qual e a ordem an�loga ao sinal
VerifyOrder:
	incf 	QNT,F				; Incremente +1 para verificar qual e o sinal
	movf 	QNT,W				; Agora armazene em W

	xorlw 	D'1' 				; Compara QNT com 1
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V1				; Sim, entao armazena o sinal na 1� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'2' 				; Compara QNT com 2
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V2				; Sim, entao armazena o sinal na 2� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'3' 				; Compara QNT com 3
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V3				; Sim, entao armazena o sinal na 3� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'4' 				; Compara QNT com 4
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V4				; Sim, entao armazena o sinal na 4� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'5' 				; Compara QNT com 5
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V5				; Sim, entao armazena o sinal na 5� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'6' 				; Compara QNT com 6
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V6				; Sim, entao armazena o sinal na 6� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'7' 				; Compara QNT com 7
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V7				; Sim, entao armazena o sinal na 7� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'8' 				; Compara QNT com 8
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V8				; Sim, entao armazena o sinal na 8� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'9' 				; Compara QNT com 9
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V9				; Sim, entao armazena o sinal na 9� variavel
	movf 	QNT,W				; Nao! Salve o valor em W e compare a proxima categoria
	xorlw 	D'10' 				; Compara QNT com 10
	btfsc 	STATUS,Z			; � igual?
	goto 	Add_V10				; Sim, entao armazena o sinal na 10� variavel
	goto 	END_INT				; Nao! Saia da interrupcao (Aqui nunca sera alcan�ado)

; 2� Etapa -> Armazenar o sinal decodificado no seu respectivo registro.
; Obs.: Lembrando que o sinal analogico atual decodificado esta em V_TEMP

Add_V1:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V1					; Salve o sinal de W para _V1
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V2:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V2					; Salve o sinal de W para _V2
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V3:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V3					; Salve o sinal de W para _V3
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V4:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V4					; Salve o sinal de W para _V4
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V5:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V5					; Salve o sinal de W para _V5
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V6:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V6					; Salve o sinal de W para _V6
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V7:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V7					; Salve o sinal de W para _V7
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V8
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V8					; Salve o sinal de W para _V8
	decf 	_V8,F				; Decremente -1 para atribuir o valor correto
	movf 	_V8,W				; Guarda o resultado em W para o somatorio
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V9:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V9					; Salve o sinal de W para _V9
	decf 	_V9,F				; Decremente -1 para atribuir o valor correto
	movf 	_V9,W 				; Guarda o resultado em W para o somatorio
	movwf 	V_TEMP				; Mova novamente pra V_TEMP para compara��o posterior
	goto 	AreaAdd				; Pule para o somatorio da Area
Add_V10:
	movf 	V_TEMP,W			; Mova o sinal analogico para W
	movwf 	_V10				; Salve o sinal de W para _V10
	decf 	_V10,F				; Decremente -1
	decf 	_V10,F				; Decremente -1, ou seja -2 para atribuir o valor correto
	movf 	_V10,W 				; Guarda o resultado em W para o somatorio
	addwf 	_RkArea,F			; Some + o ultimo sinal analogico
	goto 	MulDeltaT			; Salte para a multiplicacao por DeltaT (Um Breakpoint aqui d� 200ms)

; Obs.: Este programa pode ser facilmente adaptado pra analisar o sinal reverso (Onda decrescente)
; Para isto Apena zere os registros da area, decremente o registro QNT se outro registro auxiliar
; for 1, sendo 0 QNT incrementa, QNT sendo 10 ou 0, faz um COMF no registro auxiliar e claro:
; Adicionar mais voltagens nas configura��es manuais do Pwlin
; ------------------------------------------------------------------------------------------


; Somatorio da Area de cada valor analogico atual ------------------------------------------
; 3� Etapa -> Fazer a soma de cada sinal analogico atual com o anterior armazenando em _RkArea
AreaAdd:
	addwf 	_RkArea,F			; Some o sinal atual com o resultado anterior e armazene em RkArea
	goto 	CompareSignal		; Comparar sinais anal�gicos
; ------------------------------------------------------------------------------------------
 
	
; Multiplicacao das somas por DeltaT -------------------------------------------------------
; 4� Etapa -> Faz a multiplicacao por DeltaT e isso so executa no 10� sinal entregue.
; _RkArea  -> Registro que soma de volts em volts
; _AreaL   -> Armazena todas as somas dos volts (Que tambem estara em _RkArea)
; _AreaH   -> Parte Alta do resultado final da multiplicacao da Area
; _DeltaT  -> Uma faixa de tempo de cada sinal / 10 | 20ms / 10 = 2
; DESCRICAO IMPORTANTE :
;	Multiplicar 231 x 2 (462), � o mesmo que dizer: 2,31V x 200ms (462 ou 4,62V)
; 	ou seja, tudo representa a voltagem 4,62V. Isto porque eu fiz o mapeamento usando
; 	10 bits do registrador ADRESH:ADRESL.
; 	462 / 10 / 10 ser� 4,62 logo tamb�m dividimos o tempo de 200ms / 10 / 10
; 	que ser� 2, ou poder�amos dizer que, 20ms / 10 � 2 devido a 0,462 x 10 � 4,62.
; 	Pe�o-lhes que leia a descri��o nas primeiras linhas desse c�digo.
MulDeltaT:
	movlw 	D'1'			; Status para finalizar o programa
	movwf 	ENDP			; Mova para ENDP o valor de 1
	clrf 	_AreaL			; Limpe o registro da parte baixa da Area
	clrf 	_AreaH			; Limpe o registro da parte alta da Area
	clrf 	_DeltaT			; Limpe o registro _DeltaT
	movlw 	D'2'			; 20ms / 10 = 2 
	movwf 	_DeltaT 		; DeltaT = 2
	movf 	_RkArea,W		; Mova para o W o valor de todas as somas
	movwf 	_AreaL 			; Guarde na parte baixa da Area o valor de todas as somas
		
MUL:
	decf 	_DeltaT,F		; Decremente o conteudo de _DeltaT
	btfsc 	STATUS,Z		; O Decremento Chegou a zero?
	goto 	END_INT			; Sim! Ent�o fim da interrupcao
	addwf 	_AreaL,F 		; Nao! Ent�o adicione o resultado do somatorio por ela mesma
	btfsc 	STATUS,C		; Testa se Flag 'Carry' � 1, Sim? ent�o houve transbordo...
	incf	_AreaH,F		; Logo, entao incremente +1 na parte alta da Area
	goto 	MUL				; Mas se n�o, Volte para MUL e decremente _DeltaT +1 vez
; ------------------------------------------------------------------------------------------

; Compara��es de sinais el�tricos para acionar LEDs ----------------------------------------
; Compara o sinal el�trico atual com 3 valores, um dos valores vai ligar um dos LEDs
; Mas como o sinal � alternado em 20ms e este tempo d� l� em cima, ent�o vai tardar
; mais alguns ciclos pra assim ligar o LED, isto na depura��o vai dar uma ilus�o
; de que o LED aciona perto do pr�ximo sinal el�trico, como se fosse entre uma faixa e outra.
CompareSignal: 					; Compara 3 sinais anal�gicos para ligar 3 LEDs
	movf 	V_TEMP,W			; Move o tens�o atual para W
	xorlw 	D'003'			  	; Compara tens�o atual com 0,03V
	btfsc 	STATUS,Z			; � igual a 0,03V?
	goto 	ON_LED1				; Sim, ent�o liga LED 1
	movf 	V_TEMP,W 			; N�o, ent�o mova a tens�o atual para W
	xorlw 	D'013'				; Compara tens�o atual com 0,13V
	btfsc 	STATUS,Z			; � igual a 0,13V?
	goto 	ON_LED2				; Sim, ent�o liga LED 2
	movf 	V_TEMP,W 			; N�o, ent�o mova a tens�o atual para W
	xorlw 	D'055'				; Compara tens�o atual com 0,55V
	btfsc 	STATUS,Z			; � igual a 0,55V?
	goto 	ON_LED3				; Sim, ent�o liga LED 3
	goto 	END_INT				; Fim da Interrup��o

ON_LED1:						; Aciona entre 0.03V a 0.05V
	bsf 	_LED1				; Desligue LED 1
	goto 	END_INT				; Finalize interrup��o
ON_LED2:						; Aciona entre 0.13V a 0.21V 
	bcf 	_LED1				; Desligue LED 1
	bsf 	_LED2				; E Ligue LED 2
	goto 	END_INT				; Finalize interrup��o
ON_LED3:						; Aciona entre 0.55V a 0.89V
	bcf 	_LED2				; Desligue LED 2
	bsf 	_LED3				; E Ligue LED 3
	goto 	END_INT				; Finalize interrup��o
; ------------------------------------------------------------------------------------------

	RESTORE_CONTEXT			; Restaura os contextos de STATUS e W
; Fim da Interrup��o Causada por TMR0 ***********************************************************



; Inicio do Programa e Configura��es dos registradores *******************************************
_Start:
	_BANK 1
	movlw 	B'10000110' 	; Desabilita Pull-Ups, transi��o de descida  110  
	movwf 	OPTION_REG 		; & TMR0 Prescaler = 128 ciclos por cada contagem -> H'86'
	movlw 	B'00000001'		; Configura apenas Pino GP0 como Entrada
	movwf 	TRISIO			; GP2,GP4 como sa�da -> H'01'	
	movlw 	B'00010001'		; Configura <6:4> = 001 = 2us de Convers�o
	movwf 	ANSEL			; <3:0> = 0001 = Pino 0 como Anal�gico -> H'11'

	_BANK 0

	movlw 	B'10100000'		; BIT<7> = Habilita Interrup��es Globais 
	movwf 	INTCON 			; BIT<5> = Habilita interrup��o por TIMER0 -> H'A0'
	movlw 	B'00000111'		; <2:0> = 111 = Desabilita os comparadores
	movwf 	CMCON			; Outros bits n�o utilizados -> H'07'
	movlw 	B'00000001'		; <7> = Justificado para Esquerda; <6> = VDD
	movwf 	ADCON0			; <3:2> = Canal AN0; <1> = Nao esta em progresso
							; <0> = Habilita Conversor A/D -> H'01'
;      ADRESH  	  :  	ADRESL
; 0 0 0 0 0 0 0 0  	0 0 x x x x x x
; Sa�da da convers�o de 10 Bits
; Vou considerar todos os bits para decodificar o 
; valor e transforma-lo em 8 bits (Na rotina Decoding dentro da ISR) 
; *************************************************************************************************
	bcf 	_LED1				; Desacione LED 1
	bcf 	_LED2				; Desacione LED 2
	bcf 	_LED3				; Desacione LED 3

; Tempo de 20ms a cada sinal analogico coletado na ISR ********************************************
WaitDeltaT:
	movlw 	D'6'				; 6 coloca em W
	movwf 	CONT				; CONT = 6. Isto e preciso pra calcular o resto de tempo de 32 ciclos
	movlw 	D'100'  			; 256 - 100 = 156 Contagens
	movwf 	TMR0				; 156 x 128 x 1us = 19968us (faltando 32 para 20ms)
	TIMER_ON 					; Inicia a temporiza��o no m�dulo
								; Porem, 3 ciclos a mais sao executados antes de saltar pra interrupcao
								; logo, 19968 + 3 = 19971; 7 ciclos sao executados antes de 'Fill32' na
								; ISR e Fill32 executa 6 x 3 ciclos = 18 ciclos, logo 18 + 7 = 25 ciclos.
								; Apos Fill32 executa 4 ciclos, 25 + 4 = 29, 19971 + 29 = 20000 ciclos.	
								; Se marcar um BreakPoint come�ando da linha 327 (1� definicao do TMR0)
								; E outro BreakPoint terminando na linha 65 (2� definicao do TMR0 dentro da ISR)
								; Vera que executa exatamente 20000us = 20ms no MPLAB
				
_LOOP:
	btfss 	ENDP,0				; Se BIT 0 esta definido pra 1, ent�o salve para END
	goto 	_LOOP				; Um loop pra contabilizar o tempo, alguma coisa pode ser colocada aqui 
								; Enquanto o temporizador estiver em progresso.
; Ap�s esta linha o PIC causar� um Stack Underflow, se quiser continuar
; o programa, � s� trocar o _LOOP acima por um loop infinito

; *************************************************************************************************

END								; Fim do programa