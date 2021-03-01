//============================================================================
// Name        : EE514_Assignment1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<linux/i2c-dev.h>
#define BUFFER_SIZE 19      //0x00 to 0x12

<<<<<<< HEAD
// the time is in the registers in encoded decimal form
int bcdToDec(char b) { return (b/16)*10 + (b%16); }
=======
int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!

>>>>>>> branch 'main' of https://github.com/oharam29/EE513_Connected_Embedded

int main(){
   int file;
   printf("Starting the DS3231 test application\n");
   if((file=open("/dev/i2c-1", O_RDWR)) < 0){
      perror("failed to open the bus\n");
      return 1;
   }
   if(ioctl(file, I2C_SLAVE, 0x68) < 0){
      perror("Failed to connect to the sensor\n");
      return 1;
   }
   char writeBuffer[1] = {0x00};
   if(write(file, writeBuffer, 1)!=1){
      perror("Failed to reset the read address\n");
      return 1;
   }
   char buf[BUFFER_SIZE];
   if(read(file, buf, BUFFER_SIZE)!=BUFFER_SIZE){
      perror("Failed to read in the buffer\n");
      return 1;
   }
   printf("The RTC time is %02d:%02d:%02d\n", bcdToDec(buf[2]),
         bcdToDec(buf[1]), bcdToDec(buf[0]));
   close(file);
   return 0;
}
