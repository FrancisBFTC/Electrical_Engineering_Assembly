#define D1 GPIO.B1
#define D2 GPIO.B2
#define D3 GPIO.B4
#define D4 GPIO.B5

int ADC_Value;

void main() {
     TRISIO = 0x01;  // 0b00000001 -> AN0 como entrada, resto como saída
     ANSEL  = 0x11;  // 0b00010001 -> Fosc/8 & Canal 0
     CMCON  = 0x07;  // 0b00000111 -> Desabilita comparadores
     ADCON0 = 0x01;  // 0b00000001 -> J. Esquerda, VDD, AN0, Habilita A/D
     GPIO   = 0;

     while(1){
          ADC_Value = ADC_Read(0);
          
          if(ADC_Value == 0){
              D1 = 1;
              D2 = 0;
              D3 = 0;
              D4 = 0;
          }else if(ADC_Value == 256){
              D1 = 0;
              D2 = 1;
              D3 = 0;
              D4 = 0;
          }else if(ADC_Value == 512){
              D1 = 0;
              D2 = 0;
              D3 = 1;
              D4 = 0;
          }else if(ADC_Value == 750){
              D1 = 0;
              D2 = 0;
              D3 = 0;
              D4 = 1;
          }

     }
}