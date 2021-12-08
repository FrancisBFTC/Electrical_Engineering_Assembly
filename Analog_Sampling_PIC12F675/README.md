# Sistema de Amostragem em Assembly

Este sistema é responsável por classificar 10 amostras de sinais analógicos vindos da fonte **pwlin generator** no Proteus. Cada sinal deve ocorrer no tempo de 20ms de 0.01V
até 0.89V. Sendo a sequência _r(0ms) = 0.01V , r(20ms) = 0.01V , r(40ms) = r(0ms) + r(20ms), r(60ms) = r(20ms) + r(40ms), . . . , r(T) = 0.89V_. O sinal analógico é representado pelo sinal r(t),
e as 10 amostras deve ter o período total de T = 200ms. O sistema de amostragem no final também faz o calculo da  ́area abaixo da forma de onda por meio de uma aproximação de partição regular no 
dominio do tempo t. As seguintes operações são feitas neste projeto:

1. Configura registros iniciais e o temporizador.
2. Inicia o MAIN com um LOOP infinito.
3. Após identificar interrupção por Overflow do contador de tempo, entra na interrupção.
4. Na interrupção (Após 20ms), salva os contextos e espera pela conversão do dado analógico.
5. Capturando a amostra (o dado convertido), faça o processo de decodificação.
6. Verifique qual é a ordem da amostra e salve no seu respectivo registro.
7. Some o valor da amostra atual com a amostra anterior e salve num registro.
8. Compare o sinal atual com um valor, se for, liga um dos LEDs (são 3 valores possíveis).
9. Sai da interrupção.
10. Volte a etapa 1 e faça o mesmo processo pro 2ª,3ª ... 10ª sinal a cada 20ms.
11. Quando for o 10ª sinal, multiplique por DeltaT (20ms) todas as somas das amostras.
12. Armazene o resultado final da área abaixo da forma de onda.
13. Finalize o programa.

Observações: No arquivo "Analog_Sampling_INFO.txt" tem todas as explicações referente aos valores das amostras fornecidos pelo pwlin generator do Proteus, os registros que são configurados e como são configurados,
a explicação de como funciona o processo de decodificação e os cálculos com resultado da área abaixo da onda por meio da fórmula de **aproximação de partição regular** (Seguindo o contexto da _Expansão de Furier_).
