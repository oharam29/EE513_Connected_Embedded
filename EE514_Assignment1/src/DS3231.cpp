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

int bcdToDec(char b) { return (b/16)*10 + (b%16); }

int DecTObcd(char b){ return ((b / 10) * 16) + (b % 10); }

DS3231::DS3231(unsigned int bus, unsigned int device) {
	this->file=-1;
	this->bus = bus;
	this->device =device;
	this->open();
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
		perror("Failed to read any value.\n");
		return 1;
	}
	cout << buffer[0] << endl;
	return buffer[0];
}

int DS3231::write(unsigned char value){
	unsigned char buffer[1];
	buffer[0] = value;
	if(::write(this->file, buffer, 1)!=1){
		perror("Failed to write to device");
		return 1;
	}
	return 0;
}

int DS3231::writeReg(unsigned int regAddr, unsigned char value){
	unsigned char buffer[2];
	buffer[0] = regAddr;
	buffer[1] = value;

	if(::write(this->file, buffer, 1)!=1){
		perror("Failed to write to device");
		return 1;
	}
	return 0;
}

int DS3231::getDateTime(){
	int values[7];

	for (int reg = 0x06; reg >= 0x00; reg--){
		values[reg] = bcdToDec(readReg(reg));
	}

	cout << "read" << endl;
	return 0;
}

int DS3231::setDateTime(unsigned int year, unsigned int month, unsigned int day){
	writeReg(0x04, DecTObcd(day));
	writeReg(0x05, DecTObcd(month));
	writeReg(0x06, DecTObcd(year-2000));

	return 0;
}

