// **************************************************************************************************
// Prova N.3 - Questão 1 e 2  [Arquivo de inclusão]
// Programa em C - Microcontroladores E1 & E2
//
// Curso   : Eng. Elétrica da IFBA
// Disciplina: ENG421 - Microprocessadores & Microcontroladores
// Autora  : Dirlene Napoleão                         Criação: 11/12/2021  Rev.: 14/12/2021
// Clock   : 4MHz                                     Arquivo: SerialLib.h
// Programa: Funções de envio na comunicação serial
//
// *** DESCRIÇÃO IMPORTANTE DA COMUNICAÇÃO SERIAL ***
//
//  Este é um arquivo para declaração e armazenamento das funções de envio na Porta TX (Comunicação
//  Serial), definições dos bits do Bloco A e do Bloco B e definições dos bits necessários para
//  recepção dos dados na Porta RX da comunicação serial, no entanto, a recepção é executada na função 
//  de interrupção e não numa função própria pra ela (Como o do envio serial). Existem 2 tipos de envio:
//  de um dado único ou de uma String. O envio único é utilizado pelo Microcontrolador E1 para
//  enviar os "pacotes" (Blocos) do dado da temperatura pela linha de transmissão. Cada pacote tem 8 bits
//  , sendo 1 byte, a função Serial_Send envia apenas este byte. Já no envio de Strings é utilizado a
//  função Serial_Write pelo microcontrolador E2 que vai apresentar textos no terminal virtual, neste caso 
//  será o valor da temperatura em String (O valor em Celsius)
//  Durante a linha de transmissão, existe uma interferência eletromagnética que altera os dados
//  e esta interferência foi modelada utilizando um Pulso DC com amplitude de 5V, Largura de 3us e
//  Período de 2ms. Este pulso é o que vai adicionar o ruído no meio da transmissão, mas para o ruído
//  ser somado com o dado enviado, foi preciso criar o circuito de somador não-inversor com amplificador
//  operacional usando LM741. Os detalhes dos 2 sinais (Pulso e Saída) podem ser vistos no osciloscópio e no
//  voltímetro. No entanto, alguns testes foram feitos e concluí que o sinal aplicado (O Pulso) no somador
//  do amplificador afeta no "Tempo de simulação" do Proteus e isto aconteceu no meu computador.
//  Apenas este tipo de Pulso afeta no tempo, dando uma sobrecarga na CPU e perdendo a sincronia do tempo
//  entre o código e a simulação, ou seja, 10 segundos configurados no TMR0 do código deram quase 2 minutos
//  na simulação do Proteus. Então se caso acontecer este mesmo problema na simulação, observe os últimos 2
//  dígitos do módulo de temporização do Proteus e espere eles chegarem a "10". Ou afim de testar com o tempo
//  em sincronia (Caso acontecer o problema), só alterar o Pulso para uma fonte DC e problema resolvido.
//  Apesar de que "parece" que os dados não são alterados durante a linha de transmissão, perceba que
//  em alguns momentos bem específicos que o LED vermelho pode acender rapidamente, indicando erro na transmissão,
//  mas isto acontece apenas em algumas voltagens do sensor LM53 e após tempos específicos, com uma certa,
//  "imprevisibilidade". E olhando pelo Voltímetro de saída, veja claramente que o sinal dos pulsos e do TX são 
//  somados. Logo esta alteração ocorre e o sistema de tratamento de erros é o suficiente pra conseguir recuperar
//  o dado rapidamente.
//
// **************************************************************************************************

#ifndef  __SERIALLIB_H__
#define  __SERIALLIB_H__

#define  HIGH      1                       // Nível lógico alto 1
#define  LOW       0                       // Nível lógico baixo 0

// Definições dos bits do dado enviado pelo E1
#define  B0   ((LM35_Value & 0x01) >> 0)   // Bit B0 do Valor da temperatura
#define  B1   ((LM35_Value & 0x02) >> 1)   // Bit B1 do Valor da temperatura
#define  B2   ((LM35_Value & 0x04) >> 2)   // Bit B2 do Valor da temperatura
#define  B3   ((LM35_Value & 0x08) >> 3)   // Bit B3 do Valor da temperatura
#define  B4   ((LM35_Value & 0x10) >> 4)   // Bit B4 do Valor da temperatura
#define  B5   ((LM35_Value & 0x20) >> 5)   // Bit B5 do Valor da temperatura
#define  B6   ((LM35_Value & 0x40) >> 6)   // Bit B6 do Valor da temperatura
#define  B7   ((LM35_Value & 0x80) >> 7)   // Bit B7 do Valor da temperatura

// Definições dos bits do dado recebido pelo E2
#define  D0   ((RCREG & 0x01) >> 0)   // Bit D0 do Valor da temperatura
#define  D1   ((RCREG & 0x02) >> 1)   // Bit D1 do Valor da temperatura
#define  D2   ((RCREG & 0x04) >> 2)   // Bit D2 do Valor da temperatura
#define  D3   ((RCREG & 0x08) >> 3)   // Bit D3 do Valor da temperatura
#define  D4   ((RCREG & 0x10) >> 4)   // Bit D4 do Valor da temperatura
#define  D5   ((RCREG & 0x20) >> 5)   // Bit D5 do Valor da temperatura
#define  D6   ((RCREG & 0x40) >> 6)   // Bit D6 do Valor da temperatura
//#define  D7   ((RCREG & 0x80) >> 7) // Bit D7 IGNORADO


// Declarações para ambos os microcontroladores de envio serial
void Serial_Send(unsigned short);          // Declarando a função de envio serial
void Serial_Write(char*);                  // Declarando a função de escrita serial
void Serial_Wait();                        // Declarando a função de espera do envio serial

sbit SERIAL_SENDED at TRMT_bit;            // Este bit é setado quando todo o buffer TXREG foi esvaziado (Bits enviados)

// Declarações para o microcontrolador E1
#ifdef  __E1_MICRO__
sbit TIMER_RESET   at    RD0_bit;          // Pino para resetar módulo de temporização do Proteus
sbit E2_BLOC_A     at    RD1_bit;          // Pino que identifica a recepção do Bloco A em E2
sbit E2_BLOC_B     at    RD2_bit;          // Pino que identifica a recepção do Bloco B em E2
sbit TMR0_OVERFLOW at    TMR0IF_bit;       // Este bit é setado quando houver overflow do TMR0
sbit EXTERN_INT0   at    INT0IF_bit;       // Este bit é setado quando RB0 está em HIGH e interrupção externa habilitada
#endif

// Declarações para o microcontrolador E2
#ifdef  __E2_MICRO__
sbit FRAMING_ERROR at FERR_bit;            // Bit que indica Erro de Framing na recepção serial (Pacotes imcompletos)
sbit OVERRUN_ERROR at OERR_bit;            // Bit que indica Erro de Overrun na recepção serial (Sobreposição de dados)
sbit RECEPTION     at CREN_bit;            // Bit para acionar/desacionar a recepção contínua
sbit RECEPTION_INT at RCIF_bit;            // Bit que indica interrupção por recepção serial, ou buffer cheio
sbit LED_GREEN     at RB6_bit;             // Bit relacionado ao LED verde    (Indicando término/sucesso)
sbit LED_YELLOW    at RB5_bit;             // Bit relacionado ao LED amarelo  (Indicando recepção do Bloco A)
sbit LED_RED       at RB4_bit;             // Bit relacionado ao LED vermelho (Indicando alteração dos dados)
sbit SIGN_E1       at RB7_bit;             // Bit relacionado a sinalização para E1 reenviar o Bloco A
sbit E1_BLOC_A     at RB3_bit;             // Bit que sinaliza E1 que bloco A foi recebido e resolvido
sbit E1_BLOC_B     at RB0_bit;             // Bit que sinaliza E1 que bloco B foi recebido e resolvido
#endif

// --------------------------------------------------------------------------------------------------
// --- Funções de comunicação serial ----------------------------------------------------------------

// Envia para porta serial um dado único de 8 bits
void Serial_Send(unsigned short val){
     TXREG = val;
     Serial_Wait();
}

// Envia para porta serial vários dados de 8 bits como "Strings"
void Serial_Write(char *string){
    int i;
    for(i = 0; i < strlen(string); i++){
        TXREG = string[i];
        Serial_Wait();
    }
}

// Função de espera do envio serial
void Serial_Wait(){
    while(!SERIAL_SENDED);
}

// --------------------------------------------------------------------------------------------------
// --- Fim das Funções de comunicação serial --------------------------------------------------------

#endif