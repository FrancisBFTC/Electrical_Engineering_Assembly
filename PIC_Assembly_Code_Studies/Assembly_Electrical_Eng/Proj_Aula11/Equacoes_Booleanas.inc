; Programas com Equações booleanas
	
	BANK_SELECT 	0

	LD 	 	C1
	OR 	 	B1
	ANDN 	B0
	ANDN 	B2
	ANDN 	C2
	OUT 	C1   ; C1 = -B0&-B2&-C2&(C1|B1)

	LD 	 	C2
	OR 	 	B2
	ANDN 	B0
	ANDN 	B1
	ANDN 	C1
	OUT 	C2   ; C2 = -B0&-B1&-C1&(C2|B2)