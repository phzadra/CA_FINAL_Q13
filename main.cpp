/*
part of the MTU COMP71004 module
author: Krishna Panduru

the program here performs a serial passthrough operation between USB and serial (p9, p10). 
correct upto 5 errors for proper functionality. 
the 6th error is a runtime error generating the following error. correct that as well.

++ MbedOS Error Info ++ 
Error Status: 0x80010130 Code: 304 Module: 1 
Error Message: pinmap not found for peripheral 
Location: 0x4353 Error Value: 0xFFFFFFFF
*/

/*
1 error found: Serial is not a valid class for mbed OS 6, instead will use BufferedSerial
2 error found: p9 and p10 are not valid pins, instead will use D1 and D0 for UART4
3 error found: members .putc and .getc are not valid for new class, instead will use .read and .write
4 error found: missing ";" inside dev_recv() function
5 error found: device1.baud(115200); wrong variable, instead will use dev.baud(115200);
6 error found: dev.atach(&dev); miswritten, should be attach
7 error found: The Runtime error was due to a misdeclared pins on BufferedSerial pc (Pin Tx, Pin Rx) has to be followed

There are actually many errors with the initial program because it was actually intended to be run on mbed OS 2
The written program will only work properly if there is a closed loop between D0 and D1 so it will print on the terminal

*/

#include "mbed.h"


DigitalOut myled(LED1);
BufferedSerial pc(USBTX, USBRX); //Serial pc(USBRX, USBTX);
BufferedSerial dev(D1, D0); //RX, TX //Serial dev(p9, p10);

void dev_recv(){
    while (1){
        char recv_data;
        if (dev.read(&recv_data, 1) > 0){//while(dev.readable()){
            pc.write(&recv_data, 1);    //pc.putc(dev.getc())
        }    
    }
}

void pc_recv() {
    while (1) {
        char recv_data;
        if (pc.read(&recv_data, 1) > 0) {//while(pc.readable()){
            dev.write(&recv_data, 1);//dev.putc(pc.getc());
        }
    }
}

int main() {
    pc.set_baud(9600); //pc.baud(9600);
    dev.set_baud(115200);//device1.baud(115200);
    Thread pc_thread;
    Thread dev_thread;

    pc_thread.start(pc_recv); //pc.attach(&pc_recv);
    dev_thread.start(dev_recv); //dev.atach(&dev);
    printf("Hello!! \r\n");
    
    while(1) {
        myled = 1;
        wait_us(1000000);//wait(1);
        myled = 0;
        wait_us(1000000);//wait(1);
    }
}