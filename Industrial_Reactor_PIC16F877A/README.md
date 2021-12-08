# Reator Industrial em Assembly

O Reator Industrial é composto por um sistema de válvulas que ligam e desligam num tempo definido utilizando "Equações Booleanas" para fazer misturas de 3 produtos aquecidos em 100 graus e com pressão interna de 120psi, estas equações executam num Loop e devem executar as seguintes operações:

1. Abrir Válvula V1 quando o botão START for pressionado.
2. Esperar o 1ª reservatório encher até o nível médio (identificado pelo sensor LM).
3. Após o sensor LM identificar o produto 1, fechar válvula V1 e abrir válvula V2 com retardo de 1s.
4. Abrir válvula V2 ao mesmo tempo que liga o motor MOT1, a válvula V2 é fechada após o Produto 2 chegar ao nível Alto (Identificado pelo sensor LH).
5. Reter o motor MOT1 por 12s fazendo a mistura dos 2 produtos, após isto fechar o MOT1.
6. Após fechar MOT1, abre a válvula V3 para inserir o vapor até atingir 100Cº (Graus Celsius).
7. Fecha a válvula V3 quando o sensor TT identifica 100 graus, o sistema entra em repouso por 3s.
8. Após o tempo de repouso, Abre a válvula V5 para despejar a mistura do 1ª reservatório até o 2ª reservatório.
9. V5 fica aberta até o sensor LL não identificar nenhum líquido acima dele, após isto feche V5.
10. Após V5 fechar, abre a válvula V4 para inserir a pressão interna de 120psi.
11. Após o sensor TP identificar pressão de 120psi, fecha a válvula V4.
12. Após fechar V4, liga a bomba despejando o produto 3 no 2ª reservatório, retem a bomba por 5s.
13. Após o tempo da bomba, desligue a bomba e abre a válvula V6 para despachar todas as misturas.
14. Enquanto V6 estiver aberta, espere pela ação do usuário no Botão RESET.
15. Após o pressionamento do RESET, pare todo o sistema e reinicie tudo novamente (Sem iniciar as operações).
16. No meio das operações, se o usuário pressionar o Botão STOP, pause o sistema até que o Botão seja novamente pressionado.
17. Quando o botão STOP novamente é pressionado, retorne ao estado atual do reator, continuando as operações.

Observações: O código do reator está bem documentado, explicando cada equação de cada operação. Para executar este projeto veja tutoriais na internet de como instalar
o Proteus 8 Professional com chave de licença e o MPLAB IDE v8.92 utilizando o montador MPASM.
