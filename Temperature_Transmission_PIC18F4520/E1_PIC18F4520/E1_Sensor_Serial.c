// **************************************************************************************************
// Programa em C - Microcontrolador PIC18F4520 (E1)
//
// Autor   : Wender Francis                           Cria��o: 11/12/2021  Rev.: 14/12/2021
// Clock   : 4MHz                                     Arquivo: E1_Sensor_Serial.c
// Programa: Leitura de temperatura com transmiss�o para Microcontrolador E2
//
// *** DESCRI��O DO PROGRAMA ***
//
//  Este programa � respons�vel por iniciar a 1� fase correspondente a Quest�o 1:
//  Esta fase consiste em 3 etapas -> 1� - Ler o dado anal�gico do sensor LM35 dividindo-o
//  em 2 blocos de 8 bits chamados de "Bloco A" e "Bloco B" com os 3 bits menos significativos
//  calculados de cada Bloco; 2� - Transmitir via serial o 1� Bloco para o microcontrolador E2
//  at� que o microcontrolador E2 sinalize a este microcontrolador E1 que o Bloco A foi recebido
//  e calculado; 3� - Transmitir via serial o 2� Bloco e esperar a pr�xima sinaliza��o para o E2
//  efetuar as mesmas verifica��es. Esta fase � processada pela interrup��o ap�s overflow do TMR0.
//  O TMR0 conta 254 vezes, cada contagem com 256 ciclos e a cada vez que entra na interrup��o,
//  o contador "Counter" � incrementado at� 154, totalizando 254x256x154 = 10013ms. Isto �, a cada 10s
//  este processo ocorre. A fun��o principal configura a inicializa��o do m�dulo USART com 9600 Baulds
//  Habilita as interrup��es, habilita o temporizador e determina a dire��o dos pinos.
// **************************************************************************************************

// --------------------------------------------------------------------------------------------------
// --- Defini��es de dados e declara��es de fun��es & vari�veis -------------------------------------

#define    __E1_MICRO__                    // Para identifica��o do Micro E1 no caso da mesma inclus�o

// OBS.: Veja a descri��o no arquivo de fun��es serial
#include   "../SerialLib.h"                // Inclui as declara��es e fun��es de comunica��o serial

unsigned short LM35_Value = 0;             // Palavra de 8 bits que corresponde ao dado armazenado ap�s leitura de temperatura
unsigned short A_ = 0, B_ = 0;             // 2 Blocos de 8 bits -> Bloco A e Bloco B
unsigned short _a = 0, _b = 0, _c = 0;     // Bits "abc" de A (3 bits menos significativos)
unsigned short _d = 0, _e = 0, _f = 0;     // Bits "def" de B (3 bits menos significativos)
unsigned short Counter = 0;                // Contador auxiliar de Tempo

// --- Fim das declara��es --------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
// --- Fun��o de interrup��o por TMR0 e RB0/INT0 ----------------------------------------------------

void interrupt(){
      if(TMR0_OVERFLOW){                          // Houve Interrup��o por TMR0?
          TMR0_OVERFLOW = LOW;                    // Sim, ent�o limpe a flag.
          Counter++;                              // Incremente o contador para o temporizador
          if(Counter == 153){                     // Se for igual, significa que j� passou 10s pois 254x256x154=10s
              LM35_Value = ADC_Read(0);           // Captura a palavra de 8 bits da temperatura do LM35

              TIMER_RESET = HIGH;                 // O Pino RESET do TIMER reseta de LOW para HIGH
              TIMER_RESET = LOW;                  // Por este motivo, Colocamos em HIGH e depois em LOW
                                                  // pra pr�xima vez, resetar em HIGH
                                                  
              Counter = 0;                        // Zeramos Counter para reiniciar a contagem do 0
              TMR0L = 2;                          // Redefinimos o TMR0 de 8 Bits ap�s o Overflow
              
            // A fase de leitura e transmiss�o do microcontrolador E1 consta basicamente de tr�s etapas:
            
            // 1� etapa: Sendo b7b6b5b4b3b2b1b0 a palavra de 8 bits que corresponde ao dado armazenado ap�s leitura de temperatura, o
            // microcontrolador deve criar dois blocos de 8 bits, a saber A = 0b7b6b5b4|abc, B = 0b3b2b1b0|def,
            // sendo
            // a = b7 ^ b6 ^ b4      d = b3 ^ b2 ^ b0
            // b = b7 ^ b5 ^ b4      e = b3 ^ b1 ^ b0
            // c = b6 ^ b5 ^ b4      f = b2 ^ b1 ^ b0
            // onde ^ representa a opera��o l�gica XOR.

            // Preparando os 3 bits menos significativos de A e B
              _a = B7 ^ B6 ^ B4;    _d = B3 ^ B2 ^ B0;
              _b = B7 ^ B5 ^ B4;    _e = B3 ^ B1 ^ B0;
              _c = B6 ^ B5 ^ B4;    _f = B2 ^ B1 ^ B0;

            // Mesclando os bits do Bloco A e B
               A_ = 0 | (B7 << 6) | (B6 << 5) | (B5 << 4) | (B4 << 3) | (_a << 2) | (_b << 1) | (_c << 0);   // Bloco A de 8 bits
               B_ = 0 | (B3 << 6) | (B2 << 5) | (B1 << 4) | (B0 << 3) | (_d << 2) | (_e << 1) | (_f << 0);   // Bloco B de 8 bits

            // 2� etapa: Enviar A (Transmiss�o da palavra A) para o microcontrolador E2.

               Serial_Send(A_);                // Envie A para E2 e aguarde buffer esvaziar
               while(!E2_BLOC_A);              // Enquanto E2 faz a verifica��o de erros do Bloco A, trave aqui
            
            // 3� etapa: Enviar B (Transmiss�o da palavra B) para o microcontrolador E2, sendo que o tempo de transmiss�o das duas palavras
            // n�o pode ultrapassar os 10 segundos em que o microcontrolador ir� atualizar sua leitura. Isto porque s� atualiza sua leitura
            // na interrup��o do TIMER0 e ap�s isto, os dados anteriores em LM35_Value s�o perdidos.

               Serial_Send(B_);                // Envie B para E2 e aguarde buffer esvaziar
               while(!E2_BLOC_B);              // Enquanto E2 faz a verifica��o de erros do Bloco B, trave aqui

            
          } // Fim do IF Counter
      }else{                                   // N�o, n�o houve interrup��o por TMR0.
            if(EXTERN_INT0){                   // Mas a interrup��o foi externa?
                 EXTERN_INT0 = LOW;            // Sim ent�o limpe a flag.
                 if(!E2_BLOC_A)                // Se Bloco A n�o teve sucesso
                     Serial_Send(A_);          // Reenvie o Bloco A novamente
                 else                          // Mas se Bloco B que n�o teve sucesso
                     Serial_Send(B_);          // Reenvie o Bloco B novamente
            }
      }
}

// --- Fim da Interrup��o ---------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
// --- Fun��o principal do programa -----------------------------------------------------------------

void main() {
     TXSTA   = 0x24;       // 0010 0100 -> Clks Individuais, 8-bit transmi��o, enabled, Ass�ncrono, High Speed, TMRT=0, Non 9-bit parity.
     RCSTA   = 0x80;       // 1000 0000 -> Habilita porta serial, sem interrup��o por recep��o.
     SPBRG   = 25;         // x = 25    -> Asynchronous High Speed (9600 Bauld Rate)
     BAUDCON = 0x00;       // 0000 0000 -> Apenas para garantir que todos est�o zerados (N�o precisaremos deles)
     INTCON  = 0xF0;       // 1111 0000 -> Habilita interrup��es globais, perif�ricas, Externa INT0/1/2 e TMR0 Overflow
     INTCON2 = 0xC4;       // 1100 0100 -> Pull-Ups desabilitados de PORTB, Int. por subida em INT0, Alta prioridade para TMR0
     T0CON   = 0xC7;       // 1100 0111 -> Habilita TMR0 configurado para 8 bit, Prescaler = 1:256
     
     TRISB = 0x01;         // 0000 0001 -> Apenas pino RB0/INT0 (Interrup��o externa) como entrada
     TRISC = 0xC0;         // 1100 0000 -> TRISC<7> e TRISC<6> devem ser setados para habilita��o serial
     TRISA = 0xFF;         // 1111 1111 -> PORTA tudo como entrada (Para AN0 ser entrada)
     TRISD = 0x06;         // 0000 0110 -> PORTD com pino RD1 e RD2 (recep��o Bloco A e B) como entrada, o resto como sa�da
     PORTA = 0x00;         // Inicializar PORTA
     PORTB = 0x00;         // Inicializar PORTB
     PORTC = 0x00;         // Inicializar PORTC
     PORTD = 0x00;         // Inicializar PORTD

     ADCON0 = 0x01;        // 0000 0001 -> Habilita o m�dulo de convers�o A/D
     ADCON1 = 0x0E;        // 0000 1110 -> Garante que AN0 funcione como entrada anal�gica

     delay_ms(100);        // Tempo de 100ms para configura��o
     
     // Aqui eu causarei um overflow proposital para j� entrar na interrup��o
     // E enviar na 1� vez o dado serial, na int. os contadores s�o redefinidos
     // para iniciar do zero e contar at� 10s
     Counter = 152;        // Inicie com 152 para j� iniciar as opera��es na INT.
     TMR0L = 255;          // Ap�s 256 ciclos no While abaixo, aqui ter� um Overflow
     
     while(1){}            // Trave aqui at� o TMR0 overflow causar a interrup��o
   
}

// --- Fim da fun��o Principal ----------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------