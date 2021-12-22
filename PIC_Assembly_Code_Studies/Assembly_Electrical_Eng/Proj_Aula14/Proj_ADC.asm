
_main:

	MOVLW      1
	MOVWF      TRISIO+0
	MOVLW      17
	MOVWF      ANSEL+0
	MOVLW      7
	MOVWF      CMCON+0
	MOVLW      1
	MOVWF      ADCON0+0
	CLRF       GPIO+0
L_main0:
	CLRF       FARG_ADC_Read_channel+0
	CALL       _ADC_Read+0
	MOVF       R0+0, 0
	MOVWF      _ADC_Value+0
	MOVF       R0+1, 0
	MOVWF      _ADC_Value+1
	MOVLW      0
	XORWF      R0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main10
	MOVLW      0
	XORWF      R0+0, 0
L__main10:
	BTFSS      STATUS+0, 2
	GOTO       L_main2
	BSF        GPIO+0, 1
	BCF        GPIO+0, 2
	BCF        GPIO+0, 4
	BCF        GPIO+0, 5
	GOTO       L_main3
L_main2:
	MOVF       _ADC_Value+1, 0
	XORLW      1
	BTFSS      STATUS+0, 2
	GOTO       L__main11
	MOVLW      0
	XORWF      _ADC_Value+0, 0
L__main11:
	BTFSS      STATUS+0, 2
	GOTO       L_main4
	BCF        GPIO+0, 1
	BSF        GPIO+0, 2
	BCF        GPIO+0, 4
	BCF        GPIO+0, 5
	GOTO       L_main5
L_main4:
	MOVF       _ADC_Value+1, 0
	XORLW      2
	BTFSS      STATUS+0, 2
	GOTO       L__main12
	MOVLW      0
	XORWF      _ADC_Value+0, 0
L__main12:
	BTFSS      STATUS+0, 2
	GOTO       L_main6
	BCF        GPIO+0, 1
	BCF        GPIO+0, 2
	BSF        GPIO+0, 4
	BCF        GPIO+0, 5
	GOTO       L_main7
L_main6:
	MOVF       _ADC_Value+1, 0
	XORLW      2
	BTFSS      STATUS+0, 2
	GOTO       L__main13
	MOVLW      238
	XORWF      _ADC_Value+0, 0
L__main13:
	BTFSS      STATUS+0, 2
	GOTO       L_main8
	BCF        GPIO+0, 1
	BCF        GPIO+0, 2
	BCF        GPIO+0, 4
	BSF        GPIO+0, 5
L_main8:
L_main7:
L_main5:
L_main3:
	GOTO       L_main0
L_end_main:
	GOTO       $+0
; end of _main
