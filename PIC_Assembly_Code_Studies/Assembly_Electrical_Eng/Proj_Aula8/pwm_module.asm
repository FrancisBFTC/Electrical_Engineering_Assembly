; Aula de Módulo PWM
;
;

list p = 16f628A
 
 #include <p16f628a.inc>

__config _XT_OSC & _WDT_OFF & _PWRTE_ON & _BOREN_OFF & _LVP_OFF & _CPD_OFF & _CP_OFF & _MCLRE_OFF

#define banco1 bsf STATUS,RP0
#define banco0 bcf STATUS,RP0

#define BOT    PORTB,RB0

; Registros de propósitos gerais
cblock 		H'20'
	W_TEMP			;0x20
	STATUS_TEMP		;0x21
	REG1	
endc

; Vetor de RESET
		org 	0000h
		goto 	inicio

; Vetor de INTERRUPÇÃO
		org 	0004h

; Salvar Contexto (STATUS,W)
; no banco 0
	movwf 	W_TEMP    ; P. EX.: W = 0010 1011 então W_TEMP = 0010 1011
	swapf 	STATUS,W  ; P. EX.: STATUS = 0001 0000 -> swap -> STATUS = 0000 0001
					  ; Não altera nenhum BIT do STATUS (movf & movwf altera)
	banco0
	movwf 	STATUS_TEMP  ; P. EX.: STATUS_TEMP = 0000 0001
; ---- ISR -------------------------

; tratamento da interrupção do TMR0

	btfss 	INTCON,T0IF			; Ouve Alguma Interrupção? (BIT 1 Setado?)
	goto 	sair				; Não! Então saia da ISR
	bcf 	INTCON,T0IF			; Sim! Então limpe o BIT 1 via software (Como indicado pelo fabricante)
	clrf 	TMR0 				; Limpe o TIMER0 e inicie a contagem
	btfss 	BOT					; Botão foi pressionado?
	goto 	sair				; Não! Então saia
	goto 	incrementar			; Sim, Então incremente PWM

incrementar:
	movlw 	D'255'				; W = 255
	xorwf 	CCPR1L,W			; Compara se CCPR1L é igual a W
	btfss 	STATUS,Z			; Testa BIT Z de STATUS
	goto 	Diff				; Z = 0? XOR não deu 0, então comparação deu Diferente, incremente CCPR1L								
	goto 	Equal				; Z = 1? XOR deu 0, então comparação deu Igual
	
Diff:
	incf 	CCPR1L,F
	goto 	sair

; Aumenta o Duty Cicle inicial a cada vez que entra aqui
Equal:
	movlw 	D'5'
	addwf 	REG1,F
	movf 	REG1,W
	movwf 	CCPR1L
	goto 	sair
	
sair:
; -----------------------------------
	swapf 	STATUS_TEMP,W  ; P. EX.: STATUS_TEMP = 0001 0000 --> W
	movwf 	STATUS
	swapf 	W_TEMP,F       ; P. EX.: W_TEMP = 1011 0010
	swapf 	W_TEMP,W       ; W = 0010 1011
						   ; Substitui a instrução movf W_TEMP, que modifica o BIT Z do STATUS
	
	retfie




; Fim da Interrupção

; PWM Period (T_pwm) = (PR2 + 1) x Ciclomaquina x TRM2 Prescale -> TRM2_P
; PR2 = 255
; Ciclomaquina = 1us
; TRM2_P = 16
; T_pwm = 256 x 1us x 16 = 4096us = 4.1ms
;
; PWM Duty Cycle (T_pdc) = (CCPR1L:CCP1CON<5:4>) x Tosc x TRM2 Prescale -> TRM2_P
; CCPR1L = 00 (+1....+N) -> Valor Hipotético = 20
; Tosc = 1/4MHz = 250ns
; TRM2_P = 16
; T_pdc = 20 x 250ns x 16 = 80000ns = 80us
;
; TMR0 = TRM0 x Cm x P_trm0 | P_trm0 = 256
; TRM0 = 65536us = 65.5ms
; Quanto maior o CCPR1L, maior a largura do Duty Cycle
; Quanto menor o PR2, menor é o período de Tempo
; Quanto maior o TRM0, maior a velocidade do aumento do DutyCycle

inicio:
	banco1
	bsf 	TRISB,RB0
	bcf 	TRISB,RB3
	movlw 	H'87' 		  ; W = B'10000111' -> 87h
	movwf 	OPTION_REG    ; BIT <7> de OPTION_REG é 1, logo desabilita pull-up interno
						  ; BIT <0:2>, sendo 111 -> TRM0 Prescaler = 256
						  ; Quanto menor os 3 bits, mais rápido é a contagem do PWM
	movlw 	H'FF'		  ; W = D'255'
	movwf 	PR2			  ; Garantir um período de 4.1ms pra o PWM na fórmula
	banco0
	movlw 	H'E0' 		  ; W = B'11100000'
	movwf 	INTCON		  ; <7> = Habilita interrupções globais, <6> = Habilita interrupções periférica
						  ; <5> = Habilita Interrupção por Overflow TIMER0
	movlw 	H'06' 		  ; W = B'00000110'
	movwf 	T2CON 		  ; BIT <1> setado = 1:16 TRM2 prescaler, BIT <2> setado habilita TIMER2
	movlw 	H'0C'		  ; CCPR1L:CCP1CON = BIT<7:0>:BIT<5:4>
	movwf 	CCP1CON 
	movlw 	H'00'
	movwf 	CCPR1L
	
	clrf 	REG1
	clrf 	TMR0 		  ; Limpe o TIMER0 e inicie a contagem
	
	goto 	$
	
loop:
	
end