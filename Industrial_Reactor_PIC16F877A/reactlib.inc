; ********************************************************************************************************************************************
; Assembly Microcontrolador PIC16F877A
;
; Disciplina: Microprocessadores & Microcontroladores
; Autor   : Wender Francis   			  Criação: 06/12/2021 	  Revisão: 07/12
; Clock   : 4MHz 		  	              Arquivo: reactlib.inc 
; Programa: Sistema de Reator Industrial para despache da preparação de produtos
;
;
; Descrição: Aqui estão as inclusões de sistemas importantes para o funcionamento do reator.
; Este arquivo é incluído no "ReactMain.asm" e os arquivos incluídos são responsáveis por definir as SFRs,GPRs e FLAGs utilizadas,
; Construção de Macros para selecionar bancos, operações lógicas e salvar/recuperar contextos de interrupção,
; Configurações iniciais do microcontrolador, incluindo configuração dos registros necessários,
; Armazenamento das funções e ISRs que são utilizadas como 'VOID_INT' e 'Delay500ms',
; E construção das macros de cada conjunto de equações booleanas, equações responsáveis pelo funcionamento contínuo do Reator
; Que operam em um loop infinito do ReactMain.asm.
; ********************************************************************************************************************************************

#IFNDEF 	__REACTLIB_INC__
#DEFINE 	__REACTLIB_INC__		; Isto permite só incluir se ainda não foi incluído

; --------------------------------------------------------------------------------------------------
; Inclusões de arquivos do Reator ------------------------------------------------------------------
#include <reactdata.inc> 			; Inclue as Definições dos dados: GPRs, SFRs & FLAGs
#include <reactmacr.inc>			; Inclue as Macros para seleção de bancos, instruções lógicas e contextos.
#include <reactconf.inc>  			; Inclue a Palavra de configuração, vetores e configuração dos registros.
#include <reactintr.inc>			; Inclue o Arquivo de funções e rotinas de interrupção.
#include <reactbools.inc>			; Inclue as Macros de equações booleanas do reator.
; --------------------------------------------------------------------------------------------------

#ENDIF