#line 1 "C:/Users/BFTC/Desktop/ProvaN3_Dirlene/C_Temp_Sensor/E1_PIC18F4520/E1_Sensor_Serial.c"
#line 1 "c:/users/bftc/desktop/provan3_dirlene/c_temp_sensor/e1_pic18f4520/../seriallib.h"
#line 71 "c:/users/bftc/desktop/provan3_dirlene/c_temp_sensor/e1_pic18f4520/../seriallib.h"
void Serial_Send(unsigned short);
void Serial_Write(char*);
void Serial_Wait();

sbit SERIAL_SENDED at TRMT_bit;



sbit TIMER_RESET at RD0_bit;
sbit E2_BLOC_A at RD1_bit;
sbit E2_BLOC_B at RD2_bit;
sbit TMR0_OVERFLOW at TMR0IF_bit;
sbit EXTERN_INT0 at INT0IF_bit;
#line 104 "c:/users/bftc/desktop/provan3_dirlene/c_temp_sensor/e1_pic18f4520/../seriallib.h"
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
#line 34 "C:/Users/BFTC/Desktop/ProvaN3_Dirlene/C_Temp_Sensor/E1_PIC18F4520/E1_Sensor_Serial.c"
unsigned short LM35_Value = 0;
unsigned short A_ = 0, B_ = 0;
unsigned short _a = 0, _b = 0, _c = 0;
unsigned short _d = 0, _e = 0, _f = 0;
unsigned short Counter = 0;








void interrupt(){
 if(TMR0_OVERFLOW){
 TMR0_OVERFLOW =  0 ;
 Counter++;
 if(Counter == 153){
 LM35_Value = ADC_Read(0);

 TIMER_RESET =  1 ;
 TIMER_RESET =  0 ;


 Counter = 0;
 TMR0L = 2;
#line 72 "C:/Users/BFTC/Desktop/ProvaN3_Dirlene/C_Temp_Sensor/E1_PIC18F4520/E1_Sensor_Serial.c"
 _a =  ((LM35_Value & 0x80) >> 7)  ^  ((LM35_Value & 0x40) >> 6)  ^  ((LM35_Value & 0x10) >> 4) ; _d =  ((LM35_Value & 0x08) >> 3)  ^  ((LM35_Value & 0x04) >> 2)  ^  ((LM35_Value & 0x01) >> 0) ;
 _b =  ((LM35_Value & 0x80) >> 7)  ^  ((LM35_Value & 0x20) >> 5)  ^  ((LM35_Value & 0x10) >> 4) ; _e =  ((LM35_Value & 0x08) >> 3)  ^  ((LM35_Value & 0x02) >> 1)  ^  ((LM35_Value & 0x01) >> 0) ;
 _c =  ((LM35_Value & 0x40) >> 6)  ^  ((LM35_Value & 0x20) >> 5)  ^  ((LM35_Value & 0x10) >> 4) ; _f =  ((LM35_Value & 0x04) >> 2)  ^  ((LM35_Value & 0x02) >> 1)  ^  ((LM35_Value & 0x01) >> 0) ;


 A_ = 0 | ( ((LM35_Value & 0x80) >> 7)  << 6) | ( ((LM35_Value & 0x40) >> 6)  << 5) | ( ((LM35_Value & 0x20) >> 5)  << 4) | ( ((LM35_Value & 0x10) >> 4)  << 3) | (_a << 2) | (_b << 1) | (_c << 0);
 B_ = 0 | ( ((LM35_Value & 0x08) >> 3)  << 6) | ( ((LM35_Value & 0x04) >> 2)  << 5) | ( ((LM35_Value & 0x02) >> 1)  << 4) | ( ((LM35_Value & 0x01) >> 0)  << 3) | (_d << 2) | (_e << 1) | (_f << 0);



 Serial_Send(A_);
 while(!E2_BLOC_A);





 Serial_Send(B_);
 while(!E2_BLOC_B);


 }
 }else{
 if(EXTERN_INT0){
 EXTERN_INT0 =  0 ;
 if(!E2_BLOC_A)
 Serial_Send(A_);
 else
 Serial_Send(B_);
 }
 }
}








void main() {
 TXSTA = 0x24;
 RCSTA = 0x80;
 SPBRG = 25;
 BAUDCON = 0x00;
 INTCON = 0xF0;
 INTCON2 = 0xC4;
 T0CON = 0xC7;

 TRISB = 0x01;
 TRISC = 0xC0;
 TRISA = 0xFF;
 TRISD = 0x06;
 PORTA = 0x00;
 PORTB = 0x00;
 PORTC = 0x00;
 PORTD = 0x00;

 ADCON0 = 0x01;
 ADCON1 = 0x0E;

 delay_ms(100);




 Counter = 152;
 TMR0L = 255;

 while(1){}

}
