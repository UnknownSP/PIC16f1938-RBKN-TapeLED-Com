#include "main.h"
#include <xc.h>
#include <stdbool.h>

void init(void);
void SendToMotor(uint16_t speed,uint8_t stat);

void main(void) {
    uint16_t speed;
    uint8_t mode;
    uint8_t com_flg = 0;

    init();

    mode=0;

    while(true){

        if(I2C_ReceiveCheck()){
            if (com_flg == 0) com_flg = 1;
            speed = (rcv_data[0]&0x03)<<8;//STM仕様に変更
            speed |= rcv_data[1];
            mode = (rcv_data[0] >> 2) & 0x03;
            CLRWDT();
        }
        else if (com_flg == 0){
            CLRWDT();
        }
        PWMSet(speed,mode);
    }
}

void init(void){
  uint8_t addr = 0x10;

  // Set oscilation
  OSCCON = 0xF0; //PLL　Enable

  // Set pin mode
  ANSELA = 0x00;
  ANSELB = 0x00;
  
  // Set watch dog
  WDTCON = 0x13;

  addr |= (PORTAbits.RA0 << 0);
  addr |= (PORTAbits.RA1 << 1);
  addr |= (PORTAbits.RA2 << 2);
  addr |= (PORTAbits.RA5 << 3);

  I2C_init(addr);//アドレス
  PWMInit();
}

void interrupt  HAND(void){
    Slave_Interrupt();
}
