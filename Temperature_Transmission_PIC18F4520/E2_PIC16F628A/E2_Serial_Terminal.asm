
_Serial_Send:

	MOVF       FARG_Serial_Send_val+0, 0
	MOVWF      TXREG+0
	CALL       _Serial_Wait+0
L_end_Serial_Send:
	RETURN
; end of _Serial_Send

_Serial_Write:

	CLRF       Serial_Write_i_L0+0
	CLRF       Serial_Write_i_L0+1
L_Serial_Write0:
	MOVF       FARG_Serial_Write_string+0, 0
	MOVWF      FARG_strlen_s+0
	CALL       _strlen+0
	MOVLW      128
	XORWF      Serial_Write_i_L0+1, 0
	MOVWF      R2+0
	MOVLW      128
	XORWF      R0+1, 0
	SUBWF      R2+0, 0
	BTFSS      STATUS+0, 2
	GOTO       L__Serial_Write22
	MOVF       R0+0, 0
	SUBWF      Serial_Write_i_L0+0, 0
L__Serial_Write22:
	BTFSC      STATUS+0, 0
	GOTO       L_Serial_Write1
	MOVF       Serial_Write_i_L0+0, 0
	ADDWF      FARG_Serial_Write_string+0, 0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      TXREG+0
	CALL       _Serial_Wait+0
	INCF       Serial_Write_i_L0+0, 1
	BTFSC      STATUS+0, 2
	INCF       Serial_Write_i_L0+1, 1
	GOTO       L_Serial_Write0
L_Serial_Write1:
L_end_Serial_Write:
	RETURN
; end of _Serial_Write

_Serial_Wait:

L_Serial_Wait3:
	BTFSC      TRMT_bit+0, BitPos(TRMT_bit+0)
	GOTO       L_Serial_Wait4
	GOTO       L_Serial_Wait3
L_Serial_Wait4:
L_end_Serial_Wait:
	RETURN
; end of _Serial_Wait

_interrupt:
	MOVWF      R15+0
	SWAPF      STATUS+0, 0
	CLRF       STATUS+0
	MOVWF      ___saveSTATUS+0
	MOVF       PCLATH+0, 0
	MOVWF      ___savePCLATH+0
	CLRF       PCLATH+0

	BTFSS      RCIF_bit+0, BitPos(RCIF_bit+0)
	GOTO       L_interrupt5
	BTFSC      FERR_bit+0, BitPos(FERR_bit+0)
	GOTO       L__interrupt19
	BTFSC      OERR_bit+0, BitPos(OERR_bit+0)
	GOTO       L__interrupt19
	GOTO       L_interrupt8
L__interrupt19:
	BCF        CREN_bit+0, BitPos(CREN_bit+0)
	BSF        CREN_bit+0, BitPos(CREN_bit+0)
	GOTO       L_interrupt9
L_interrupt8:
	INCF       _Received+0, 1
	BCF        RB6_bit+0, BitPos(RB6_bit+0)
	BSF        RB5_bit+0, BitPos(RB5_bit+0)
	MOVLW      127
	ANDWF      RCREG+0, 1
	MOVLW      0
	BTFSC      RCREG+0, 6
	MOVLW      1
	MOVWF      _D_+2
	MOVLW      32
	ANDWF      RCREG+0, 0
	MOVWF      R2+0
	MOVLW      5
	MOVWF      R1+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	MOVF       R1+0, 0
L__interrupt26:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt27
	RRF        R0+0, 1
	BCF        R0+0, 7
	ADDLW      255
	GOTO       L__interrupt26
L__interrupt27:
	MOVF       R0+0, 0
	XORWF      _D_+2, 1
	MOVLW      8
	ANDWF      RCREG+0, 0
	MOVWF      R2+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	MOVF       R0+0, 0
	XORWF      _D_+2, 1
	MOVLW      0
	BTFSC      RCREG+0, 6
	MOVLW      1
	MOVWF      _D_+1
	MOVLW      16
	ANDWF      RCREG+0, 0
	MOVWF      R2+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	MOVF       R0+0, 0
	XORWF      _D_+1, 1
	MOVLW      8
	ANDWF      RCREG+0, 0
	MOVWF      R2+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	MOVF       R0+0, 0
	XORWF      _D_+1, 1
	MOVLW      0
	BTFSC      RCREG+0, 5
	MOVLW      1
	MOVWF      _D_+0
	MOVLW      16
	ANDWF      RCREG+0, 0
	MOVWF      R2+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	MOVF       R0+0, 0
	XORWF      _D_+0, 1
	MOVLW      8
	ANDWF      RCREG+0, 0
	MOVWF      R2+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	MOVF       R0+0, 0
	XORWF      _D_+0, 1
	MOVLW      1
	ANDWF      RCREG+0, 0
	MOVWF      _D+0
	MOVLW      0
	BTFSC      RCREG+0, 1
	MOVLW      1
	MOVWF      _D+1
	MOVLW      0
	BTFSC      RCREG+0, 2
	MOVLW      1
	MOVWF      _D+2
	CLRF       _i+0
L_interrupt10:
	MOVLW      3
	SUBWF      _i+0, 0
	BTFSC      STATUS+0, 0
	GOTO       L_interrupt11
	MOVF       _i+0, 0
	ADDLW      _D_+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	MOVWF      R0+0
	MOVF       _i+0, 0
	ADDLW      _D+0
	MOVWF      FSR
	MOVF       INDF+0, 0
	XORWF      R0+0, 1
	MOVF       R0+0, 0
	MOVLW      1
	BTFSS      STATUS+0, 2
	MOVLW      0
	MOVWF      R1+0
	MOVF       R1+0, 0
	MOVWF      _Logic_Val+0
	MOVF       R1+0, 0
	XORLW      0
	BTFSS      STATUS+0, 2
	GOTO       L_interrupt13
	BSF        RB7_bit+0, BitPos(RB7_bit+0)
	BSF        RB4_bit+0, BitPos(RB4_bit+0)
	BCF        RB5_bit+0, BitPos(RB5_bit+0)
	DECF       _Received+0, 1
	BCF        RB7_bit+0, BitPos(RB7_bit+0)
	GOTO       L_interrupt11
L_interrupt13:
	INCF       _i+0, 1
	GOTO       L_interrupt10
L_interrupt11:
	MOVF       _Received+0, 0
	XORLW      1
	MOVLW      1
	BTFSS      STATUS+0, 2
	MOVLW      0
	MOVWF      R0+0
	MOVF       _Logic_Val+0, 0
	ANDWF      R0+0, 1
	BTFSC      STATUS+0, 2
	GOTO       L_interrupt14
	BSF        RB3_bit+0, BitPos(RB3_bit+0)
	MOVLW      0
	BTFSC      RCREG+0, 6
	MOVLW      1
	MOVWF      _D+6
	MOVLW      0
	BTFSC      RCREG+0, 5
	MOVLW      1
	MOVWF      _D+5
	MOVLW      0
	BTFSC      RCREG+0, 4
	MOVLW      1
	MOVWF      _D+4
	MOVLW      0
	BTFSC      RCREG+0, 3
	MOVLW      1
	MOVWF      _D+3
	BCF        RB4_bit+0, BitPos(RB4_bit+0)
	BCF        RB7_bit+0, BitPos(RB7_bit+0)
	GOTO       L_interrupt15
L_interrupt14:
	MOVF       _Received+0, 0
	XORLW      2
	MOVLW      1
	BTFSS      STATUS+0, 2
	MOVLW      0
	MOVWF      R0+0
	MOVF       _Logic_Val+0, 0
	ANDWF      R0+0, 1
	BTFSC      STATUS+0, 2
	GOTO       L_interrupt16
	BSF        RB0_bit+0, BitPos(RB0_bit+0)
	MOVLW      7
	MOVWF      R0+0
	MOVF       _D+6, 0
	MOVWF      R2+0
	MOVF       R0+0, 0
L__interrupt28:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt29
	RLF        R2+0, 1
	BCF        R2+0, 0
	ADDLW      255
	GOTO       L__interrupt28
L__interrupt29:
	MOVLW      6
	MOVWF      R1+0
	MOVF       _D+5, 0
	MOVWF      R0+0
	MOVF       R1+0, 0
L__interrupt30:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt31
	RLF        R0+0, 1
	BCF        R0+0, 0
	ADDLW      255
	GOTO       L__interrupt30
L__interrupt31:
	MOVF       R0+0, 0
	IORWF      R2+0, 0
	MOVWF      R3+0
	MOVLW      5
	MOVWF      R2+0
	MOVF       _D+4, 0
	MOVWF      R0+0
	CLRF       R0+1
	MOVF       R2+0, 0
L__interrupt32:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt33
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	ADDLW      255
	GOTO       L__interrupt32
L__interrupt33:
	MOVF       R0+0, 0
	IORWF      R3+0, 1
	MOVLW      4
	MOVWF      R2+0
	MOVF       _D+3, 0
	MOVWF      R0+0
	CLRF       R0+1
	MOVF       R2+0, 0
L__interrupt34:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt35
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	ADDLW      255
	GOTO       L__interrupt34
L__interrupt35:
	MOVF       R0+0, 0
	IORWF      R3+0, 0
	MOVWF      R4+0
	MOVLW      64
	ANDWF      RCREG+0, 0
	MOVWF      R1+0
	MOVLW      6
	MOVWF      R0+0
	MOVF       R1+0, 0
	MOVWF      R3+0
	MOVF       R0+0, 0
L__interrupt36:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt37
	RRF        R3+0, 1
	BCF        R3+0, 7
	ADDLW      255
	GOTO       L__interrupt36
L__interrupt37:
	MOVLW      3
	MOVWF      R2+0
	MOVF       R3+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	MOVF       R2+0, 0
L__interrupt38:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt39
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	ADDLW      255
	GOTO       L__interrupt38
L__interrupt39:
	MOVF       R0+0, 0
	IORWF      R4+0, 1
	MOVLW      32
	ANDWF      RCREG+0, 0
	MOVWF      R1+0
	MOVLW      5
	MOVWF      R0+0
	MOVF       R1+0, 0
	MOVWF      R3+0
	MOVF       R0+0, 0
L__interrupt40:
	BTFSC      STATUS+0, 2
	GOTO       L__interrupt41
	RRF        R3+0, 1
	BCF        R3+0, 7
	ADDLW      255
	GOTO       L__interrupt40
L__interrupt41:
	MOVF       R3+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	MOVF       R0+0, 0
	IORWF      R4+0, 1
	MOVLW      16
	ANDWF      RCREG+0, 0
	MOVWF      R1+0
	MOVF       R1+0, 0
	MOVWF      R3+0
	RRF        R3+0, 1
	BCF        R3+0, 7
	RRF        R3+0, 1
	BCF        R3+0, 7
	RRF        R3+0, 1
	BCF        R3+0, 7
	RRF        R3+0, 1
	BCF        R3+0, 7
	MOVF       R3+0, 0
	MOVWF      R0+0
	CLRF       R0+1
	RLF        R0+0, 1
	RLF        R0+1, 1
	BCF        R0+0, 0
	MOVF       R0+0, 0
	IORWF      R4+0, 0
	MOVWF      R3+0
	MOVLW      8
	ANDWF      RCREG+0, 0
	MOVWF      R2+0
	MOVF       R2+0, 0
	MOVWF      R0+0
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	RRF        R0+0, 1
	BCF        R0+0, 7
	MOVLW      0
	MOVWF      R0+1
	MOVF       R3+0, 0
	IORWF      R0+0, 1
	MOVF       R0+0, 0
	MOVWF      _Terminal_Word+0
	CALL       _byte2double+0
	MOVLW      0
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVLW      32
	MOVWF      R4+2
	MOVLW      129
	MOVWF      R4+3
	CALL       _Mul_32x32_FP+0
	MOVLW      0
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVLW      0
	MOVWF      R4+2
	MOVLW      137
	MOVWF      R4+3
	CALL       _Div_32x32_FP+0
	MOVLW      0
	MOVWF      R4+0
	MOVLW      0
	MOVWF      R4+1
	MOVLW      72
	MOVWF      R4+2
	MOVLW      133
	MOVWF      R4+3
	CALL       _Mul_32x32_FP+0
	MOVF       R0+0, 0
	MOVWF      _Celsius+0
	MOVF       R0+1, 0
	MOVWF      _Celsius+1
	MOVF       R0+2, 0
	MOVWF      _Celsius+2
	MOVF       R0+3, 0
	MOVWF      _Celsius+3
	MOVLW      ?lstr1_E2_Serial_Terminal+0
	MOVWF      FARG_Serial_Write_string+0
	CALL       _Serial_Write+0
	MOVLW      ?lstr2_E2_Serial_Terminal+0
	MOVWF      FARG_Serial_Write_string+0
	CALL       _Serial_Write+0
	MOVF       _Celsius+0, 0
	MOVWF      FARG_FloatToStr_FixLen_fnum+0
	MOVF       _Celsius+1, 0
	MOVWF      FARG_FloatToStr_FixLen_fnum+1
	MOVF       _Celsius+2, 0
	MOVWF      FARG_FloatToStr_FixLen_fnum+2
	MOVF       _Celsius+3, 0
	MOVWF      FARG_FloatToStr_FixLen_fnum+3
	MOVLW      _Text+0
	MOVWF      FARG_FloatToStr_FixLen_str+0
	MOVLW      5
	MOVWF      FARG_FloatToStr_FixLen_len+0
	CALL       _FloatToStr_FixLen+0
	MOVLW      _Text+0
	MOVWF      FARG_Serial_Write_string+0
	CALL       _Serial_Write+0
	MOVLW      ?lstr3_E2_Serial_Terminal+0
	MOVWF      FARG_Serial_Write_string+0
	CALL       _Serial_Write+0
	MOVLW      ?lstr4_E2_Serial_Terminal+0
	MOVWF      FARG_Serial_Write_string+0
	CALL       _Serial_Write+0
	CLRF       _Received+0
	BSF        RB6_bit+0, BitPos(RB6_bit+0)
	BCF        RB3_bit+0, BitPos(RB3_bit+0)
	BCF        RB0_bit+0, BitPos(RB0_bit+0)
L_interrupt16:
L_interrupt15:
L_interrupt9:
L_interrupt5:
L_end_interrupt:
L__interrupt25:
	MOVF       ___savePCLATH+0, 0
	MOVWF      PCLATH+0
	SWAPF      ___saveSTATUS+0, 0
	MOVWF      STATUS+0
	SWAPF      R15+0, 1
	SWAPF      R15+0, 0
	RETFIE
; end of _interrupt

_main:

	MOVLW      128
	MOVWF      OPTION_REG+0
	MOVLW      25
	MOVWF      SPBRG+0
	MOVLW      36
	MOVWF      TXSTA+0
	MOVLW      144
	MOVWF      RCSTA+0
	MOVLW      32
	MOVWF      PIE1+0
	MOVLW      192
	MOVWF      INTCON+0
	MOVLW      6
	MOVWF      TRISB+0
	CLRF       PORTB+0
L_main17:
	GOTO       L_main17
L_end_main:
	GOTO       $+0
; end of _main
