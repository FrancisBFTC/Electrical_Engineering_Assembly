#line 1 "C:/Users/BFTC/Desktop/D.S.O.S/Systems/PIC Assembly Programming/Source-Code/ProjetoENG421_Especial/Proj_Aula14/Proj_ADC.c"





int ADC_Value;

void main() {
 TRISIO = 0x01;
 ANSEL = 0x11;
 CMCON = 0x07;
 ADCON0 = 0x01;
 GPIO = 0;

 while(1){
 ADC_Value = ADC_Read(0);

 if(ADC_Value == 0){
  GPIO.B1  = 1;
  GPIO.B2  = 0;
  GPIO.B4  = 0;
  GPIO.B5  = 0;
 }else if(ADC_Value == 256){
  GPIO.B1  = 0;
  GPIO.B2  = 1;
  GPIO.B4  = 0;
  GPIO.B5  = 0;
 }else if(ADC_Value == 512){
  GPIO.B1  = 0;
  GPIO.B2  = 0;
  GPIO.B4  = 1;
  GPIO.B5  = 0;
 }else if(ADC_Value == 750){
  GPIO.B1  = 0;
  GPIO.B2  = 0;
  GPIO.B4  = 0;
  GPIO.B5  = 1;
 }

 }
}
