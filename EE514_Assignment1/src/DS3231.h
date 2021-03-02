#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<unistd.h>
#include<linux/i2c-dev.h>
#define BUFFER_SIZE 19      //0x00 to 0x12

#ifndef I2C_H_
#define I2C_H_

class DS3231{
	unsigned int bus;
	unsigned int device;
	int file;

public:
	DS3231(unsigned int bus, unsigned int device);
	virtual int open();
};
