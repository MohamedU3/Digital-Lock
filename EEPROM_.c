#include "EEPROM_.h"

void EEPROM_Write(uint8_t address,uint8_t data){
    uint8_t GIE_STATE = GIE;
    while(EECON1bits.WR);
    EEADR = address;
    EEDATA = data;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1;
    GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    GIE = GIE_STATE;
    EECON1bits.WREN = 0;
    EECON1bits.WR = 0;
}
uint8_t EEPROM_Read(uint8_t address){
    uint8_t data;
    EEADR = address;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1;
    data = EEDATA;
    return data;
}