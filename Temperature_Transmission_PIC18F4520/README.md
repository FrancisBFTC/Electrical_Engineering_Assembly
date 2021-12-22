# Transmissão da Temperatura via Comunicação Serial em C

Este é um projeto relacionado a um microcontrolador E1 que faz a leitura de um sensor de temperatura LM35 e faz a conversão analógica para digital do dado deste sensor.
O dado vem em formato de voltagem, a cada 1 unidade incrementada na temperatura, serão 10mV adicionadas. Então o microcontrolador E1 (PIC16F4520) após a conversão do sinal,
efetua 3 etapas para a transmissão do dado. Após realizar as 3 etapas, ele transmite via comunicação serial para a porta TX, onde o dado passará por uma linha de transmissão conturbada,
isto é, um amplificador operacional LM741 utiliza um circuito de realimentação negativa com somador não-inversor para adicionar um ruído ao dado serial transmitido, a partir daí,
o dado é recebido pelo microcontrolador E2 que vai efetuar 5 etapas de verificação e correção de erros, sinalizando o outro microcontrolador em possíveis em situações que é necessário
o reenvio do dado atual. O dado é dividido em 2 pacotes, logo são 2 envios, e estes 2 envios ocorrem dentro do tempo de 10s, ou seja, a cada 10 segundos a temperatura é recebida
pelo microcontrolador E2. A missão final de E2 é fazer a conversão do dado corrigido para Float e escrever no terminal virtual via comunicação serial.
