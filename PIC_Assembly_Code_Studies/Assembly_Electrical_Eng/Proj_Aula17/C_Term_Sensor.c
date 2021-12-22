int LM_Value = 0;
char text[30];
float Celsius = 0.0;

void main() {
     TRISA = 0xFF;      // PORTA como entrada
     ADCON0 = 0x01;     // Habilita o módulo de conversão
     ADCON1 = 0x0E;     // Garante que AN0 funcione como entrada analógica
     
     UART1_Init(9600);  // Inicializa Serial como 9600 Baulds
     delay_ms(100);     // Tempo de 100ms para configuração
     
     UART1_Write_Text("Temperatura medida: ");
     
     while(1){
          LM_Value = ADC_Read(0);   // Captura valor analógico da temperatura do LM35
          
          // 10mV  --> 1ºC
          // AN0   --> ?
          
          // Visto que o ADC usa 10bits
          // como 2^10 = 1024, então
          // 5V  --> 1024
          // AN0 --> LM_Value
          
          Celsius = ((LM_Value * 5.0) / 1024.0) * 100.0;   // Mesma coisa que dividir por 10mV
          
          FloatToStr(Celsius, text);
          UART1_Write_Text(text);
          UART1_Write_Text(" oC...");
          UART1_Write(10);
          UART1_Write(13);
          
          delay_ms(500);
     }
}