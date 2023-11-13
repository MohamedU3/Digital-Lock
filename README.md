# Digital-Lock

## Why did I do this project?.
I challenged myself to do a project on the things that I have learned on the first edition of the PIC tutorial so I decided to do a project on EEPROM.
I got an idea to make a digital lock and decided to use LCD to interface everything (without I2C) and chose 4 bit mode for the LCD.
I wanted to add a keypad in the beginning but decided to just go with 4 buttons (which are a small part of a keypad) and adjusted the code to be flexible so that I can easily add any type of keypad later, buttons are 1, 2, back and enter(back could be the symbol "*" and enter could be "#" on the keypay, but you can set them to anything you want).
So I implemented all of the LCD and EEPROM initializations and commands from scratch and wrote the code for the lock.

## How to Install and Run the Project:
1-Make sure to load and include all of the LCD and EEPROM source and header files.
2-if you want to run it on proteus make sure you add the HEX file to the PIC MCU on proteus

## Code explanation:
The code is written on MPLAB X IDE, It has comments explaining everything (almost everything).
The main trick of the code is that the "1" and "2" buttons have two functions:
1- if you are in the main menu then 1 and 2 buttons aren't inputs for a password, instead they are used to choose between options on the screen and these options are "1- Enter pass 2-New Pass".
2-if you are in one of the options, this means that the buttons are used for input to enter a password.
That's why I decided to use multiple flags to indicate which screen you are on and based on it the functions of the button will change(flags are explained in comments).
I am willing to add a feature in the future to give you 3 attempts to enter the password and if you failed you will get a 5-min cooldown, that's why TMR1 module initializations 
are implemented in the code.

## How it works?
When you first start the digital lock it will ask you to set your first password, after that it will show you the main menu which has 2 options : 1- enter pass 2-change the current pass.
If you turn the lock off and on again it won't show you the same screen that it showed you when you first started it, that's because password is already set, also password won't be deleted if it's shut on and off (obviously, I mean it's a lock duuh 🤓?).
You can change the pass by pressing 2 and typing the old pass first then you can set the new one, if you enter the password incorrectly it will show you "Pass Incorrect" and it sends you back to the main menu, I am willing to add a feature in the future to give you 3 attempts to enter the password and if you failed you will get a 5-min cooldown, I already have TMR1 module initializations implemented in the code to add that feature in the future.

## Code adjument:
if you wanted to add a keypad you have to change the name of the buttons pressed and then copy the code for the buttons 1,2 and paste that same code on every other number on the keypad from 0~9 (I recommend making a function that gets a char based on the number instead of copy and pasting).
