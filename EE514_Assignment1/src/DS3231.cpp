#include"DS3231.h"
#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<string>
#include<iostream>
#include<sstream>
#define BUFFER_SIZE 19      //0x00 to 0x12
using namespace std;


DS3231::DS3231(unsigned int bus, unsigned int device) {
	this->file=-1;
	this->bus = bus;
	this->device =device;
	this->open();
	this->readReg(0x00);
}


int DS3231::open()
{
	string name;
	if(this->bus==0){
		name = "/dev/i2c-0";
	}
	else name = "/dev/i2c-1";

	if((this->file=::open(name.c_str(),O_RDWR)) < 0)
	{
		perror("I2C: failed to open the bus\n");
		return 1;
	}
	if(ioctl(this->file, I2C_SLAVE, this->device) < 0)
	{
		perror("I2C: Failed to connect to the device\n");
		return 1;
	}
	cout << "Connected" << endl;
	return 0;
}

unsigned char DS3231::readReg(unsigned int regAddr){
	unsigned char buffer[1];
	if(::read(this->file, buffer, 1)!=1){
		perror("I2C: Failed to read any value.\n");
		return 1;
	}
	cout << buffer[0] << endl;
	return buffer[0];
}




