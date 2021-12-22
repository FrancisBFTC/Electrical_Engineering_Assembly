// **************************************************************************************************
// Prova N.3 - Quest�o 1 e 2  [Arquivo de inclus�o]
// Programa em C - Microcontroladores E1 & E2
//
// Curso   : Eng. El�trica da IFBA
// Disciplina: ENG421 - Microprocessadores & Microcontroladores
// Autora  : Dirlene Napole�o                         Cria��o: 11/12/2021  Rev.: 14/12/2021
// Clock   : 4MHz                                     Arquivo: SerialLib.h
// Programa: Fun��es de envio na comunica��o serial
//
// *** DESCRI��O IMPORTANTE DA COMUNICA��O SERIAL ***
//
//  Este � um arquivo para declara��o e armazenamento das fun��es de envio na Porta TX (Comunica��o
//  Serial), defini��es dos bits do Bloco A e do Bloco B e defini��es dos bits necess�rios para
//  recep��o dos dados na Porta RX da comunica��o serial, no entanto, a recep��o � executada na fun��o 
//  de interrup��o e n�o numa fun��o pr�pria pra ela (Como o do envio serial). Existem 2 tipos de envio:
//  de um dado �nico ou de uma String. O envio �nico � utilizado pelo Microcontrolador E1 para
//  enviar os "pacotes" (Blocos) do dado da temperatura pela linha de transmiss�o. Cada pacote tem 8 bits
//  , sendo 1 byte, a fun��o Serial_Send envia apenas este byte. J� no envio de Strings � utilizado a
//  fun��o Serial_Write pelo microcontrolador E2 que vai apresentar textos no terminal virtual, neste caso 
//  ser� o valor da temperatura em String (O valor em Celsius)
//  Durante a linha de transmiss�o, existe uma interfer�ncia eletromagn�tica que altera os dados
//  e esta interfer�ncia foi modelada utilizando um Pulso DC com amplitude de 5V, Largura de 3us e
//  Per�odo de 2ms. Este pulso � o que vai adicionar o ru�do no meio da transmiss�o, mas para o ru�do
//  ser somado com o dado enviado, foi preciso criar o circuito de somador n�o-inversor com amplificador
//  operacional usando LM741. Os detalhes dos 2 sinais (Pulso e Sa�da) podem ser vistos no oscilosc�pio e no
//  volt�metro. No entanto, alguns testes foram feitos e conclu� que o sinal aplicado (O Pulso) no somador
//  do amplificador afeta no "Tempo de simula��o" do Proteus e isto aconteceu no meu computador.
//  Apenas este tipo de Pulso afeta no tempo, dando uma sobrecarga na CPU e perdendo a sincronia do tempo
//  entre o c�digo e a simula��o, ou seja, 10 segundos configurados no TMR0 do c�digo deram quase 2 minutos
//  na simula��o do Proteus. Ent�o se caso acontecer este mesmo problema na simula��o, observe os �ltimos 2
//  d�gitos do m�dulo de temporiza��o do Proteus e espere eles chegarem a "10". Ou afim de testar com o tempo
//  em sincronia (Caso acontecer o problema), s� alterar o Pulso para uma fonte DC e problema resolvido.
//  Apesar de que "parece" que os dados n�o s�o alterados durante a linha de transmiss�o, perceba que
//  em alguns momentos bem espec�ficos que o LED vermelho pode acender rapidamente, indicando erro na transmiss�o,
//  mas isto acontece apenas em algumas voltagens do sensor LM53 e ap�s tempos espec�ficos, com uma certa,
//  "imprevisibilidade". E olhando pelo Volt�metro de sa�da, veja claramente que o sinal dos pulsos e do TX s�o 
//  somados. Logo esta altera��o ocorre e o sistema de tratamento de erros � o suficiente pra conseguir recuperar
//  o dado rapidamente.
//
// **************************************************************************************************

#ifndef  __SERIALLIB_H__
#define  __SERIALLIB_H__

#define  HIGH      1                       // N�vel l�gico alto 1
#define  LOW       0                       // N�vel l�gico baixo 0

// Defini��es dos bits do dado enviado pelo E1
#define  B0   ((LM35_Value & 0x01) >> 0)   // Bit B0 do Valor da temperatura
#define  B1   ((LM35_Value & 0x02) >> 1)   // Bit B1 do Valor da temperatura
#define  B2   ((LM35_Value & 0x04) >> 2)   // Bit B2 do Valor da temperatura
#define  B3   ((LM35_Value & 0x08) >> 3)   // Bit B3 do Valor da temperatura
#define  B4   ((LM35_Value & 0x10) >> 4)   // Bit B4 do Valor da temperatura
#define  B5   ((LM35_Value & 0x20) >> 5)   // Bit B5 do Valor da temperatura
#define  B6   ((LM35_Value & 0x40) >> 6)   // Bit B6 do Valor da temperatura
#define  B7   ((LM35_Value & 0x80) >> 7)   // Bit B7 do Valor da temperatura

// Defini��es dos bits do dado recebido pelo E2
#define  D0   ((RCREG & 0x01) >> 0)   // Bit D0 do Valor da temperatura
#define  D1   ((RCREG & 0x02) >> 1)   // Bit D1 do Valor da temperatura
#define  D2   ((RCREG & 0x04) >> 2)   // Bit D2 do Valor da temperatura
#define  D3   ((RCREG & 0x08) >> 3)   // Bit D3 do Valor da temperatura
#define  D4   ((RCREG & 0x10) >> 4)   // Bit D4 do Valor da temperatura
#define  D5   ((RCREG & 0x20) >> 5)   // Bit D5 do Valor da temperatura
#define  D6   ((RCREG & 0x40) >> 6)   // Bit D6 do Valor da temperatura
//#define  D7   ((RCREG & 0x80) >> 7) // Bit D7 IGNORADO


// Declara��es para ambos os microcontroladores de envio serial
void Serial_Send(unsigned short);          // Declarando a fun��o de envio serial
void Serial_Write(char*);                  // Declarando a fun��o de escrita serial
void Serial_Wait();                        // Declarando a fun��o de espera do envio serial

sbit SERIAL_SENDED at TRMT_bit;            // Este bit � setado quando todo o buffer TXREG foi esvaziado (Bits enviados)

// Declara��es para o microcontrolador E1
#ifdef  __E1_MICRO__
sbit TIMER_RESET   at    RD0_bit;          // Pino para resetar m�dulo de temporiza��o do Proteus
sbit E2_BLOC_A     at    RD1_bit;          // Pino que identifica a recep��o do Bloco A em E2
sbit E2_BLOC_B     at    RD2_bit;          // Pino que identifica a recep��o do Bloco B em E2
sbit TMR0_OVERFLOW at    TMR0IF_bit;       // Este bit � setado quando houver overflow do TMR0
sbit EXTERN_INT0   at    INT0IF_bit;       // Este bit � setado quando RB0 est� em HIGH e interrup��o externa habilitada
#endif

// Declara��es para o microcontrolador E2
#ifdef  __E2_MICRO__
sbit FRAMING_ERROR at FERR_bit;            // Bit que indica Erro de Framing na recep��o serial (Pacotes imcompletos)
sbit OVERRUN_ERROR at OERR_bit;            // Bit que indica Erro de Overrun na recep��o serial (Sobreposi��o de dados)
sbit RECEPTION     at CREN_bit;            // Bit para acionar/desacionar a recep��o cont�nua
sbit RECEPTION_INT at RCIF_bit;            // Bit que indica interrup��o por recep��o serial, ou buffer cheio
sbit LED_GREEN     at RB6_bit;             // Bit relacionado ao LED verde    (Indicando t�rmino/sucesso)
sbit LED_YELLOW    at RB5_bit;             // Bit relacionado ao LED amarelo  (Indicando recep��o do Bloco A)
sbit LED_RED       at RB4_bit;             // Bit relacionado ao LED vermelho (Indicando altera��o dos dados)
sbit SIGN_E1       at RB7_bit;             // Bit relacionado a sinaliza��o para E1 reenviar o Bloco A
sbit E1_BLOC_A     at RB3_bit;             // Bit que sinaliza E1 que bloco A foi recebido e resolvido
sbit E1_BLOC_B     at RB0_bit;             // Bit que sinaliza E1 que bloco B foi recebido e resolvido
#endif

// --------------------------------------------------------------------------------------------------
// --- Fun��es de comunica��o serial ----------------------------------------------------------------

// Envia para porta serial um dado �nico de 8 bits
void Serial_Send(unsigned short val){
     TXREG = val;
     Serial_Wait();
}

// Envia para porta serial v�rios dados de 8 bits como "Strings"
void Serial_Write(char *string){
    int i;
    for(i = 0; i < strlen(string); i++){
        TXREG = string[i];
        Serial_Wait();
    }
}

// Fun��o de espera do envio serial
void Serial_Wait(){
    while(!SERIAL_SENDED);
}

// --------------------------------------------------------------------------------------------------
// --- Fim das Fun��es de comunica��o serial --------------------------------------------------------

#endif