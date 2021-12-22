; -------------------------------------------------------------------------
; 3� Denominador
; Assembly Microcontrolador PIC16F6284A
;
; Autor : Wender Francis   Data: 28/10/2021
; Clock : 4 MHz
; 
; Programa do 3� Denominador (s� + 2s� + 5s - 1) ********************************
; Descri��o: Programa que recebe os coeficientes do 3� Denominador
; E realiza a subtra��o dos resultados das multiplica��es
; de Coef0 com Coef3 e de Coef1 com Coef2, Zero Flags sendo definida, significa
; que possui estabilidade marginal, retornando 0; Zero Flags sendo limpada,
; Logo n�o possui estabilidade marginal, retornando 1; 
; **************************************************************************
; -------------------------------------------------------------------------

; *** Defini��o do Microcontrolador Utilizado ***
list p=16f628A   

; *** Inclus�o das defini��es de endere�os do PIC ***
#include	<p16f628a.inc>

; *** Configura��o dos FUSE BITs ***
__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF


; *** Aloca��o dos registradores de Uso Gerais ***
CBLOCK     H'1F' 		; Inicia da mem�ria de usu�rio no endere�o 0x1F
  	Coef0			 	; Registrador do Coeficiente 0
	Coef1				; Registrador do Coeficiente 1
	Coef2				; Registrador do Coeficiente 2
	Coef3				; Registrador do Coeficiente 3
	HProd1				; Armazena Produto Alto entre Coef0 e Coef3
	LProd1				; Armazena Produto Baixo entre Coef0 e Coef3
	HProd2				; Armazena Produto Alto entre Coef1 e Coef2
	LProd2				; Armazena Produto Baixo entre Coef1 e Coef2
	Resul				; Resultado para estabilidade marginal  
ENDC					; Fim da mem�ria de usu�rio

; *** Vetor de RESET ***
	org		H'0000'
	goto	InicioPrograma			; Salta o Vetor de Interrup��o indo para o Programa Inicial

; *** Vetor de Interrup��o ***
	org 	H'0004'
	retfie
		
; *****************************************************************************************
; Inicio do Programa:
; 	Coeficientes da Fun��o H(s): Testando 3� Fun��o -> (s� + 2s� + 5s - 1)
; *****************************************************************************************
InicioPrograma:
	movlw	D'1'					; Move Coeficiente 1 para Registrador de Trabalho
	movwf	Coef0					; Move do Registrador de Trabalho para Registrador Coef0
	movlw	D'1'					; Move Coeficiente 1 para Registrador de Trabalho
	movwf	Coef1					; Move do Registrador de Trabalho para Registrador Coef1
	movlw	D'3'					; Move Coeficiente 3 para Registrador de Trabalho
	movwf	Coef2					; Move do Registrador de Trabalho para Registrador Coef2
	movlw	D'255'					; Move Coeficiente 255 para Registrador de Trabalho
	movwf	Coef3					; Move do Registrador de Trabalho para Registrador Coef3

	call    RealizaProduto1			; Chama a Sub-Rotina 'RealizaProduto1'
	call	RealizaProduto2			; Chama a Sub-Rotina 'RealizaProduto2'
	call 	SubtraiProdutos 		; Chama a Sub-Rotina 'SubtraiProdutos'
	
	btfsc   STATUS,Z				; Se bit Z do STATUS for 1...
	goto 	ValorLogico0			; Execute o ValorLogico0
	goto	ValorLogico1			; Se nao, Se for 0, Execute o ValorLogico1


; *****************************************************************************************
; Desenvolvimento das Sub-Rotinas 'RealizaProduto1', 'RealizaProduto2' & 'SubtraiProdutos'
; RealizaProduto1 = Coef0 x Coef3
; RealizaProduto2 = Coef1 x Coef2
; SubtraiProdutos = LProd2 - LProd1 
; Descri��o: A multiplica��o consiste em adicionar Y vezes o conteudo de X por ele mesmo (Um Loop)
; Gerando um produto final com a parte alta e baixa (Ex.: HProd1:LProd1), na subtra��o vamos
; supor que estamos usando apenas a parte baixa, j� que o programa apenas consiste em
; analisar se possui estabilidade marginal e n�o os produtos em si.
; *****************************************************************************************

RealizaProduto1:					; 1� Sub-Rotina para efetuar Coef0 x Coef3
	clrf    LProd1					; Limpa o 1� Registrador do Produto Baixo
	clrf    HProd1					; Limpa o 1� Registrador do Produto Alto
	movf    Coef3,W					; Move o valor do Registrador Coef3 para Registrador de Trabalho
	movwf	LProd1					; Move do Registrador de Trabalho para Registrador Prod1	
Multi1:		
	decf    Coef0,F					; Decrementa Coef0 at� valer 0
	btfsc   STATUS,Z				; Testa Bit 'Z' do Registrador STATUS, Skipa abaixo se o teste for 0
	return            				; Retorna � chamada se o teste acima for 1
	movf	Coef3,W    				; Se o teste for 0, Move o valor de Coef3 para Registrador de Trabalho
	addwf   LProd1,F				; Adicione do Registrador de Trabalho (Coef3) com o LProd1, Resultado Baixo em LProd1
	btfsc 	STATUS,C				; Testa se Bit 'Carry' for 1,se for, ent�o houve transbordo...
	incf	HProd1,F				; Logo, armazena a parta alta do produto em HProd1
	goto	Multi1					; Volte para a label Multi1				



RealizaProduto2:					; 1� Sub-Rotina para efetuar Coef1 x Coef2
	clrf    LProd2					; Limpa o 2� Registrador do Produto Baixo
	clrf    HProd2					; Limpa o 2� Registrador do Produto Alto
	movf    Coef2,W					; Move o valor do Registrador Coef2 para Registrador de Trabalho
	movwf	LProd2					; Move do Registrador de Trabalho para Registrador Prod2
Multi2:		
	decf    Coef1,F					; Decrementa Coef1 at� valer 0
	btfsc   STATUS,Z				; Testa Bit 'Z' do Registrador STATUS, Skipa abaixo se o teste for 0
	return            				; Retorna � chamada se o teste acima for 1           
	movf	Coef2,W    				; Se o teste for 0, Move o valor de Coef2 para Registrador de Trabalho    
	addwf   LProd2,F				; Adicione do Registrador de Trabalho (Coef2) com o LProd2, Resultado Baixo em LProd2
	btfsc 	STATUS,C				; Testa se Bit 'Carry' for 1,se for, ent�o houve transbordo...
	incf	HProd2,F				; Logo, armazena a parta alta do produto em HProd2
	goto	Multi2					; Volte para a label Multi2

; Vamos supor que estamos usando apenas a parte baixa, s� pra subtrair e
; verificar se possui estabilidade marginal
SubtraiProdutos:					; 3� Sub-Rotina para Subtrair os 2 produtos
	movlw	LProd1					; Move para Registrador de Trabalho o Valor de Prod1
	sublw	LProd2					; Subtraia o Valor de Prod2 com o Valor do Registrador de Trabalho (Prod1)
	return

; *****************************************************************************************
; Desenvolvimento das Sub-Rotinas 'ValorLogico0' & 'ValorLogico1'
; ValorLogico0 = Retorna 0 pois o sistema possui estabilidade marginal
; ValorLogico1 = Retorna 1 pois o sistema NAO possui estabilidade marginal
; *****************************************************************************************
ValorLogico0:						; Caso possua estabilidade marginal
	clrf    Resul					; Limpa o Registrador de Resultado
	movlw 	D'0'					; Move o Decimal 0 para o Work
	movwf 	Resul					; Move de Work para o Registrador Resul
	goto    $						; Loop infinito na mesma instru��o
 
ValorLogico1:						; Caso nao possua estabilidade marginal
	clrf    Resul					; Limpa o Registrador de Resultado
	movlw 	D'1'					; Move o Decimal 1 para o Work
	movwf 	Resul					; Move de Work para o Registrador Resul
	goto    $ 						; Loop infinito na mesma instru��o
			
end									; Fim do Programa