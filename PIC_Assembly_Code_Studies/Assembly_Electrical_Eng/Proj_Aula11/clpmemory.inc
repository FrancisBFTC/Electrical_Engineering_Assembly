CBLOCK 		H'20'
	W_TEMP
	STATUS_TEMP
	PCLATH_TEMP
	TEMP
ENDC

#define 	I0  PORTD
#define 	Q0  PORTB

; ---- Saídas ----

#define		C1	Q0,0
#define 	C2  Q0,1 	

; ---- Entradas ----

#define 	B0 	I0,0
#define 	B1 	I0,1
#define 	B2 	I0,2