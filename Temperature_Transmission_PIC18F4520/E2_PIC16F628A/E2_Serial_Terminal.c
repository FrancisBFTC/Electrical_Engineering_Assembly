// **************************************************************************************************
// Programa em C - Microcontrolador PIC16F628A (E2)
//
// Autor   : Wender Francis                           Cria��o: 11/12/2021  Rev.: 14/12/2021
// Clock   : 4MHz                                     Arquivo: E2_Serial_Terminal.c
// Programa: Recep��o dos blocos via comunica��o serial e escrita da temperatura no terminal
//
// *** DESCRI��O DO PROGRAMA ***
//
//  Este programa tem a fun��o de receber o dado serial vindo do microcontrolador E1 e fazer trata-
//  mento de erros. Ap�s E1 transmitir o dado dividido por 2 blocos a cada 10 segundos, Este programa 
//  do microcontrolador E2 trata cada Bloco individualmente para acopl�-los e envi�-los ao terminal. 
//  Este processo faz parte da 2� fase correspondente a Quest�o 2: Esta fase consiste em 5 etapas ->
//  1� - Ignorar o �ltimo bit do dado e calcular os 3 bits LSBs do dado utilizando a opera��o XOR.
//  2� - Comparar a cada bit dos 3 bits LSBs do dado com os 3 bits calculados.
//  3� - Se uma das compara��es forem diferentes, sinalizar o E1 para reenviar o dado.
//  4� - Se todas as compara��es forem iguais, ent�o mesclar os 4 bits do Bloco A com os 4 do Bloco B.
//  5� - Enviar ao terminal o dado completo de 8 bits.
//  Antes do envio, um c�lculo do dado � feito para ter o Float correspondente da temperatura e ap�s
//  isto, uma convers�o do Float para String � feita enviando ao terminal a String da temperatura.
//  Durante a recep��o, os LEDs de estado s�o ligados, indicando em qual momento o programa est�.
// **************************************************************************************************


// --------------------------------------------------------------------------------------------------
// --- Defini��es de dados e declara��es de fun��es & vari�veis -------------------------------------

#define    __E2_MICRO__                      // Para identifica��o do Micro E2 no caso da mesma inclus�o

// OBS.: Veja a descri��o no arquivo de fun��es serial
#include   "../SerialLib.h"                  // Inclui as declara��es e fun��es de comunica��o serial

unsigned short i = 0;                        // Vari�vel i para o loop FOR
unsigned short D_[3], D[7];                  // Vetores pra armazenar bits que ser�o comparados
unsigned short Logic_Val = 0;                // Vari�vel l�gica que determina o resultado da compara��o
unsigned short Received = 0;                 // Vari�vel pra identificar a ordem do dado: Se � o 1� ou 2�
unsigned short Terminal_Word = 0;            // Vari�vel que armazena o dado final do Bloco A + B
float  Celsius = 0.0;                        // Vari�vel que armazena o dado calculado da temperatura
char   Text[5];                              // Vetor de caracteres da convers�o do Float para Texto

// --- Fim das declara��es --------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
// --- Fun��o de interrup��o por Recep��o serial ----------------------------------------------------

void interrupt(){
     if(RECEPTION_INT){                        // Se bit RCIF de PIR1 for 1, ent�o Buffer Cheio e dado recebido
          if(FRAMING_ERROR || OVERRUN_ERROR){  // Se os BITs FERR ou OERR de RCSTA for 1, logo ouve erro de Framing ou Overrun (Pack. Imcompletos ou Sobreposi��o)
                RECEPTION = LOW;               // Logo, Desligue a recep��o cont�nua de RCSTA
                RECEPTION = HIGH;              // E Ligue novamente a recep��o cont�nua de RCSTA
          }else{                               // Se n�o, ent�o n�o houve erros e vamos efetuar as etapas da Fase 2
                
                Received++;                    // Received vale 0, incrementa +1 pra valer 1 e depois 2
                LED_GREEN = LOW;               // Desliga led verde
                LED_YELLOW = HIGH;             // Liga led amarelo indicando a chegada do Bloco
                     
                // A fase de verifica��o/corre��o de erros do microcontrolador E2 consta basicamente de cinco etapas:
                
                // 4� e 5� etapa: Sendo d7d6d5d4d3d2d1d0 uma palavra de 8 bits recebida, o bit mais significativo (d7) ser� ignorado e
                // Ser�o re-calculados os tr�s bits menos significativos como
                //
                //     D_2 = d6 ^ d5 ^ d3
                //     D_1 = d6 ^ d4 ^ d3
                //     D_0 = d5 ^ d4 ^ d3
                //

                RCREG = RCREG & 0b01111111;   // Ignora o bit d7
                
                // Recalculando os 3 bits
                D_[2] = D6 ^ D5 ^ D3;
                D_[1] = D6 ^ D4 ^ D3;
                D_[0] = D5 ^ D4 ^ D3;
                D[0] = D0;           // Salvando Di, onde i = 0, para compara��o
                D[1] = D1;           // Salvando Di, onde i = 1, para compara��o
                D[2] = D2;           // Salvando Di, onde i = 2, para compara��o

                // E comparar cada D_[i] com D[i], para i = 0, 1, 2, sendo 1 quando o resultado da compara��o � IGUAL e 0 quando o
                // resultado da compara��o � DIFERENTE.
                for(i = 0; i < 3; i++){
                    Logic_Val = !(D_[i] ^ D[i]);     // Correspondente ao XNOR ou == -> Retorna 1 se for igual
                                                     // O mesmo que !Di com Di (Sem par�nteses)

                    // 6� etapa. Se pelo menos uma das tr�s compara��es resulta em valor l�gico 0 ent�o o microcontrolador E2 deve sinalizar ao
                    // microcontrolador E1 para encaminhar novamente o dado. Processo que n�o deve superar os 10 segundos em que o
                    // microcontrolador E1 ir� atualizar sua leitura.
                    if(Logic_Val == 0){             // Se o valor l�gico for 0 (de uma das compara��es)
                        SIGN_E1 = HIGH;             // Sinalize E1 enviando ligado para RB7 de E2 que vai para RB0/INT0 de E1
                        LED_RED = HIGH;             // Ligue led vermelho indicando erro
                        LED_YELLOW = LOW;           // Desligue led amarelo
                        Received -= 1;              // -1 na vari�vel apontando pro mesmo bloco
                        SIGN_E1 = LOW;              // Desligue a sinaliza��o
                        break;                      // Quebre o Loop FOR agora mesmo
                    }
                }

              if(Received == 1 & Logic_Val){        // Se dado recebido for 1 e valor logico 1, logo Bloco A sem erros
                                                    // Isto significa que todas as compara��es resultaram em 1
                     E1_BLOC_A = HIGH;              // Avise ao microcontrolador E1 que o Bloco A foi resolvido com sucesso
                     // 7� etapa:  Se todas as compara��es resultam em valor l�gico 1 ent�o o microcontrolador deve salvar os bits d6d5d4d3
                     // (correspondentes aos 4 bits mais significativos da palavra b7b6b5b4b3b2b1b0), at� receber a seguinte palavra...

                     D[6] = D6;       // Salva o bit D6
                     D[5] = D5;       // Salva o bit D5
                     D[4] = D4;       // Salva o bit D4
                     D[3] = D3;       // Salva o bit D3
                     
                     LED_RED = LOW;           // Desligue LED vermelho
                     SIGN_E1 = LOW;           // Desligue a sinaliza��o
              }else{
                if(Received == 2 & Logic_Val){            // Se o dado recebido for 2  e valor logico 1, logo Bloco B sem erros
                      E1_BLOC_B = HIGH;                   // Avise ao microcontrolador E1 que o Bloco B foi resolvido com sucesso
                     // ... e, ap�s verificar que o item 3 � sucedido com essa palavra, acoplar os novos bits d'6 d'5 d'4 d'3
                     // (correspondentes aos 4 bits menos significativos da palavra b7b6b5b4b3b2b1b0).

                      // Acoplando os novos bits LSBs aos antigos bits MSBs salvos (Bloco A + Bloco B)
                      Terminal_Word = (D[6] << 7) | (D[5] << 6) | (D[4] << 5) | (D[3] << 4) | (D6 << 3) | (D5 << 2) | (D4 << 1) | (D3 << 0);

                      // Convers�o da voltagem para Float correspondente � temperatura
                      Celsius = ((Terminal_Word * 5.0) / 1024.0) * 100.0;
                      

                      //8� etapa: Enviar a palavra d6 d5 d4 d3 d'6 d'5 d'4 d'3 ao terminal.
                       Serial_Write("\r\n ************ \r\n\r\n");
                       Serial_Write("Temp.: ");
                       FloatToStr_Fixlen(Celsius, Text, 5);            // Convers�o de Float para String com limite de 5 chars
                       Serial_Write(Text);                             // Escreva no terminal o valor de Celsius com Limite de 5 caracteres
                       Serial_Write(" C");
                       Serial_Write("\r\n\r\n ************  \r\n");
                       Received = 0;
                       LED_GREEN = HIGH;       // Liga LED verde indicando que toda a leitura ocorreu com sucesso
                       E1_BLOC_A = LOW;        // Desligue os aviso do Bloco A pra pr�xima itera��o
                       E1_BLOC_B = LOW;        // Desligue os aviso do Bloco B pra pr�xima itera��o
                 }
              }
           
        }
     }
}

// --- Fim da Interrup��o ---------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
// --- Fun��o principal do programa -----------------------------------------------------------------

void main() {
     OPTION_REG = 0x80;   // 1000 0000 -> Desabilita Pull-Ups de PORTB
     SPBRG = 25;          // x = 25 -> Asynchronous High Speed (9600 Bauld Rate)
     TXSTA = 0b00100100;  // Don�t care, 8-bit transmi��o, enabled, Ass�ncrono, High Speed, TMRT=0, Non 9-bit parity.
     RCSTA = 0b10010000;  // Serial Port Enabled, 8-bit recep��o, Don�t care, recep��o cont�nua, unused (Async), Framming and Overrun Error =0, Non parity bit
     PIE1  = 0b00100000;  // <5> = Habilita Int. por Recep��o Serial. COMPs, EE, TMRs, TX -> Desabilitados.
     INTCON = 0b11000000; // Habilita interrup��es globais e perif�ricas.

     TRISB = 0x06;        // Apenas pino RB1 e RB2 como set (RX e TX), o resto, incluindo RB7, ser�o sa�das
     PORTB = 0x00;        // Inicialize PORTB
     
     // Loop infinito at� a chegada do pr�ximo bloco
     while(1){ }
}

// --- Fim da fun��o Principal ----------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------