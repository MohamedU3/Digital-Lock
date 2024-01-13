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
//EEPROM_Write(uint8_t address,uint8_t data)->write data to address ,EEPROM_Read(uint8_t address)->returns data
#define _XTAL_FREQ 4000000
#define c1 RD0 //c as in column
#define c2 RD1
#define c3 RD2

//-----------------password checking variables-----------------------
    uint16_t password = 0; 
    //every time a button is clicked it will save the number of the button to password.
    //this means that this variable holds what ever the user inputs.
    uint16_t check_input = 0;
    //password will be split into two number : 1-first two digits 2-last two digits
    //then it will be sent to the EEPROM into two times you can only send an 8-bit number at a time so a max number of 255
    //this variable to add the first and last two digits, so it combines the whole password
    //the purpose of it is to hold the password that the user inputs to compare it to the pass in the EEPROM
uint8_t counter = 0; 
//this counter is to reach 1 second in TMR1.
uint16_t preload = 0; 
//preload value that is preloaded to TMR1 (will be determined in main function).
void timer1_timermode(uint8_t);
//a lock down feature will be added in the future using TMR1.
//this feature will be blocking the user for "n" minutes if he entered the pass incorrectly for more than "n" times.
void __interrupt()ISR();
void main_menu();
void blink();
void keypad(void);
void lock_down(void);

//-------------------------------------flags---------------------------------
    uint8_t digits = 0;
    //every time the user presses a button this will be incremented.
    //indicating how many digits are on the screen.
    //this is a safety procedure to prevent the user from entering a password less than 4 digits.
    uint8_t take_input = 0; //this indicates if it's possible to take input from user or not.
    //it's made to prevent the user from typing any input in the main menu but allows it on other menus.
    //0:user can't press any button except 1 and 2,
    //1:user can press buttons, 2:user reached maximum number of inputs(4-digit password).
    //3: user is on lock down for 4 minutes
    uint8_t screen = 0; 
    //indicates which screen is the user on, 0:main menu , 1:enter password , 2:new password, 3:lock down
    uint8_t line = 1;
    //this is used to indicate which line is the user on, on the LCD.
    
    uint8_t seconds = 0;
    uint8_t tries;
    //counts the number of seconds 4min = 240 seconds
    //clear block 3 of memory where it saves the value of seconds
void main(void) {
    //========================================TMR1================================================
    preload = 15536; //change this value to what ever you want to preload TMR1 with
    timer1_timermode(preload);
    //========================================TMR1 END================================================
    //========================================LCD================================================
    //PASSWORD WILL BE A 4 DIGIT NUMBER.
    TRISD0 = 1; TRISD1 = 1; TRISD2 = 1; TRISD3 = 1; //Inputs for each button.
    TRISD = 0x78;
    //LCD Start.
    LCD_INIT();
    LCD_SHOW_CURSOR(1); 
    //add cursor when you enter password.
    
    if(EEPROM_Read(0) == 0xFF){//all EEPROMs contain 0xFF:s in all memory locations when shipped from the factory.
        EEPROM_Write(3,0);
        EEPROM_Write(4,0);
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
    else if(EEPROM_Read(2) == 3) lock_down();//this checks if the user is locked or not even if her restarted the lock
    else main_menu();
    //========================================LCD END================================================
    while(1){
        tries = EEPROM_Read(4);
    //number of tries before lock down for 4 min---- max num of tries = 5
        keypad();      
    }
    return;
}

void keypad(void){
    start:
    c1 = 1; c2 = 0; c3 = 0;
    if(RD3){ //'1'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('1');
                digits++;
                if(digits > 1) password = password * 10 + 1;
                else password = 1;
                if(digits == 4)
                {
                    take_input = 2;
                    //this means that it's awaiting the user to press enter to check password.
                    //or waiting the user to press "back" to cancel.
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
    if(RD4){//'4'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('4');
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
            else if(take_input == 1){
                
                LCD_WRITE_CHAR('4');
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
    if(RD5){//'7'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('7');
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
            else if(take_input == 1){
                
                LCD_WRITE_CHAR('7');
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
    if(RD6){ //'*'
        if(EEPROM_Read(0) == 0xFF){}
        else main_menu();
        __delay_ms(250);
    }
    
    c1 = 0; c2 = 1; c3 = 0;
    if(RD3){ //'2'
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
    if(RD4){//'5'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('5');
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
            else if(take_input == 1){
                
                LCD_WRITE_CHAR('5');
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
    if(RD5){//'8'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('8');
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
            else if(take_input == 1){
                
                LCD_WRITE_CHAR('8');
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
    if(RD6){//'0'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('0');
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
            else if(take_input == 1){  
                LCD_WRITE_CHAR('0');
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
    
    c1 = 0; c2 = 0; c3 = 1;
    if(RD3){//'3'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('3');
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
            else if(take_input == 1){
                LCD_WRITE_CHAR('3');
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
    if(RD4){//'6'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('6');
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
            else if(take_input == 1){
                LCD_WRITE_CHAR('6');
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
    if(RD5){//'9'
        if(EEPROM_Read(0) == 0xFF){
                LCD_WRITE_CHAR('9');
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
            else if(take_input == 1){
                LCD_WRITE_CHAR('9');
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
    if(RD6){//'#'
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
                        EEPROM_Write(4,0);
                        main_menu();
                    }
                    else{
                        LCD_CLEAR();
                        LCD_SET_CURSOR(1,5);
                        LCD_WRITE_STRING("Pass Incorrect"); blink();
                        tries++;
                        EEPROM_Write(4,tries);
                        if(EEPROM_Read(4) >= 4)
                        {
                            EEPROM_Write(4,0);
                            EEPROM_Write(2,3);
                            TMR1ON = 1;
                            lock_down();
                        }
                        else main_menu();
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
                            tries++;
                            EEPROM_Write(4,tries);
                            if(EEPROM_Read(4) >= 4)
                            {
                                EEPROM_Write(4,0);
                                EEPROM_Write(2,3);
                                TMR1ON = 1;
                                lock_down();
                            }
                            else main_menu();
                        }
                    }
                    else if(line == 2){
                        EEPROM_Write(0,(password/100));
                        EEPROM_Write(1,(password%100));
                        LCD_CLEAR();
                        LCD_SET_CURSOR(1,5);
                        LCD_WRITE_STRING("Pass Changed"); blink();
                        EEPROM_Write(4,0);
                        main_menu();
                    }
                }
            }
        __delay_ms(250);
    }
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
    TMR1ON = 0;
}
void lock_down(){
    LCD_CLEAR();
    LCD_SET_CURSOR(1,1);
    LCD_WRITE_STRING("Max 5 tries");
    LCD_SET_CURSOR(2,1);
    LCD_WRITE_STRING("Locked:4 Minutes");
    take_input = 3;
    screen = 3;
    TMR1ON = 1;
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
    TMR1ON = 0;
}
//========================================TMR1 END================================================
void __interrupt()ISR(){
    if(TMR1IF){
        TMR1 = preload; //preloaded value to 
        counter++;//if this variable counts 20 times this means that 1 second passed
        if(counter == 20)
        { //this means that it counted 1 second
            seconds = EEPROM_Read(3);
            seconds++;
            EEPROM_Write(3,seconds);
            if(EEPROM_Read(3) >= 240)
            {
                EEPROM_Write(3,0);//reset seconds 
                EEPROM_Write(2,0);//reset flag indicator
                main_menu();
            }
            counter = 0; //must reset the 1 second counter
        }
        TMR1IF = 0;
    }
}
