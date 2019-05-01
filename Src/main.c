#include "main.h"
#include <xc.h>
#include <stdbool.h>

void init(void);
static int send_data(uint8_t *data);

void main(void) {

  static uint8_t receive_data[8] = {};
  
  init();
  while(true){
    if(I2C_ReceiveCheck()){
      for(int i=0;i<8;i++){
	receive_data[i] = rcv_data[i];
      }
    }
    send_data(receive_data);
  }
}

static int send_data(uint8_t *data){
  static bool datachange_flag = false;
  static int send_data_num = 0;
  if(DATA_CLOCK == 0){
    datachange_flag = true;
  }
  if(DATA_CLOCK == 1 && datachange_flag){
    send_data_num++;
    if(send_data_num >= 8) send_data_num = 0;
    DATA_SELECT_0 = send_data_num & 0x01;
    DATA_SELECT_1 = send_data_num>>1 & 0x01;
    DATA_SELECT_2 = send_data_num>>2 & 0x01;
    DATA_OUT_0 = data[send_data_num] & 0x01;
    DATA_OUT_1 = data[send_data_num]>>1 & 0x01;
    DATA_OUT_2 = data[send_data_num]>>2 & 0x01;
    DATA_OUT_3 = data[send_data_num]>>3 & 0x01;
    DATA_OUT_4 = data[send_data_num]>>4 & 0x01;
    DATA_OUT_5 = data[send_data_num]>>5 & 0x01;
    DATA_OUT_6 = data[send_data_num]>>6 & 0x01;
    DATA_OUT_7 = data[send_data_num]>>7 & 0x01;
    datachange_flag = false;
  }
}

void init(void){
  uint8_t addr = 0x70;

  // Set oscilation
  OSCCON = 0xF0; //PLL　Enable

  // Set pin mode
  ANSELA = 0x00;
  ANSELB = 0x00;
  TRISA  = 0b00000000;
  TRISB  = 0b00000000;
  TRISC  = 0b00100000;
  
  I2C_init(addr);//アドレス
}

void interrupt  HAND(void){
  Slave_Interrupt();
}
