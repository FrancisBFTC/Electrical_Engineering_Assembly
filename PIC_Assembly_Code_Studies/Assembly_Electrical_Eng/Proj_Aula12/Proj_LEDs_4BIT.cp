#line 1 "C:/Users/BFTC/Desktop/D.S.O.S/Systems/PIC Assembly Programming/Source-Code/ProjetoENG421_Especial/Proj_Aula12/Proj_LEDs_4BIT.c"
void mydelay();

int time1 = 0;
int time2 = 0;

void main() {






 TRISA = 0x00;
 PORTA = 0x00;

 while(PORTA < 13){
 PORTA++;

 mydelay();
 }

 mydelay();

 PORTA = 0x00;
 RA4_bit = 0x01;

}

void mydelay(){

asm{
Delay500ms:
 movlw 200 ; Move o valor 200 para W
 movwf _time1 ; Inicializa variável tempo0 (Endereço relativo)
Aux1: ; Já gastou 4 ciclos de máquina
 movlw 250 ; Move o valor 250 para W
 movwf _time2 ; Inicializa variávem tempo1 (Endereço relativo)

Aux2:
 nop ; Gasta 1 ciclo de máquina
 nop ; Gasta 1 ciclo de máquina
 nop ; Gasta 1 ciclo de máquina
 nop ; Gasta 1 ciclo de máquina
 nop ; Gasta 1 ciclo de máquina
 nop ; Gasta 1 ciclo de máquina
 nop ; Gasta 1 ciclo de máquina
 decfsz _time2,F ; Decrementa tempo1 até que seja igual a 0
 goto Aux2 ; Vai para a label Aux2
 ; GOTO gasta 2 ciclos de máquinas, logo foram 10 ciclos
 ; 250 x 10 ciclos de máquina = 2500 ciclos
 decfsz _time1,F ; Decrementa tempo0 até que seja igual a 0
 goto Aux1 ; Vai para a label Aux1
 ; Instruções acima gastou 3 ciclos de máquina
 ; 2500 x 200 = 500000 ciclos de máquina
;return ; Retorna para a chamada
}
}
