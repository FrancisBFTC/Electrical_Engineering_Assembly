// **************************************************************************************************
// Programa em C - Microcontrolador PIC16F628A (E2)
//
// Autor   : Wender Francis                           Criação: 11/12/2021  Rev.: 14/12/2021
// Clock   : 4MHz                                     Arquivo: E2_Serial_Terminal.c
// Programa: Recepção dos blocos via comunicação serial e escrita da temperatura no terminal
//
// *** DESCRIÇÃO DO PROGRAMA ***
//
//  Este programa tem a função de receber o dado serial vindo do microcontrolador E1 e fazer trata-
//  mento de erros. Após E1 transmitir o dado dividido por 2 blocos a cada 10 segundos, Este programa 
//  do microcontrolador E2 trata cada Bloco individualmente para acoplá-los e enviá-los ao terminal. 
//  Este processo faz parte da 2ª fase correspondente a Questão 2: Esta fase consiste em 5 etapas ->
//  1ª - Ignorar o último bit do dado e calcular os 3 bits LSBs do dado utilizando a operação XOR.
//  2ª - Comparar a cada bit dos 3 bits LSBs do dado com os 3 bits calculados.
//  3ª - Se uma das comparações forem diferentes, sinalizar o E1 para reenviar o dado.
//  4ª - Se todas as comparações forem iguais, então mesclar os 4 bits do Bloco A com os 4 do Bloco B.
//  5ª - Enviar ao terminal o dado completo de 8 bits.
//  Antes do envio, um cálculo do dado é feito para ter o Float correspondente da temperatura e após
//  isto, uma conversão do Float para String é feita enviando ao terminal a String da temperatura.
//  Durante a recepção, os LEDs de estado são ligados, indicando em qual momento o programa está.
// **************************************************************************************************


// --------------------------------------------------------------------------------------------------
// --- Definições de dados e declarações de funções & variáveis -------------------------------------

#define    __E2_MICRO__                      // Para identificação do Micro E2 no caso da mesma inclusão

// OBS.: Veja a descrição no arquivo de funções serial
#include   "../SerialLib.h"                  // Inclui as declarações e funções de comunicação serial

unsigned short i = 0;                        // Variável i para o loop FOR
unsigned short D_[3], D[7];                  // Vetores pra armazenar bits que serão comparados
unsigned short Logic_Val = 0;                // Variável lógica que determina o resultado da comparação
unsigned short Received = 0;                 // Variável pra identificar a ordem do dado: Se é o 1ª ou 2ª
unsigned short Terminal_Word = 0;            // Variável que armazena o dado final do Bloco A + B
float  Celsius = 0.0;                        // Variável que armazena o dado calculado da temperatura
char   Text[5];                              // Vetor de caracteres da conversão do Float para Texto

// --- Fim das declarações --------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
// --- Função de interrupção por Recepção serial ----------------------------------------------------

void interrupt(){
     if(RECEPTION_INT){                        // Se bit RCIF de PIR1 for 1, então Buffer Cheio e dado recebido
          if(FRAMING_ERROR || OVERRUN_ERROR){  // Se os BITs FERR ou OERR de RCSTA for 1, logo ouve erro de Framing ou Overrun (Pack. Imcompletos ou Sobreposição)
                RECEPTION = LOW;               // Logo, Desligue a recepção contínua de RCSTA
                RECEPTION = HIGH;              // E Ligue novamente a recepção contínua de RCSTA
          }else{                               // Se não, então não houve erros e vamos efetuar as etapas da Fase 2
                
                Received++;                    // Received vale 0, incrementa +1 pra valer 1 e depois 2
                LED_GREEN = LOW;               // Desliga led verde
                LED_YELLOW = HIGH;             // Liga led amarelo indicando a chegada do Bloco
                     
                // A fase de verificação/correção de erros do microcontrolador E2 consta basicamente de cinco etapas:
                
                // 4ª e 5ª etapa: Sendo d7d6d5d4d3d2d1d0 uma palavra de 8 bits recebida, o bit mais significativo (d7) será ignorado e
                // Serão re-calculados os três bits menos significativos como
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
                D[0] = D0;           // Salvando Di, onde i = 0, para comparação
                D[1] = D1;           // Salvando Di, onde i = 1, para comparação
                D[2] = D2;           // Salvando Di, onde i = 2, para comparação

                // E comparar cada D_[i] com D[i], para i = 0, 1, 2, sendo 1 quando o resultado da comparação é IGUAL e 0 quando o
                // resultado da comparação é DIFERENTE.
                for(i = 0; i < 3; i++){
                    Logic_Val = !(D_[i] ^ D[i]);     // Correspondente ao XNOR ou == -> Retorna 1 se for igual
                                                     // O mesmo que !Di com Di (Sem parênteses)

                    // 6ª etapa. Se pelo menos uma das três comparações resulta em valor lógico 0 então o microcontrolador E2 deve sinalizar ao
                    // microcontrolador E1 para encaminhar novamente o dado. Processo que não deve superar os 10 segundos em que o
                    // microcontrolador E1 irá atualizar sua leitura.
                    if(Logic_Val == 0){             // Se o valor lógico for 0 (de uma das comparações)
                        SIGN_E1 = HIGH;             // Sinalize E1 enviando ligado para RB7 de E2 que vai para RB0/INT0 de E1
                        LED_RED = HIGH;             // Ligue led vermelho indicando erro
                        LED_YELLOW = LOW;           // Desligue led amarelo
                        Received -= 1;              // -1 na variável apontando pro mesmo bloco
                        SIGN_E1 = LOW;              // Desligue a sinalização
                        break;                      // Quebre o Loop FOR agora mesmo
                    }
                }

              if(Received == 1 & Logic_Val){        // Se dado recebido for 1 e valor logico 1, logo Bloco A sem erros
                                                    // Isto significa que todas as comparações resultaram em 1
                     E1_BLOC_A = HIGH;              // Avise ao microcontrolador E1 que o Bloco A foi resolvido com sucesso
                     // 7ª etapa:  Se todas as comparações resultam em valor lógico 1 então o microcontrolador deve salvar os bits d6d5d4d3
                     // (correspondentes aos 4 bits mais significativos da palavra b7b6b5b4b3b2b1b0), até receber a seguinte palavra...

                     D[6] = D6;       // Salva o bit D6
                     D[5] = D5;       // Salva o bit D5
                     D[4] = D4;       // Salva o bit D4
                     D[3] = D3;       // Salva o bit D3
                     
                     LED_RED = LOW;           // Desligue LED vermelho
                     SIGN_E1 = LOW;           // Desligue a sinalização
              }else{
                if(Received == 2 & Logic_Val){            // Se o dado recebido for 2  e valor logico 1, logo Bloco B sem erros
                      E1_BLOC_B = HIGH;                   // Avise ao microcontrolador E1 que o Bloco B foi resolvido com sucesso
                     // ... e, após verificar que o item 3 é sucedido com essa palavra, acoplar os novos bits d'6 d'5 d'4 d'3
                     // (correspondentes aos 4 bits menos significativos da palavra b7b6b5b4b3b2b1b0).

                      // Acoplando os novos bits LSBs aos antigos bits MSBs salvos (Bloco A + Bloco B)
                      Terminal_Word = (D[6] << 7) | (D[5] << 6) | (D[4] << 5) | (D[3] << 4) | (D6 << 3) | (D5 << 2) | (D4 << 1) | (D3 << 0);

                      // Conversão da voltagem para Float correspondente à temperatura
                      Celsius = ((Terminal_Word * 5.0) / 1024.0) * 100.0;
                      

                      //8ª etapa: Enviar a palavra d6 d5 d4 d3 d'6 d'5 d'4 d'3 ao terminal.
                       Serial_Write("\r\n ************ \r\n\r\n");
                       Serial_Write("Temp.: ");
                       FloatToStr_Fixlen(Celsius, Text, 5);            // Conversão de Float para String com limite de 5 chars
                       Serial_Write(Text);                             // Escreva no terminal o valor de Celsius com Limite de 5 caracteres
                       Serial_Write(" C");
                       Serial_Write("\r\n\r\n ************  \r\n");
                       Received = 0;
                       LED_GREEN = HIGH;       // Liga LED verde indicando que toda a leitura ocorreu com sucesso
                       E1_BLOC_A = LOW;        // Desligue os aviso do Bloco A pra próxima iteração
                       E1_BLOC_B = LOW;        // Desligue os aviso do Bloco B pra próxima iteração
                 }
              }
           
        }
     }
}

// --- Fim da Interrupção ---------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------
// --- Função principal do programa -----------------------------------------------------------------

void main() {
     OPTION_REG = 0x80;   // 1000 0000 -> Desabilita Pull-Ups de PORTB
     SPBRG = 25;          // x = 25 -> Asynchronous High Speed (9600 Bauld Rate)
     TXSTA = 0b00100100;  // Don´t care, 8-bit transmição, enabled, Assíncrono, High Speed, TMRT=0, Non 9-bit parity.
     RCSTA = 0b10010000;  // Serial Port Enabled, 8-bit recepção, Don´t care, recepção contínua, unused (Async), Framming and Overrun Error =0, Non parity bit
     PIE1  = 0b00100000;  // <5> = Habilita Int. por Recepção Serial. COMPs, EE, TMRs, TX -> Desabilitados.
     INTCON = 0b11000000; // Habilita interrupções globais e periféricas.

     TRISB = 0x06;        // Apenas pino RB1 e RB2 como set (RX e TX), o resto, incluindo RB7, serão saídas
     PORTB = 0x00;        // Inicialize PORTB
     
     // Loop infinito até a chegada do próximo bloco
     while(1){ }
}

// --- Fim da função Principal ----------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------