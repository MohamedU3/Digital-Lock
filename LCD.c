#include "LCD.h"

void LCD_DATA_4BIT(unsigned char data){
    if(data & 1)
        D4 = 1;
    else 
        D4 = 0;
    
    if(data & 2) 
        D5 = 1;
    else 
        D5 = 0;
    
    if(data & 4) 
        D6 = 1;
    else 
        D6 = 0;
    
    if(data & 8) 
        D7 = 1;
    else 
        D7 = 0;
}
void LCD_CMD(unsigned char CMD){ 
    RS = 0;
    LCD_DATA_4BIT(CMD);
    EN = 1;
    __delay_us(LCD_EN_DELAY);
    EN = 0;
}
//-----------------------------LCD INTIALIZATION----------------------------
void LCD_INIT(){
    //IO PIN CONFIG
    LCD_DATA_PORT_D = 0x00;
    LCD_RS_D = 0;
    LCD_EN_D = 0;
    //THE Init. Procedure
    LCD_DATA_4BIT(0x00);
    __delay_ms(30);
    __delay_us(LCD_EN_DELAY);
    LCD_CMD(0x03);
    __delay_ms(5);
    LCD_CMD(0x03);
    __delay_us(150);
    LCD_CMD(0x03);
    LCD_CMD(0x02); //Function set (Set interface to be 4 bits long.)
    LCD_CMD(0x02); //Function set no. of line and font
    LCD_CMD(0x08); //Function set no. of line and font (it will be initialized as 2 lines and font 5x8 dot)
    LCD_CMD(0x00);
    LCD_CMD(0x0C);//CURSOR ON
    LCD_CMD(0x00);//ENTRY MODE
    LCD_CMD(0x06);
}
//-----------------------------LCD INTIALIZATION END----------------------------

//-----------------------------LCD WRITE----------------------------
void LCD_WRITE_CHAR(char data){
    char low4,high4;
    low4 = data & 0x0F;
    high4 = data & 0xF0;
    RS = 1; 
    LCD_DATA_4BIT(high4>>4);
    EN = 1;
    __delay_us(LCD_EN_DELAY);
    EN = 0;
    __delay_us(LCD_EN_DELAY);
    LCD_DATA_4BIT(low4);
    EN = 1;
    __delay_us(LCD_EN_DELAY);
    EN = 0;
    __delay_us(LCD_EN_DELAY);
}
void LCD_WRITE_STRING(char* str){
    for(int i = 0;str[i]!='\0';i++){
        LCD_WRITE_CHAR(str[i]);
    }
}
//-----------------------------LCD WRITE END----------------------------

void LCD_CLEAR(){ //DOESN'T WORK IF U PUT IT AT THE START OF THE CODE
    LCD_CMD(0x00);
    LCD_CMD(0x01);
    __delay_us(82);
}

//-----------------------DISPLAY CONTROL START------------------------------
void LCD_DISPLAY(unsigned char state){//if state -> 1:Display ON , 0:Display off
    LCD_CMD(0);
    if(state == 1) LCD_CMD(0x0C);
    else LCD_CMD(0x08);
    __delay_us(37);
}
void LCD_SHOW_CURSOR(unsigned char c){ //if state = 1 ->cursor on
    LCD_CMD(0);
    if(c == 1) LCD_CMD(0x0E);
    else LCD_CMD(0x0C);
    __delay_us(37);
}
//-----------------------DISPLAY CONTROL END------------------------------

void RETURN_HOME(){
    LCD_CMD(0);
    LCD_CMD(0x02);
    __delay_ms(1.52);
}

//-----------------------SCREEN/CURSOR SHIFT LEFT/RIGHT------------------------------
//NOTE:the following used for navigating only, it doesn't change any value displayed on screen when the screen/cursor is moved
void LCD_SR(){ //THIS DOESN'T WORK AS IT GOES TO THE LEFT NOT THE RIGHT
    LCD_CMD(0x01);
    LCD_CMD(0x0C);
    __delay_us(37);
}
void LCD_SL(){//THIS DOESN'T WORK AS IT GOES TO THE RIGHT NOT THE LEFT AND IF YOU PUT IT FIRST IT THE SCREEN TURNS OFF AND ON??
    LCD_CMD(1);
    LCD_CMD(0x08);
    __delay_us(37);
}
void LCD_CR(){ //THIS DOESN'T SHOW THE CURSOR MOVING (IDK IF THAT'S HOW IT WORKS OR THERE IS SOMETHING WRONG WITH IT)
    LCD_CMD(0x01);
    LCD_CMD(0x04);
    __delay_us(37);
}
void LCD_CL(){//THIS DOESN'T SHOW THE CURSOR MOVING (IDK IF THAT'S HOW IT WORKS OR THERE IS SOMETHING WRONG WITH IT)
    LCD_CMD(0x01);
    LCD_CMD(0);
    __delay_us(37);
}
//-----------------------DISPLAY/CURSOR SHIFT END------------------------------

void LCD_SET_CURSOR(unsigned char r,unsigned char c){//SET DDRAM ADDRESS OR CURSOR POSTION ON THE SCREEN 0x08+add*
    unsigned char high4,low4,temp;
    if(r == 1){
        temp = 0x80 + c - 1; //0x08 is used as cursor base on line 1
        high4 = temp>>4;
        low4 = temp & 0x0F;
        LCD_CMD(high4);
        LCD_CMD(low4);
    }
    if(r == 2){
        temp = 0xC0 + c - 1; //0xC0 is used as cursor base on line 2
        high4 = temp>>4;
        low4 = temp & 0x0F;
        LCD_CMD(high4);
        LCD_CMD(low4);
    }
    __delay_us(37);
}
