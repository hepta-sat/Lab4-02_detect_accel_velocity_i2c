#include "mbed.h"
#include "HEPTA_EPS.h"
#define ACC 0x19<<1 //addr_accel

RawSerial pc(USBTX,USBRX,9600);
HEPTA_EPS eps(p16,p26);
I2C i2c(p28,p27);

float accel[3];
char cmd[2]={0};
uint8_t data[6]={0};
char send[1];

int main() {
    i2c.frequency(100000);
    pc.printf("accel setting\r\n");
    cmd[0]=0x14; //softwareset 
    cmd[1]=0xB6; //triggers a reset
    i2c.write(ACC,cmd,2); 
    cmd[0]=0x0F; //acceleration measurement range
    cmd[1]=0x05; //+-4g
    i2c.write(ACC,cmd,2); 
    cmd[0]=0x11; //Selection of the main power modes and low power sleep period
    cmd[1]=0x00; //NORMAL mode, Sleep duration = 0.5ms
    i2c.write(ACC,cmd,2); 
    while(1) {
        send[0]=(char)(2);
        i2c.write(ACC,send,1,true);
        i2c.read(ACC,(char*)data,6);
        for(int i=0;i<3;i++){
            accel[i]=(int16_t)(((int16_t)data[i*2+1]<<8) | data[i*2]) >> 4;
            if(accel[i]>2047)accel[i]-=4096;
            accel[i]=accel[i]/519*9.8;
        }
        pc.printf("ax = %2.4f, ay = %2.4f, az = %2.4f\r\n",accel[0],accel[1],accel[2]);
        wait(1);
    }
}