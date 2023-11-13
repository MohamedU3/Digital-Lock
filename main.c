#include <xc.h>
#include "config.h"
#include <stdint.h>
#include <stdio.h>
#include "LCD.h"
//FUNCTIONS OF THE LCD.h
//LCD_INIT(),LCD_WRITE_CHAR(char data),LCD_WRITE_STRING(char* str),LCD_CLEAR(),RETURN_HOME()
//LCD_DISPLAY(unsigned char state) 1:Display ON , 0:Display off
//LCD_SHOW_CURSOR(unsigned char state) 1:cursor ON , 0:cursor off
//SHIFTING SCREEN OR CURSOR: LCD_SR(),LCD_SL(),LCD_CR(),LCD_CL()
//SET CURSOR POSITION: LCD_SET_CURSOR(unsigned char r,unsigned char c)
#include "EEPROM_.h"
//FUNCTIONS OF EEPROM.h 
//EEPROM_write(uint8_t address,uint8_t data)->write data to address ,EEPROM_read(uint8_t address)->returns data
#define _XTAL_FREQ 4000000
#define keypad_1 RD0
#define keypad_2 RD1
#define keypad_back RD2
#define keypad_enter RD3

uint8_t counter = 0; //this counter is to reach 1 second in TMR1
uint16_t preload = 0; //preload value that is preloaded to TMR1 (will be determined in main function)
void timer1_timermode(uint8_t);//a lock down feature will be added in the future using TMR1 
//this feature will be blocking the user for "n" minutes if he entered the pass incorrectly for more than "n" times
void __interrupt()ISR();
void main_menu();
void blink();

//-------------------------------------flags---------------------------------
    uint8_t digits = 0;//every time the user presses a button this will be incremented
    //indicating how many digits are on the screen
    uint8_t take_input = 0; //0:user can't press any button, 1:user can press buttons
    //2:user reached maximum number of inputs(4-digit password)
    uint8_t screen = 0; //indicates which screen is the user on, 0:main menu , 1:enter password , 2:new password
    uint8_t line = 1;
    
void main(void) {
    //========================================TMR1================================================

    preload = 15536; //change this value to what ever you want to preload TMR1 with
    timer1_timermode(preload);
    //========================================TMR1 END================================================
    
    //========================================LCD================================================
    //PASSWORD WILL BE A 4 DIGIT NUMBER
    TRISD0 = 1; TRISD1 = 1; TRISD2 = 1; TRISD3 = 1; //I/O
    
    //-----------------password checking variables-----------------------
    uint16_t password = 0; //every time a button is clicked it will save the number of the button to password
    //this means that this variable holds what ever the user inputs
    uint16_t check_input = 0;

    //---------------------default password----------------
    //LCD Start
    LCD_INIT();
    LCD_SHOW_CURSOR(1); //add cursor when you enter password
    if(EEPROM_Read(0) == 0xFF){//all EEPROMs contain 0xFF:s in all memory locations when shipped from the factory
        LCD_CLEAR();
        LCD_SET_CURSOR(1,1);
        LCD_WRITE_STRING("First Time?");
        __delay_ms(2000);
        LCD_CLEAR();
        LCD_SET_CURSOR(1,1);
        LCD_WRITE_STRING("Set Pass!:");
        LCD_SET_CURSOR(2,1);
        LCD_WRITE_STRING("4 digit pass");
        LCD_SET_CURSOR(1,11);
    }
    else main_menu();
    //========================================LCD END================================================
    while(1){
        start:
        if(keypad_1)
        {
            if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('1');
                digits++;
                if(digits > 1) password = password * 10 + 1;
                else password = 1;
                if(digits == 4)
                {
                    take_input = 2;//this means that it's awating the user to press enter to check password
                    //or waiting the user to press "back" to cancel
                    digits = 0;
                }
            }
            else if(take_input == 0){//this means we are in the main_menu, so don't take any input just choose option 1 or 2
                screen = 1;
                take_input = 1; //change this to 1 because now we have to type the password
                LCD_CLEAR();
                LCD_SET_CURSOR(1,1);
                LCD_WRITE_STRING("Pass:");
                LCD_SET_CURSOR(2,1);
                LCD_WRITE_STRING("*:Main menu");
                LCD_SET_CURSOR(1,6);
                __delay_ms(250);
                goto start;
            }
            else if(take_input == 1){
                
                LCD_WRITE_CHAR('1');
                digits++;
                if(digits > 1) password = password * 10 + 1;
                else password = 1;
                if(digits == 4)
                {
                    take_input = 2;//this means that it's awating the user to press enter to check password
                    //or waiting the user to press "back" to cancel
                    digits = 0;
                }
            }
            
            __delay_ms(250);
        }
        if(keypad_2)
        {
            if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('2');
                digits++;
                if(digits > 1) password = password * 10 + 2;
                else password = 2;
                if(digits == 4)
                {
                    take_input = 2;//this means that it's awating the user to press enter to check password
                    //or waiting the user to press "back" to cancel
                    digits = 0;
                }
            }
            else if(take_input == 0){//this means we are in the main_menu, so don't take any input just choose option 1 or 2
                screen = 2;
                take_input = 1; //change this to 1 because now we have to type the password
                LCD_CLEAR();
                LCD_SET_CURSOR(1,1);
                LCD_WRITE_STRING("Old Pass:");
                LCD_SET_CURSOR(2,1);
                LCD_WRITE_STRING("New pass:");
                LCD_SET_CURSOR(1,10);
                __delay_ms(250);
                goto start;
            }
            else if(take_input == 1){
                
                LCD_WRITE_CHAR('2');
                digits++;
                if(digits > 1) password = password * 10 + 2;
                else password = 2;
                if(digits == 4)
                {
                    take_input = 2;//this means that it's awating the user to press enter to check password
                    //or waiting the user to press "back" to cancel
                    digits = 0;
                }
            }
            __delay_ms(250);
        }
        if(keypad_back)
        {
            if(EEPROM_Read(0) == 0xFF){}
            else main_menu();
            __delay_ms(250);
        }
        if(keypad_enter)//EEPROM read and write
        {
            if(take_input == 2){ //must be 2 in order for this button to do any action
                if(EEPROM_Read(0) == 0xFF){
                    EEPROM_Write(0,(password/100));
                    EEPROM_Write(1,(password%100));
                    LCD_CLEAR();
                    LCD_SET_CURSOR(1,5);
                    LCD_WRITE_STRING("Welcome :)"); blink();
                    main_menu();
                    }
                else if(screen == 1){
                    check_input = EEPROM_Read(0);
                    check_input *= 100;
                    check_input += EEPROM_Read(1);
                    if(check_input == password){
                        LCD_CLEAR();
                        LCD_SET_CURSOR(1,5);
                        LCD_WRITE_STRING("ACCESS GRANTED"); blink();
                        main_menu();
                    }
                    else{
                        LCD_CLEAR();
                        LCD_SET_CURSOR(1,5);
                        LCD_WRITE_STRING("Pass Incorrect"); blink();
                        main_menu();
                    }
                }
                else if(screen == 2){
                    if(line == 1){
                        check_input = EEPROM_Read(0);
                        check_input *= 100;
                        check_input += EEPROM_Read(1);
                        if(check_input == password){
                            LCD_SET_CURSOR(2,10);
                            line = 2;
                            take_input = 1;
                            __delay_ms(250);
                            goto start;
                        }
                        else{
                            LCD_CLEAR();
                            LCD_SET_CURSOR(1,5);
                            LCD_WRITE_STRING("Pass Incorrect"); blink();
                            main_menu();
                        }
                    }
                    else if(line == 2){
                        EEPROM_Write(0,(password/100));
                        EEPROM_Write(1,(password%100));
                        LCD_CLEAR();
                        LCD_SET_CURSOR(1,5);
                        LCD_WRITE_STRING("Pass Changed"); blink();
                        main_menu();
                    }
                }
            }
          __delay_ms(250);
        }
                
    }

    
    return;
}
void main_menu(){
    line = 1;
    take_input = 0;
    digits = 0;
    screen = 0;
    LCD_CLEAR();
    LCD_SET_CURSOR(1,1);
    LCD_WRITE_STRING("1:Enter Password");
    LCD_SET_CURSOR(2,1);
    LCD_WRITE_STRING("2:New Pass");
}
void blink(){
    for(uint8_t i = 0; i < 4; i++)
        {
        LCD_DISPLAY(0);
         __delay_ms(250);
        LCD_DISPLAY(1);
        __delay_ms(250);
        }
}
//========================================TMR1================================================
void timer1_timermode(uint8_t preloaded){//TMR1 initialization
    /*TMR1CS: Timer1 Clock Source Select bit
1 = External clock from pin RC0/T1OSO/T1CKI (on the rising edge)
0 = Internal clock (FOSC/4)*/
    TMR1CS = 0;
    /*T1CKPS1:T1CKPS0: Timer1 Input Clock Prescale Select bits
11 = 1:8 prescale value
10 = 1:4 prescale value
01 = 1:2 prescale value
00 = 1:1 prescale value*/
    T1CKPS0 = 0;
    T1CKPS1 = 0;
    
    TMR1 = preloaded; //preloaded value
    //interrupt enable
    TMR1IE = 1;
    TMR1IF = 0;//clear interrupt flag
    GIE = 1;
    PEIE = 1;
    /*TMR1ON: Timer1 On bit
1 = Enables Timer1
0 = Stops Timer1*/
    TMR1ON = 1;
}
//========================================TMR1 END================================================
void __interrupt()ISR(){
    if(TMR1IF){
        TMR1 = preload; //preloaded value to 
        counter++;//if this variable counts 20 times this means that 1 second passed
        if(counter == 20){ //this means that it counted 1 second
            
            //DO SOMETHING HERE
            
            
            counter = 0; //must reset the 1 second counter
        }
        TMR1IF = 0;
    }
}