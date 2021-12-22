#line 1 "C:/Users/BFTC/Desktop/ProvaN3_Dirlene/C_Temp_Sensor/E2_PIC16F628A/E2_Serial_Terminal.c"
#line 1 "c:/users/bftc/desktop/provan3_dirlene/c_temp_sensor/e2_pic16f628a/../seriallib.h"
#line 71 "c:/users/bftc/desktop/provan3_dirlene/c_temp_sensor/e2_pic16f628a/../seriallib.h"
void Serial_Send(unsigned short);
void Serial_Write(char*);
void Serial_Wait();

sbit SERIAL_SENDED at TRMT_bit;
#line 88 "c:/users/bftc/desktop/provan3_dirlene/c_temp_sensor/e2_pic16f628a/../seriallib.h"
sbit FRAMING_ERROR at FERR_bit;
sbit OVERRUN_ERROR at OERR_bit;
sbit RECEPTION at CREN_bit;
sbit RECEPTION_INT at RCIF_bit;
sbit LED_GREEN at RB6_bit;
sbit LED_YELLOW at RB5_bit;
sbit LED_RED at RB4_bit;
sbit SIGN_E1 at RB7_bit;
sbit E1_BLOC_A at RB3_bit;
sbit E1_BLOC_B at RB0_bit;






void Serial_Send(unsigned short val){
 TXREG = val;
 Serial_Wait();
}


void Serial_Write(char *string){
 int i;
 for(i = 0; i < strlen(string); i++){
 TXREG = string[i];
 Serial_Wait();
 }
}


void Serial_Wait(){
 while(!SERIAL_SENDED);
}
#line 36 "C:/Users/BFTC/Desktop/ProvaN3_Dirlene/C_Temp_Sensor/E2_PIC16F628A/E2_Serial_Terminal.c"
unsigned short i = 0;
unsigned short D_[3], D[7];
unsigned short Logic_Val = 0;
unsigned short Received = 0;
unsigned short Terminal_Word = 0;
float Celsius = 0.0;
char Text[5];








void interrupt(){
 if(RECEPTION_INT){
 if(FRAMING_ERROR || OVERRUN_ERROR){
 RECEPTION =  0 ;
 RECEPTION =  1 ;
 }else{

 Received++;
 LED_GREEN =  0 ;
 LED_YELLOW =  1 ;
#line 72 "C:/Users/BFTC/Desktop/ProvaN3_Dirlene/C_Temp_Sensor/E2_PIC16F628A/E2_Serial_Terminal.c"
 RCREG = RCREG & 0b01111111;


 D_[2] =  ((RCREG & 0x40) >> 6)  ^  ((RCREG & 0x20) >> 5)  ^  ((RCREG & 0x08) >> 3) ;
 D_[1] =  ((RCREG & 0x40) >> 6)  ^  ((RCREG & 0x10) >> 4)  ^  ((RCREG & 0x08) >> 3) ;
 D_[0] =  ((RCREG & 0x20) >> 5)  ^  ((RCREG & 0x10) >> 4)  ^  ((RCREG & 0x08) >> 3) ;
 D[0] =  ((RCREG & 0x01) >> 0) ;
 D[1] =  ((RCREG & 0x02) >> 1) ;
 D[2] =  ((RCREG & 0x04) >> 2) ;



 for(i = 0; i < 3; i++){
 Logic_Val = !(D_[i] ^ D[i]);





 if(Logic_Val == 0){
 SIGN_E1 =  1 ;
 LED_RED =  1 ;
 LED_YELLOW =  0 ;
 Received -= 1;
 SIGN_E1 =  0 ;
 break;
 }
 }

 if(Received == 1 & Logic_Val){

 E1_BLOC_A =  1 ;



 D[6] =  ((RCREG & 0x40) >> 6) ;
 D[5] =  ((RCREG & 0x20) >> 5) ;
 D[4] =  ((RCREG & 0x10) >> 4) ;
 D[3] =  ((RCREG & 0x08) >> 3) ;

 LED_RED =  0 ;
 SIGN_E1 =  0 ;
 }else{
 if(Received == 2 & Logic_Val){
 E1_BLOC_B =  1 ;




 Terminal_Word = (D[6] << 7) | (D[5] << 6) | (D[4] << 5) | (D[3] << 4) | ( ((RCREG & 0x40) >> 6)  << 3) | ( ((RCREG & 0x20) >> 5)  << 2) | ( ((RCREG & 0x10) >> 4)  << 1) | ( ((RCREG & 0x08) >> 3)  << 0);


 Celsius = ((Terminal_Word * 5.0) / 1024.0) * 100.0;



 Serial_Write("\r\n ************ \r\n\r\n");
 Serial_Write("Temp.: ");
 FloatToStr_Fixlen(Celsius, Text, 5);
 Serial_Write(Text);
 Serial_Write(" C");
 Serial_Write("\r\n\r\n ************  \r\n");
 Received = 0;
 LED_GREEN =  1 ;
 E1_BLOC_A =  0 ;
 E1_BLOC_B =  0 ;
 }
 }

 }
 }
}








void main() {
 OPTION_REG = 0x80;
 SPBRG = 25;
 TXSTA = 0b00100100;
 RCSTA = 0b10010000;
 PIE1 = 0b00100000;
 INTCON = 0b11000000;

 TRISB = 0x06;
 PORTB = 0x00;


 while(1){ }
}
