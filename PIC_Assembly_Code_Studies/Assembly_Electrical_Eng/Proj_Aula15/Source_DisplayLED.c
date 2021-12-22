void main() {
    unsigned short i = 0;
    unsigned char number = 0x00;
    unsigned char digits[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
                       
    TRISB = 0x00;
    PORTB = 0x00;
    
    while(1){
        for(i = 0; i <= 9; i++){
            number = digits[i];
            PORTB = number;
            delay_ms(1000);
        }
    }
}