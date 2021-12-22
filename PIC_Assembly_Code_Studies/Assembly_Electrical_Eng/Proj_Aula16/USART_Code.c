// C�lculo Bauld Rate p/ USART:
//   Determinar x ->
//   Bauld = 4000000 Hz / (64 . (x + 1))
//   x = 4000000 / (64 . 9600) - 1
//   x = 5,5  -> Asynchronous Low Speed
//   x = 25   -> Asynchronous High Speed
//   Asynchronous Low Speed = Fosc/(64 . x + 1)  -> BRGH = 0
//   Asynchronous High Speed = Fosc/(16 . x + 1) -> BRGH = 1
//   synchronous Low Speed = Fosc/(4 . x + 1)    -> BRGH = 0


void Wait_Trans();
void Print();
void Write(char*);

// M�todo de Interrup��o por USART Receive
void interrupt(){
     if(RCIF_bit){      // Se bit RCIF de PIR1 for 1, ent�o Buffer Cheio
          if(FERR_bit || OERR_bit){  // Se os BITs FERR ou OERR de RCSTA for 1, logo ouve erro de Framing ou Overrun (Pack. Imcompletos ou Sobreposi��o)
                CREN_bit = 0x00;     // Logo, Desligue a recep��o cont�nua de RCSTA
                CREN_bit = 0x01;     // E Ligue novamente a recep��o cont�nua de RCSTA
          }else{
                RA0_bit = 0x00;
                delay_ms(10);
                RA1_bit = 0x01;
                Print();
          }
     }
}

// M�todo principal
void main() {

     OPTION_REG = 0x80;   // 1000 0000 -> Desabilita Pull-Ups de PORTB
     SPBRG = 25;          // x = 25 -> Asynchronous High Speed (9600 Bauld Rate)
     TXSTA = 0b00100100;  // Don�t care, 8-bit transmi��o, enabled, Ass�ncrono, High Speed, TMRT=0, Non 9-bit parity.
     RCSTA = 0b10010000;  // Serial Port Enabled, 8-bit recep��o, Don�t care, recep��o cont�nua, unused (Async), Framming and Overrun Error =0, Non parity bit
     PIE1 = 0b00100000;   // <5> = Habilita Int. por Recep��o Serial. COMPs, EE, TMRs, TX -> Desabilitados.
     INTCON = 0b11000000; // Habilita interrup��es globais e perif�ricas.
     
     TRISA = 0xF0;        // MSB como entrada e LSB como sa�da em PORTA (RA0 e RA1)
     TRISB = 0x02;        // Apenas pino RB1 como entrada (RX)
     PORTB = 0x00;        // Inicializar PORTB
     PORTA = 0x00;        // Inicializar PORTA
     
     Write("Command : \r\n");
     while(1);
}

void print(){
    TXREG = RCREG;        // Envie de volta pro terminal o dado recebido.
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
    while(!TRMT_bit);  // Enquanto o TXREG estiver cheio (TRMT == 0)
}
