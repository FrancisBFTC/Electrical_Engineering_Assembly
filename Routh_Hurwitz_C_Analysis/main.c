#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

float Kc;
float RHt[4][2];
float  x, y;
float a3, a2, a1, a0;
float E_s, S_s;
int status = 0;
int signal = 0;
int R = 0;

float RouthHurwitz(float, float, float);

int main(int argc, char *argv[]) {
	
while(R == 0){
	system("cecho {0B}");
	printf(" **** Analise de Estabilidade de Routh-Hurwitz ****\n\n");
	printf("                    s+1\n");
	printf("E(s) -> Kc -> _______________ = -> S(s)\n");
	printf("     ^         s( sx )( sy )     |\n");
	printf("     |___________________________|\n\n");
	
	system("cecho {0F}");
	printf("Digite a entrada para o Controlador Kc: ");
	scanf("%f", &Kc);
	printf("Insira o coeficiente x: ");
	scanf("%f", &x);
	printf("Insira o coeficiente y: ");
	scanf("%f", &y);
	
	E_s = Kc;
	S_s = RouthHurwitz(Kc, x, y);
	
	printf("\n\n O sistema possui %d mudanca(s) de sinal\n", signal);
	printf("(%d polo(s) no semiplano direito),\n ", signal);
	
	switch(status){
		case 0: 
			printf("portanto o sistema e ");
			system("cecho {04}");
			printf("INSTAVEL!");
			break;
		case 1:
			printf("portanto o sistema e ");
			system("cecho {02}");
			printf("ESTAVEL!");
			break;
		case 2:
			printf("e 0 raizes na 1ª coluna, portanto o sistema e ");
			system("cecho {0A}");
			printf("MARGINALMENTE ESTAVEL!");
			break;
	}
	
	system("cecho {0B}");
	printf("\n\nPara o sistema ser considerado");
	system("cecho {0A}");
	printf(" ESTAVEL ");
	system("cecho {0B}");
	printf(" pelo denominador atual D(s)\n");
	printf("A entrada do controlador deve ser maior que %.1f", S_s);
		
	printf("\n\n");
	
	system("cecho {0F}");
	printf("Digite 0 para recomecar: ");
	scanf("%d", &R);
	system("cls");	
	signal = 0;
	status = 0;
}

	system("pause");
	
	return 0;
}


float RouthHurwitz(float K, float s2, float s0){
	system("cecho {0B}");
	printf("\n\nDefinindo sistema em malha fechada...\n\n");
	
	system("cecho {02}");
	printf("                    s+1\n");
	printf("E(s) -> Kc -> _______________ = -> S(s)\n");
	
	if(s2 > 0)
		printf("     ^        s( s+%0.f )", s2);
	else
		if(s2 < 0)
			printf("     ^        s( s%0.f )", s2);
		else
			printf("     ^        s( s+0 )", s2);
			
	if(s0 > 0)
		printf("( s+%0.f )    |\n", s0);
	else
		if(s0 < 0)
			printf("( s%0.f )    |\n", s0);
		else
			printf("( s+0 )    |\n", s0);
			
	printf("     |___________________________|\n\n");
	
	system("cecho {0B}");
	printf("\n\nCriando denominador do sistema...\n\n");
	
	system("cecho {0A}");
	printf("D(s) = ");
	
	int j = 3;
	int a = 0;
	int s = 0;
	int i;
	
	for(i = j; i >= 0; i--){
		if(i > (j - 2)){
			if((i % 2) != 0){
				RHt[j-i][s] = 1;
				RHt[j-i][s+1] = K - s0; 
			}else{
				RHt[j-i][s] = s0 + s2;
			 	RHt[j-i][s+1] = K; 
			}
		}else{
			a3 = RHt[a][s];
			a1 = RHt[a][s+1];
			a2 = RHt[a+1][s];
			a0 = RHt[a+1][s+1];
			
			RHt[a+2][s] = (RHt[a+1][s] == 0.00) ? 0.00 : ((a2 * a1) - (a3 * a0)) / a2;
			a++;
		}
		
		
			if(i != 0){
				if(i > (j - 2))
					printf("%.0fs^%d", RHt[j-i][s], i);
				else
					printf("%.1fs^%d", RHt[i-1][s+1], i);	

					
				printf(" + ");
			}else{
				printf("%.1fs", K);
			}
		
	}
	
	
	system("cecho {0B}");
	printf("\n\n\nExibindo a tabela de Routh-Hurwitz\n e verificando estabilidade...\n\n");
	
	s = 3;
	
	for(i = 0; i < 4; i++){
		printf("s%d |", s);
		for(j = 0; j < 2; j++){
			
			if(RHt[i][0] < 0){
				if(j == 0)
					system("cecho {04}");
					
				printf("   %.2f ", RHt[i][j]);
				if(j == 0)
					signal++;
					
			}else 
				if(RHt[i][0] > 0){
					system("cecho {0B}");
						
				printf("    %.2f", RHt[i][j]);
				if(j == 0)
					if(signal == 1)
						signal++;
						
			}else{
				if(j == 0)
					system("cecho {0A}");
					
				printf("    %.2f", RHt[i][j]);
				if(j != 1)
					if(signal == 1)
						signal++;
	
			}
			
			system("cecho {0B}");
		}
		
		if(s == 2){
			printf("\n---|----------------------------");
		}
		printf("\n");
		s--;
	}
	
	if(signal > 0)
		status = 0;
	else 
		status = 1;
			
	if(RHt[2][0] == 0)
		status = 2;
	
	Kc = (RHt[1][0] * s0) / (RHt[1][0] - RHt[0][0]);
	
	return Kc;
}
