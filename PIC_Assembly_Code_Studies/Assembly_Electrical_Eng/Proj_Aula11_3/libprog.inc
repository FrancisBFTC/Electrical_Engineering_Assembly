; **************************************************************************************************
; Prova N.2 - Quest�o 1 e 2 (Arquivo de Inclus�o)
; Assembly Microcontrolador PIC12F675
;
; Curso   : Eng. El�trica da IFBA 
; Autora  : Dirlene Napole�o   Data: 19/11/2021
; Clock   : 4MHz (Oscila��o interna)  Arquivo: libprog.inc 
; Programa: Classifica��o de sinais anal�gicos e calculo da �rea de onda 
;
; REGISTROS DE PROP�SITOS GERAIS & ENDERE�OS
;
; W_TEMP (0x40) -> Pra Armazenar contexto de W antes da interrup��o 
; STATUS_TEMP (0x41) -> Pra Armazenar contexto de STATUS antes da interrup��o
; DEC1 (0x42)   -> 1� Registro auxiliar para decodifica��o de ADRESH:ADRESL
; DEC2 (0x43)   -> 2� Registro auxiliar para decodifica��o de ADRESH:ADRESL
; V_TEMP (0x44) -> Armazena o dado analogico decodificado de cada sinal
; CONT (0x45)   -> Contador auxiliar para resto de tempo 20ms
; QNT  (0x46)   -> Ordem do sinal atual/Quantidade de sinais coletados
; _V1..._V10 (0x47...0x50) -> Pra armazenar cada sinal coletado, s�o 10 sinais
; _RkArea (0x51) -> Armazena cada sinal somado com o anterior
; _AreaH  (0x52) -> Parte Alta do resultado da multiplica��o da �rea
; _AreaL  (0x53) -> Parte Baixa do resultado da multiplica��o da �rea
; _DeltaT (0x54) -> Armazena o valor de 20ms / 10.
; ENDP 	  (0x55) -> Registro que sinaliza fim do programa
; ROTINAS INICIAIS DO PROGRAMA
;
; _Start     -> Configura as SFRs para Convers�o A/D e Interrup��o
; WaitDeltaT -> Espera tempo de "quase" 20ms pra cada coleta (Resto de tempo � completado na ISR)
;
; ROTINAS DA ISR DO PROGRAMA
;
;  _StartINT  -> Inicio da Interrup��o que armazena contextos, preenche os 20ms e reconfigura TMR0
; Conversion  -> Seta BIT 'GO_DONE' de 'ADCON0' para iniciar convers�o A/D
; WaitConv    -> Loop que espera pela convers�o A/D ser finalizada
; Decoding    -> Decodifica dados em ADRESH:ADRESL mesclando em 8 bits e dividindo por 2
; VerifyOrder -> Incrementa QNT e identifica ordem do sinal, armazena o sinal de acordo com a ordem
; ADD_V1..._V10 -> Armazena cada dado anal�gico coletado nos seus respectivos registros e vai pra soma
; AreaAdd     -> Soma do sinal coletado com o sinal anterior, armazenando em _RkArea
; MulDeltaT   -> Prepara os registros para a multiplica��o da �rea
; MUL -> Multiplica o valor total da soma em _AreaL por _DeltaT (20 / 10), resultado em _AreaH:_AreaL
; CompareSignal -> Compara 3 sinais el�tricos, dependendo do sinal a rotina liga um dos LEDs
; ON_LED1     -> Aciona o LED 1 entre 0.03V e 0.05V
; ON_LED2 	  -> Aciona o LED 2 e Desaciona o LED 1 entre 0.13V e 0.21v
; ON_LED3 	  -> Aciona o LED 3 e Desaciona o LED 2 entre 0.55V e 0.89V
;
; EXPLICA��ES DA L�GICA DO RESULTADO DA �REA 
;
; A cada 20ms exatos um sinal anal�gico � coletado da fonte Pwlin (Pwlin j� pr�-configurado 
; manualmente) e no per�odo total de 200ms, as 10 categorias (10 sinais) foram coletadas, 
; cada sinal com sua representa��o digital e decimal no seu respectivo registrador. 
; Somei todos os sinais e armazenei num outro registrador e depois multipliquei por (20 / 10)
; que deu 462. 462 � a �rea abaixo da curva de onda, onda que pode variar entre 0 a 5V, 
; ou seja, a �rea � 4,62V. Por�m se peg�ssemos 0.90V / 2 = 0.45V que � ~= 0.46V Logo 0,462V 
; (ou 462mV) seria a aproxima��o regular abaixo da forma de onda de 0.01V at� 0.9V... 
;
; Um pouco abaixo da metade de 1.0V, 10 vezes, seria o mesmo que dizer 4,62V
; abaixo de uma onda que varia at� 5V, se falamos da metade de 1.0V, logo:
;
; 10 vezes / 2 = 5V, ou seja, 10 vezes de aproxima��es regulares de 1V.
;
; T
; r(t) ~= {k=0 a N}rK x dT
; 0
;
; N = 10
; r = sinal
; k = instante de tempo
; rK = sinal somado
; T = 200ms
; t = 20ms
; r(t) = sinal atual
; _DeltaT (dT) = 20 (20 / 10)
; _RkArea (rK) = 231 (2,31) -> de 0ms a N somas
; _Area = _RkArea x DeltaT
; _Area = 231 x 2 = 462 | 2,31V x 200 = 462
;
; de 0 a 200ms, r(t) ~= 4,62V | 0V < r(t) < 5V
; 0.90V / (20ms / 10) = 0,45V ~= 0,46V
;
; r(t) ~= 0,500V <-> r(t) = 1V
; r(t) ~= 0,462V <-> r(t) = 0.9V
; r(t) ~= 2,500V <-> r(t) = 5V
; r(t) ~= 2,310V <-> r(t) = 4,62V 
;
; Somat�rio: 0.01 + 0.02 + 0.03 + 0.05 + 0.08 + 0.13 + 0.21 + 0.34 + 0.55 + 0.89 = 2,31 (231)
; Multiplica��o: (2,31 x 20 = 46,2) ou (2,31 x 200 = 462) ou (2,31 x 2 = 4,62)
;
; �LTIMAS CONSIDERA��ES
;
; Na pasta do Projeto tem a imagem "Depuracao_ProvaN2" com o Print da Tela apresentando a minha depura��o 
; no Proteus. Pra enxergar melhor o comportamento do programa abra o "PIC CPU Variables" em Debug e 
; marque os 19 registros de DEC1 at� _DeltaT como Valores Decimais, o resultado da Area estar� em
; _AreaH:_AreaL, disso achei melhor visto em Hexadecimal no PIC CPU Variables (0x1CE).
; E abra "PIC CPU Data Memory" a partir do Endere�o 0x50, pois fica vis�vel o valor da �rea nos Endere�os
; 0x52 e 0x53. J� na quest�o do tempo, eu n�o tive muito sucesso em depurar o tempo no Proteus
; Ent�o utilizei os recursos do MPLAB. Abrindo a janela StopWatch. Marquei um BreakPoint na linha 291
; na 1� defini��o de TMR0 e outro BreakPoint na linha 65 na 2� defini��o do TMR0 dentro da ISR.
; Dava um "Run" at� o 1� BreakPoint, tirava o BreakPoint e zerava os 19 ciclos do StopWatch e depois
; Dava outro "Run" at� o 2� BreakPoint e olhava o tempo que precisamente deu 20ms antes da convers�o 
; e cada 20000us se inicia outra convers�o, 10 sinais deu 200ms.
; Mesmo assim, pra facilitar eu adicionei um m�dulo de temporiza��o no circuito do Proteus e pode-se
; fazer a mesma forma de depura��o analisando o tempo neste m�dulo.
; **************************************************************************************************


; **** Palavra de Configura��o do Microcontrolador ****
__config 	_INTRC_OSC_NOCLKOUT & _WDT_OFF & _PWRTE_ON & _MCLRE_OFF & _CPD_OFF & _CP_OFF & _BODEN_OFF


; **** Entradas & Sa�das de dados ****
#define 	_SIGNAL 	GPIO, GP0 			; Sinal Anal�gico no Pino 0
#define 	_LED1 		GPIO, GP1			; Led 1 no pino 6
#define 	_LED2		GPIO, GP4			; Led 2 no pino 3
#define 	_LED3 		GPIO, GP5			; Led 3 no pino 2
#define 	TIMER_ON	bsf GPIO, GP2		; Inicia M�dulo de temporiza��o
#define 	TIMER_OFF	bcf GPIO, GP2		; Pausa M�dulo de temporiza��o

; **** Registros de Prop�sitos Gerais ****
CBLOCK 		H'40'			; Inicio da Mem�ria de Usu�rio
	W_TEMP					; Registro W temporario	para interrupcao
	STATUS_TEMP				; Registro STATUS temporario para interrupcao				
	DEC1				    ; Registro temporario 1 para decoding do sinal
	DEC2 					; Registro temporario 2 para decoding do sinal
	V_TEMP				    ; Armazena a voltagem temporaria atual
	CONT					; Contador auxiliar para tempo DeltaT
	QNT 					; Quantidade de categorias de sinais avaliadas
	_V1 					; 1� sinal da voltagem
	_V2						; 2� sinal da voltagem
	_V3						; 3� sinal da voltagem
	_V4						; 4� sinal da voltagem
	_V5						; 5� sinal da voltagem
	_V6						; 6� sinal da voltagem
	_V7						; 7� sinal da voltagem
	_V8						; 8� sinal da voltagem
	_V9						; 9� sinal da voltagem
	_V10					; 10� sinal da voltagem
	_RkArea 				; Valores do somatorio da Area
	_AreaH					; Resultado final da Somatorio (Parte Alta)
	_AreaL  				; Resultado final do Somatorio (Parte Baixa)
	_DeltaT 				; Valor de DeltaT = 20ms / 10
	ENDP					; Status que finaliza o programa
ENDC						; Fim da Mem�ria de Usu�rio

; **** Macro para sele��o de bancos  ****
_BANK   macro  	num				; Nome da Macro
	IF 	num==0					; Se num for 0, ent�o...
		bcf 	STATUS,RP0		; Seleciona banco de mem�ria 0
	ENDIF						; Se n�o...
	IF 	num==1					; Se num for 1, ent�o...
		bsf 	STATUS,RP0		; Seleciona banco de mem�ria 1
	ENDIF						; Fim da condicional
ENDM							; Fim da Macro

; **** Macro para Armazenamento de Contextos  ****
STORE_CONTEXT 	macro 	 ; Nome da Macro
	movwf 	W_TEMP    	 ; Salva o contexto de W no registro temporario
	swapf 	STATUS,W     ; Faz uma troca de Nibbles de STATUS e salva em W
	_BANK 0				 ; Seleciona Banco de memoria 0
	movwf 	STATUS_TEMP  ; Salva o contexto de STATUS no registro temporario
ENDM				   	 ; Fim da macro

; **** Macro para Recupera��o de Contextos  ****
RESTORE_CONTEXT  macro	   ; Nome da Macro
END_INT:				   ; Label de Fim da Interrup��o
	swapf 	STATUS_TEMP,W  ; Faz uma troca de Nibbles do STATUS temporario
	movwf 	STATUS		   ; Recupera o contexto de STATUS
	swapf 	W_TEMP,F       ; Faz uma troca de Nibbles em W temporario
	swapf 	W_TEMP,W       ; Recupera o contexto de W e n�o altera o BIT Z de STATUS
	retfie				   ; Retorno da Interrupcao
ENDM					   ; Fim da Macro
