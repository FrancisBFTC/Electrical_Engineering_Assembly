; Biblioteca de Macros do motor de partida direta

; NOME_MACRO  {macro} ARGUMENTOS_SEPARADOS_POR_VIRGULA


BANK_SELECT   macro  	num
	IF 	num==0
		bcf 	STATUS,RP0
		bcf 	STATUS,RP1
	ENDIF
	IF 	num==1
		bsf 	STATUS,RP0
		bcf 	STATUS,RP1
	ENDIF
	IF 	num==2
		bcf 	STATUS,RP0
		bsf 	STATUS,RP1
	ENDIF
	IF 	num==3
		bsf 	STATUS,RP0
		bsf 	STATUS,RP1
	ENDIF
endm


LD		macro	reg,bit
	movwf 	TEMP
	clrw			; W = 0
	btfsc 	reg,bit
	movlw 	H'01'	; W = 1
	nop
endm

LDN		macro	reg,bit
	movwf 	TEMP
	clrw			; W = 0
	btfss 	reg,bit
	movlw 	H'01'	; W = 1
	nop
endm


OR		macro	reg,bit
	movwf 	TEMP
	clrw			; W = 0
	btfsc 	reg,bit
	movlw 	H'01'
	iorwf 	TEMP,W
endm

ORN		macro	reg,bit
	movwf 	TEMP
	clrw			; W = 0
	btfss 	reg,bit
	movlw 	H'01'
	iorwf 	TEMP,W
endm

AND		macro	reg,bit
	movwf 	TEMP
	clrw			; W = 0
	btfsc 	reg,bit
	movlw 	H'01'
	andwf 	TEMP,W
endm


ANDN	macro	reg,bit
	movwf 	TEMP
	clrw			; W = 0
	btfss 	reg,bit
	movlw 	H'01'
	andwf 	TEMP,W
endm

OUT 	macro 	reg,bit
	movwf 	TEMP
	andlw 	H'01'
	btfss 	STATUS,Z
	goto 	$ + 3
	bcf 	reg,bit
	goto 	$ + 2
	bsf 	reg,bit
	movf	TEMP,W
endm

