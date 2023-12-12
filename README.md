# Digital-Lock

## Motivation
This project stemmed from a desire to apply my learnings from the initial PIC tutorial. I chose to delve into EEPROM functionality, conceptualizing a digital lock interfaced with an LCD. I opted for a 4-bit LCD mode to interface without I2C. Initially considering a keypad, I streamlined the design to incorporate four buttons, primed for future keypad integration. The buttons (1, 2, back, enter) were adaptable, allowing flexible customization (back symbolized as "*" and enter as "#"). The code was meticulously crafted to accommodate future expansions effortlessly.

## Installation and Execution
1- Ensure inclusion of all LCD and EEPROM source and header files.
2- For Proteus simulation, load the HEX file onto the PIC MCU.

## Code Overview
Developed using MPLAB X IDE, the code features comprehensive comments elucidating the system's functioning. Key to the code's versatility is the dual functionality of "1" and "2" buttons—navigational on the main menu and as password inputs in other screens. The code utilizes flags to determine the active screen, enabling dynamic button functionalities (flag explanations provided in comments). Future enhancements are planned, incorporating a three-attempt password feature with a 5-minute cooldown, for which the groundwork with TMR1 module initializations has been laid.

## Functionality
Upon startup, the digital lock prompts the user to set an initial password. The main menu presents two options: entering the password or changing it. Notably, the lock retains the password even after power cycles, ensuring continued security. Password modification requires entry of the old password followed by the new one. Incorrect password inputs redirect users to the main menu. Future developments will include a three-attempt mechanism with a cooldown period, facilitated by existing TMR1 module initializations.

## Code Adaptation
To integrate a keypad, modification involves renaming the button inputs and replicating the code logic across the keypad digits (0-9). Encouraging code modularity, it's advisable to create a function that retrieves characters based on the keypad digits rather than direct replication.
