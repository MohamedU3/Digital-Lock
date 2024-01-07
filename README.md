# Digital-Lock

## Motivation
This project was born out of the initial PIC tutorial and evolved to explore EEPROM functionality, focusing on a digital lock integrated with an LCD. Transitioning to a keypad interface from the initial 4 buttons (now replaced), this design allows for expanded options and seamless keypad integration. The code architecture was carefully crafted to facilitate effortless future expansions.

## Installation and Execution
1- Ensure inclusion of all LCD, EEPROM, and keypad source and header files.
2- For Proteus simulation, load the HEX file onto the PIC MCU.

## Code Overview
Developed in MPLAB X IDE, the code offers detailed comments clarifying its operations. Notably, the keypad replaces previous buttons for increased functionality. Flags manage active screens, enabling dynamic key operations. A recent addition includes a user lockout feature after 5 unsuccessful attempts, with a 4-minute lock duration, along with EEPROM password protection. Importantly, both time and retry counts persist in EEPROM even after restarts.

## Functionality
Upon startup, users set an initial password. The main menu offers options for password entry or modification, securely retained through power cycles. Incorrect inputs redirect users to the main menu. Future developments aim to incorporate a three-attempt mechanism with a cooldown, utilizing existing TMR1 module initializations.

## Code Adaptation
For keypad integration, modifications involve mapping keypad inputs and logic across digits (0-9). Encouraging modularity, functions to retrieve characters based on keypad inputs streamline the code for enhanced readability and scalability.
