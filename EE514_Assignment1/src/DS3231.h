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

#ifndef I2C_H_
#define I2C_H_

class DS3231{
private:
	unsigned int bus;
	unsigned int device;
	int file;

public:
	DS3231(unsigned int bus, unsigned int device);
	virtual int open();
	unsigned char readReg(unsigned int regAddr);
	virtual int write(unsigned char value);
	virtual int writeReg(unsigned char regAddr, unsigned char value);
	virtual int getDateTime();
	void int setDateTime(unsigned int year, unsigned int month, unsigned int day);
};

#endif /* I2C_H_ */
