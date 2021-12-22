# Análise de Estabilidade de Routh-Hurwitz em C

Este projeto é uma aplicação CMD para análise de estabilidade de um sistema de malha fechada utilizando o critério de Routh-Hurwitz. Este critério consiste em transformar os
coeficientes de uma equação característica num denominador completo e construir uma tabela acerca deste denominador. Através desta tabela serão feito cálculos pra determinar
se este sistema é estável, instável ou marginalmente estável. O Algoritmo em C é capaz de apresentar informações no CMD de forma visual e divertida, mostrando as equações e a tabela,
como também relatórios informando ao usuário sobre o resultado da análise. O Algoritmo processa em cima de 3 argumentos dados no início: O valor do controlador que é uma voltagem
positiva ou negativa dado ao "ganho" do sistema de malha fechada, o coeficiente X que será parte da equação característica como denominador e o coeficiente Y que será a outra parte da
equação característica como denominador, é através destes 3 argumentos que todo o sistema irá se basear para efetuar os cálculos e nos dar o resultado apropriado.
