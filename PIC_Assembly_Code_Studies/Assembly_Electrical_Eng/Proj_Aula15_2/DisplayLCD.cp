#line 1 "C:/Users/BFTC/Desktop/D.S.O.S/Systems/PIC Assembly Programming/Source-Code/ProjetoENG421_Especial/Proj_Aula15_1/DisplayLCD.c"
sbit LCD_RS at RD2_bit;
sbit LCD_EN at RD3_bit;
sbit LCD_D4 at RD4_bit;
sbit LCD_D5 at RD5_bit;
sbit LCD_D6 at RD6_bit;
sbit LCD_D7 at RD7_bit;

sbit LCD_D7_Direction at TRISD7_bit;
sbit LCD_D6_Direction at TRISD6_bit;
sbit LCD_D5_Direction at TRISD5_bit;
sbit LCD_D4_Direction at TRISD4_bit;
sbit LCD_RS_Direction at TRISD2_bit;
sbit LCD_EN_Direction at TRISD3_bit;

void main() {

 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);

 Lcd_Out(1,1,"String: ");
 lcd_chr(2,2,'H');
 lcd_chr_cp('e');
 lcd_chr_cp('l');
 lcd_chr_cp('l');
 lcd_chr_cp('o');

 lcd_cmd(_LCD_RETURN_HOME);
}
