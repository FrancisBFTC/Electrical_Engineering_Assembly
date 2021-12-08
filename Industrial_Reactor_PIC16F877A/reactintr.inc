; **************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor   : Wender Francis  			  Criação: 06/12/2021 	  Revisão: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactintr.inc 
; Programa: Sistema de Reator Industrial para despache da preparação de produtos
;
;
; Descrição: Este é um arquivo da função da interrupção (A ISR), aqui também podem ser colocados
; outras funções, como a de Delay. Após armazenar contextos de W e STATUS, o sistema verifica
; a origem da interrupção, se é por TMR0 ou por RB0, sendo TMR0 a isr vai incrementar contadores
; de tempo, dependendo da Flag que for acionada, por Ex.: Quando Valvula V3 for acionada, ela deve
; aquecer o reservatório em 100Cº, então a Flag C100 será acionada e sendo acionada, o contador 
; CELSIUS irá incrementar até 100, TMR0 começando com 100, irá dar um tempo aproximado de 4 segundos,
; Enquanto que no caso de V4, a pressão interna deve atingir 120psi no reservatório secundário, V4
; ligado, aciona a Flag P120, que incrementa PSI 120 vezes, com TMR0 iniciando com 120, dá um tempo
; de 4,17 segundos, quando o contador atual chega ao máximo exigido (100 ou 120), a Flag atual é 
; limpada e isto que vai determinar o acionamento dos sensores TT ou TP. De uma forma simbólica, apenas
; por imaginação, se o sistema deve ficar de repouso por 3s após o aquecimento de 100Cº que levou 4s,
; Logo o repouso é 90% do aquecimento, se fosse 10 minutos de aquecimento, seria 9 minutos de repouso,
; o que por ventura teríamos um reservatório até bem repousado.
; Agora já no caso da origem de interrupção for por RB0, então o sistema salva o contexto dos registros
; Pois estes registros são zerados e 2 tipos de interrupção são desabilitadas, pra garantir que nada
; vai interferir o "pause" do sistema, por motivos de segurança. Se estamos falando de um sistema 
; real de reator, então vamos imaginar que, o STOP poderia ser um botão de emergência, para se analisar
; possíveis ocorridos ou inserir mais ou menos produtos, talvez um gerenciamento manual. Isto significa
; que durante a espera do botão ser novamente pressionado, outros sistemas com controle manual poderiam
; ser inseridos, sem prejudicar o percurso original do reator. Se o botão for novamente pressionado, 
; os dados são recuperados e o processo volta de onde parou. Mas cabe salientar que eu precisei fazer
; uma rotina de Delay, pois o tempo é muito rápido pra processar o botão, a entrada na interrupção
; é até estável mas na saída pode ocorrer falhas, no caso do botão, ou seja, existe uma duração certa
; pra segurar o botão e o sistema sair da interrupção mas este pressionamento não pode passar do
; necessário, se não poderá entrar novamente na interrupção, justamente por isto os 500ms de Delay
; ajudará muito nisso.
; **************************************************************************************************

#IFNDEF 	__REACTINTR_INC__
#DEFINE 	__REACTINTR_INC__	; Isto permite só incluir se ainda não foi incluído

; *****************************************************************************************
; INICIO DA INTERRUPÇÃO
VOID_INT:						; Inicio da ISR chamada em 0x0004
	STORE_CONTEXT				; Armazena o contexto de STATUS e W

; --------------------------------------------------------------------------------------
; Código para Interrupção RB0 do Botão STOP --------------------------------------------
STOP_BUTTON:
	btfss 	INTCON,INTF			; Ouve uma interrupcao Externa?
	goto 	TIMER0INT			; Não! Entao veja se a próxima interrupção é TMR0
	bcf 	INTCON,INTF			; Sim! Então limpe a Flag setada pela interrupcao

; Salvar contextos de registros utilizados
SaveDatas:
	movf 	TMR0,W				; Salve o contador de tempo TMR0
	movwf 	TMR0_TEMP			; Para o registro temporario de TMR0
	movf 	COUNT,W				; Salve o contador auxiliar do TMR0
	movwf 	COUNT_TEMP			; Para o registro temporario do contador
	movf	Q0,W				; Salve os estados de pinos de PORTD
	movwf 	Q0_TEMP				; Para o registro temporario de PORTD
	movf 	Q1,W				; Salve o registro temporario de PORTC
	movwf 	Q1_TEMP				; Para o registro temporario de PORTC
	movf 	FLAG1,W				; Salve os sinalizadores de tempo e contagem
	movwf 	FLAG1_TEMP			; Para o registro temporario destes sinalizadores
	movf 	FLAG2,W				; Salve os sinalizadores de acionamento
	movwf  	FLAG2_TEMP			; Para o registro temporario destes sinalizadores
	movf 	CELSIUS,W			; Salve o contador de Graus Celsius
	movwf 	CELS_TEMP			; Para o registro temporario do contador de Graus
	movf 	PSI,W				; Salve o contador de psi de pressão
	movwf 	PSI_TEMP			; Para o registro temporario do contador de Psi
	
; Após salvar, limpe os registros
	clrf 	PORTA				; Limpe o Registro PORTA
	clrf 	PORTB				; Limpe o Registro PORTB
	clrf 	PORTC				; Limpe o Registro PORTC
	clrf 	PORTD				; Limpe o Registro PORTD
	clrf 	PORTE				; Limpe o Registro PORTE
	clrf 	FLAG1				; Limpe os bits das Flags de contagem
	clrf 	FLAG2				; Limpe os bits das Flags de acionamento
	clrf 	COUNT				; Limpe o contador
	clrf 	CELSIUS				; Limpe o contador de Graus Celsius
	clrf 	PSI 				; Limpe o contador de pressão
	bsf 	PS					; Acione o LED azul ao lado do STOP

; Se TMR0 Int. estiver habilitada, desabilite e salva estado
	bcf 	INTCON,INTE			; Desative por enquanto a interrupção RB0
	btfss 	INTCON,TMR0IE		; Teste se Interrupção por TMR0 esta habilitada
	goto 	WaitTime			; Se não, então só salte para Delay
	bcf 	INTCON,TMR0IE		; Se sim, então desabilite o TMR0 Int.
	bsf 	FLAG2,0				; Isto vai nos garantir que TMR0 estava habilitada antes


; Espere 500ms e Após, Espere STOP ser pressionado novamente
WaitTime:
	call 	Delay500ms			; Espere por retardo de 500ms
WaitStop:
	btfss 	STOP				; Botão STOP foi pressionado novamente?
	goto 	WaitStop			; Não, então aguarda ele ser 
								; Sim...

; Se TMR0 Int. Estava habilitada antes, habilite novamente
	btfss 	FLAG2,0				; Se este bit foi setado, então TMR0IE estava acionada
	goto 	RestoreDatas		; Se não estava, logo não nos preocupamos com TMR0IE
	bsf 	INTCON,TMR0IE		; Se estava, então reabilite a interrupção do TMR0

; Recuperar contextos de registros utilizados
RestoreDatas:
	movf	 Q0_TEMP,W			; Recupere o valor de PORTD armazenado
	movwf	 Q0					; Reponha no registro PORTD
	movf   	 Q1_TEMP,W			; Recupere o valor de PORTC armazenado
	movwf 	 Q1					; Reponha no registro PORTC
	movf 	 FLAG1_TEMP,W		; Recupere o valor dos sinalizadores de tempo
	movwf 	 FLAG1				; Reponha no registro destes sinalizadores
	movf 	 FLAG2_TEMP,W		; Recupere o valor dos sinalizadores de acionamento
	movwf  	 FLAG2				; Reponha no registro destes sinalizadores
	movf 	 CELS_TEMP,W		; Recupere o valor do contador de Graus Celsius
	movwf 	 CELSIUS			; Reponha no registro contador de Graus
	movf 	 PSI_TEMP,W			; Recupere o valor do contador de psi da pressão
	movwf 	 PSI				; Reponha no registro de contador de psi
	movf 	 COUNT_TEMP,W		; Recupere o valor do contador auxiliar de tempo
	movwf 	 COUNT				; Reponha no registro do contador auxiliar
	movf 	 TMR0_TEMP,W		; Recupere o valor da contagem do TMR0
	movwf 	 TMR0				; Reponha no registro do contador TMR0
	bsf 	 INTCON,INTE		; Reabilite a interrupção externa RB0
	goto 	 END_INT			; Saia da Interrupção
; --------------------------------------------------------------------------------------


; --------------------------------------------------------------------------------------
; Código dos temporizadores e contadores aqui ------------------------------------------
TIMER0INT:
	btfss 	INTCON,TMR0IF 		; Ouve uma interrupção por Overflow do TMR0?
	goto 	END_INT 			; Não, saia da interrupção	  
	bcf 	INTCON,TMR0IF 		; Sim, Zere a Flag TMR0IF de INTCON
	
	btfsc 	C100				; Se a Flag C100 estiver acionada
	goto 	IncCelsius			; Vai para o incremento de CELSIUS
	btfsc 	P120				; Se a Flag P120 estiver acionada
	goto 	IncPsi				; Vai para o incremento de PSI
	btfsc 	T1					; Se a Flag T1 estiver acionada
	goto 	DefTimer1			; Vai para a redefinição de tempo entre V1 e V2
	goto 	DefTimer			; Se não, Vai para o incremento do contador geral
IncCelsius:
	incf 	CELSIUS,F			; Incremente +1 em Graus Celsius
	movlw	D'100'				; Mova a literal 100 (Simbolizando 100Cº)
	movwf	TMR0				; ..Para TMR0, logo (256-100)x256 = 39936us x 100 ~= 4s
	goto 	IEND				; Saia da interrupção
IncPsi:
	incf 	PSI,F				; Incremente +1 em PSI da Pressão
	movlw	D'120'				; Mova a literal 120 (Simbolizando 120psi)
	movwf	TMR0				; ...Para TMR0, logo (256-120)x256 = 34816usx120 ~= 4,18s
	goto 	IEND				; Saia da interrupção
DefTimer:
	incf 	COUNT,F				; incremente +1 no contador auxiliar das válvulas
	movlw	D'6'				; Move a literal 6 para W e de W para TMR0
	movwf	TMR0				; (256-6)x256 = 64ms, o tempo final será reajustado em COUNT
	goto 	IEND				; Saia da interrupção	
DefTimer1:
	incf 	COUNT,F				; incremente +1 no contador auxiliar das válvulas
	movlw	D'11'				; Move a literal 11 para W e de W para TMR0
	movwf	TMR0				; (256-11)x245 = 62ms, o tempo final será reajustado em COUNT
	goto 	IEND				; Saia da interrupção
; --------------------------------------------------------------------------------------

END_INT:
	RESTORE_CONTEXT			; Recupera o contexto de STATUS e W
return						; Retorna pra chamada da ISR em 0x0004

; FIM DA INTERRUPÇÃO (retfie em 0x0004)
; *****************************************************************************************



; --------------------------------------------------------------------------------------
; Sub-Rotina de Delay de 500ms para o STOP ---------------------------------------------
Delay500ms:
	movlw 	D'200' 			; Move o valor 200 para W
	movwf   DELAY1			; Inicializa com 200 registro DELAY1
Time1:						; Já gastou 4 ciclos de máquina
	movlw 	D'250'			; Move o valor 250 para W
	movwf 	DELAY2			; Inicializa com 250 registro DELAY2
Time2:
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	nop						; Gasta 1 ciclo de máquina
	decfsz 	DELAY2			; Decrementa DELAY2 até que seja igual a 0
	goto 	Time2			; Vai para a label Time2
							; GOTO gasta 2 ciclos de máquinas, logo foram 10 ciclos
							; 250 x 10 ciclos de máquina = 2500 ciclos
	decfsz 	DELAY1			; Decrementa DELAY1 até que seja igual a 0
	goto 	Time1			; Vai para a label Time1
							; Instruções acima gastou 3 ciclos de máquina
							; 2500 x 200 = 500000 ciclos de máquina
return 						; Retorna para a chamada
; --------------------------------------------------------------------------------------

#ENDIF