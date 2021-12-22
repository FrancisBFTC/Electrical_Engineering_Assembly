Interrupt:
; ***************************************************************
; Salva os contextos do programa
	MOVWF    W_TEMP           ;Copy W to TEMP register
	SWAPF    STATUS,W         ;Swap status to be saved into W 
	CLRF     STATUS           ;bank 0, regardless of current bank, Clears IRP,RP1,RP0
	MOVWF    STATUS_TEMP      ;Save status to bank zero STATUS_TEMP register
	MOVF     PCLATH, W     	  ;Only required if using pages 1, 2 and/or 3
	MOVWF    PCLATH_TEMP      ;Save PCLATH into W
	CLRF     PCLATH           ;Page zero, regardless of current page
	; Programa que trata a interrupção
	; ---------------------------------------------


	


	; ---------------------------------------------
; Recupera os contextos do programa
IEND:
	MOVF     PCLATH_TEMP, W ;Restore PCLATH
	MOVWF    PCLATH         ;Move W into PCLATH
	SWAPF    STATUS_TEMP,W  ;Swap STATUS_TEMP register into W,;(sets bank to original state)       
	MOVWF    STATUS         ;Move W into STATUS register
	SWAPF    W_TEMP,F 		;Swap W_TEMP
	SWAPF    W_TEMP,W       ;Swap W_TEMP into W

	retfie					; End of Interruption
; ***************************************************************