; ********************************************************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor   : Wender Francis   			  Cria��o: 06/12/2021 	  Revis�o: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactlib.inc 
; Programa: Sistema de Reator Industrial para despache da prepara��o de produtos
;
;
; Descri��o: Aqui est�o as inclus�es de sistemas importantes para o funcionamento do reator.
; Este arquivo � inclu�do no "ReactMain.asm" e os arquivos inclu�dos s�o respons�veis por definir as SFRs,GPRs e FLAGs utilizadas,
; Constru��o de Macros para selecionar bancos, opera��es l�gicas e salvar/recuperar contextos de interrup��o,
; Configura��es iniciais do microcontrolador, incluindo configura��o dos registros necess�rios,
; Armazenamento das fun��es e ISRs que s�o utilizadas como 'VOID_INT' e 'Delay500ms',
; E constru��o das macros de cada conjunto de equa��es booleanas, equa��es respons�veis pelo funcionamento cont�nuo do Reator
; Que operam em um loop infinito do ReactMain.asm.
; ********************************************************************************************************************************************

#IFNDEF 	__REACTLIB_INC__
#DEFINE 	__REACTLIB_INC__		; Isto permite s� incluir se ainda n�o foi inclu�do

; --------------------------------------------------------------------------------------------------
; Inclus�es de arquivos do Reator ------------------------------------------------------------------
#include <reactdata.inc> 			; Inclue as Defini��es dos dados: GPRs, SFRs & FLAGs
#include <reactmacr.inc>			; Inclue as Macros para sele��o de bancos, instru��es l�gicas e contextos.
#include <reactconf.inc>  			; Inclue a Palavra de configura��o, vetores e configura��o dos registros.
#include <reactintr.inc>			; Inclue o Arquivo de fun��es e rotinas de interrup��o.
#include <reactbools.inc>			; Inclue as Macros de equa��es booleanas do reator.
; --------------------------------------------------------------------------------------------------

#ENDIF