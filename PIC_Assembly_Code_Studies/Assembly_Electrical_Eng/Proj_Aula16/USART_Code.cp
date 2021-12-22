#line 1 "C:/Users/BFTC/Desktop/D.S.O.S/Systems/PIC Assembly Programming/Source-Code/ProjetoENG421_Especial/Proj_Aula16/USART_Code.c"
#line 12 "C:/Users/BFTC/Desktop/D.S.O.S/Systems/PIC Assembly Programming/Source-Code/ProjetoENG421_Especial/Proj_Aula16/USART_Code.c"
void Wait_Trans();
void Print();
void Write(char*);


void interrupt(){
 if(RCIF_bit){
 if(FERR_bit || OERR_bit){
 CREN_bit = 0x00;
 CREN_bit = 0x01;
 }else{
 RA0_bit = 0x00;
 delay_ms(10);
 RA1_bit = 0x01;
 Print();
 }
 }
}


void main() {

 OPTION_REG = 0x80;
 SPBRG = 25;
 TXSTA = 0b00100100;
 RCSTA = 0b10010000;
 PIE1 = 0b00100000;
 INTCON = 0b11000000;

 TRISA = 0xF0;
 TRISB = 0x02;
 PORTB = 0x00;
 PORTA = 0x00;

 Write("Command : \r\n");
 while(1);
}

void print(){
 TXREG = RCREG;
 Wait_Trans();
 RA0_bit = 0x01;
 delay_ms(50);
 RA1_bit = 0x00;
}

void write(char *string){
 int i;
 for(i = 0; i < strlen(string); i++){
 TXREG = string[i];
 Wait_Trans();
 }
}

void Wait_Trans(){
 while(!TRMT_bit);
}
