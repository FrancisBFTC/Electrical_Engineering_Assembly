#line 1 "C:/Users/BFTC/Desktop/D.S.O.S/Systems/PIC Assembly Programming/Source-Code/ProjetoENG421_Especial/Proj_Aula17/C_Term_Sensor.c"
int LM_Value = 0;
char text[30];
float Celsius = 0.0;

void main() {
 TRISA = 0xFF;
 ADCON0 = 0x01;
 ADCON1 = 0x0E;

 UART1_Init(9600);
 delay_ms(100);

 UART1_Write_Text("Temperatura medida: ");

 while(1){
 LM_Value = ADC_Read(0);









 Celsius = ((LM_Value * 5.0) / 1024.0) * 100.0;

 FloatToStr(Celsius, text);
 UART1_Write_Text(text);
 UART1_Write_Text(" oC...");
 UART1_Write(10);
 UART1_Write(13);

 delay_ms(500);
 }
}
