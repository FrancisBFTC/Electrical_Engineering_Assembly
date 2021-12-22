list p = 16F877A

#include <p16F877a.inc>

__config 	_XT_OSC & _WDT_OFF & _LVP_OFF & _BOREN_OFF

#include <clpmemory.inc>
#include <clpmacros.inc>

; ---- Vetor de RESET ----
	
	ORG 	0000H
	goto 	_config

; ---- Vetor de INTERRUPÇÃO ---
	
	ORG 	0004H
	call 	Interrupt
	retfie

#include <interrupts.inc>
#include <configs.inc>

_start:
	#include <Equacoes_Booleanas.inc>
	goto 	_start

END
	



