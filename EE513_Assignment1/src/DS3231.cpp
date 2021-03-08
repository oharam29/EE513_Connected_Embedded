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

void DS3231::close(){
	::close(this->file);
	this->file = -1;
}

DS3231::~DS3231(){
	if(file!=-1){
		this->close();
	}
}

unsigned char DS3231::readReg(unsigned char regAddr){
	this->write(regAddr);
	unsigned char buffer[1];
	if(::read(this->file, buffer, 1)!=1){
		perror("Failed to read any value.\n");
		return 1;
	}
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

int DS3231::writeReg(unsigned char regAddr, unsigned char value){
	unsigned char buffer[2];
	buffer[0] = regAddr;
	buffer[1] = value;

	if(::write(this->file, buffer, 2)!=2){
		perror("Failed to write to device");
		return 1;
	}
	return 0;
}

int DS3231::getDateTime(){
	int values[7];
	cout << "------------------------" << endl;
	cout << "Begin getDateTime() fucntion" << endl;
	cout << "------------------------" << endl;
	cout << "Reading date and time from DS3231: " << endl;

	for (int reg = 0x06; reg >= 0x00; reg--){
		values[reg] = bcdToDec(readReg(reg));
	}

	if(values[3] == 1){
		cout << "The day of the week is Sunday" << endl;
	}
 	else if(values[3] == 2){
		cout << "The day of the week is Monday" << endl;
	}
 	else if(values[3] == 3){
		cout << "The day of the week is Tuesday" << endl;
	}
 	else if(values[3] == 4){
		cout << "The day of the week is Wednesday" << endl;
	}
 	else if(values[3] == 5){
		cout << "The day of the week is Thursday" << endl;
	}
 	else if(values[3] == 6){
		cout << "The day of the week is Friday" << endl;
	}
 	else if(values[3] == 7){
		cout << "The day of the week is Saturday" << endl;
	}

	cout << "The time is: " << values[2] << ":" << values[1] << ":"  << values[0] << "." <<endl;
	cout << "The date is: " << values[4] << "/" << values[5] << "/" << values[6] << "." << endl;
	cout << "getDateTime() finished" << endl;
	return 0;
}

int DS3231::setDateTime(unsigned int year, unsigned int month, unsigned int day, unsigned int DOW, unsigned int hour, unsigned int minute, unsigned int second){

	cout << "------------------------" << endl;
	cout << "setDateTime() begin" << endl;
	cout << "------------------------" << endl;

	cout << "Setting time and date to: " << hour << ":" << minute << ":" << second << " and " << day << "/" << month << "/" << year << endl;
	if(minute > 59 || second > 59){
		cout << "Error value outside of 0-59 range" << endl;
	}
	else{
		writeReg(0x00, DecTObcd(second));
		writeReg(0x01, DecTObcd(minute));
	}

	writeReg(0x02, DecTObcd(hour));
	if(DOW > 7){
		cout << "Error value outside 1-7 range" << endl;
		return 1;
	}
	else{
		writeReg(0x03, DecTObcd(DOW));
	}
	writeReg(0x04, DecTObcd(day));
	writeReg(0x05, DecTObcd(month));
	writeReg(0x06, DecTObcd(year-2000));

	cout << "Time set complete" << endl;

	return 0;
}

int DS3231::getTemp(){
	unsigned char tempReg = 0x11;

	cout << "------------------------" << endl;
	cout << "Begin getTemp() fucntion" << endl;
	cout << "------------------------" << endl;

	cout << "Reading temp from DS3231: " << endl;

	int x = bcdToDec(readReg(tempReg));
	cout << "The RTC Temperature is: "<< x << endl;

	cout << "getTemp() finished" << endl;
	return 0;
}

int DS3231::getAlarm(int Alarm){
	int values[5];
	int seconds;
	int minutes;
	int hours;
	int day;
	int date;

	cout << "------------------------" << endl;
	cout << "getAlarm() begin" << endl;
	cout << "------------------------" << endl;
	if(Alarm == 1 || Alarm == 2){
		if(Alarm == 2){
			minutes = 0x0B;
			hours = 0x0C;
			day = 0x0D;
			date = 0x0D;
		}
		else{
			seconds = 0x07;
			minutes = 0x08;
			hours = 0x09;
			day = 0x0A;
			date = 0x0A;
			values[0] = bcdToDec(readReg(seconds));
		}
		values[1] = bcdToDec(readReg(minutes));
		values[2] = bcdToDec(readReg(hours));
		values[3] = bcdToDec(readReg(day));
		values[4] = bcdToDec(readReg(date));

		if(Alarm == 1){
			cout << "Alarm " << Alarm << " is set for: " << values[2] << ":" << values[1] << ":" << values[0] << endl;
		}
		if(Alarm == 2){
			cout << "Alarm " << Alarm << " is set for: " << values[2] << ":" << values[1] << " on " << values[3] << "/" << values[4] << endl;
		}
		return 0;
	}
	else{
		cout << "Alarm value must be 1 or 2" << endl;
		return 1;
	}

}
void DS3231::setAlarm(int Alarm, unsigned int date, unsigned int day, unsigned int hour, unsigned int minute, unsigned int second){

	cout << "------------------------" << endl;
	cout << "setAlarm() begin" << endl;
	cout << "------------------------" << endl;

	if(Alarm == 1){
		writeReg(0x07, DecTObcd(second));
		writeReg(0x08, DecTObcd(minute));
		writeReg(0x09, DecTObcd(hour));
		writeReg(0x0A, DecTObcd(day));
		writeReg(0x0A, DecTObcd(date));
	}
	else{
		writeReg(0x0B, DecTObcd(minute));
		writeReg(0x0C, DecTObcd(hour));
		writeReg(0x0D, DecTObcd(day));
		writeReg(0x0D, DecTObcd(date));
	}

}

void DS3231::readControlByte(int byte){
	char cmd[2];
	cmd[0] = 0x0F;
	cmd[1] = 0x0E;

	if(byte == 1){
		readReg(cmd[0]);
	}
	else if(byte == 2){
		readReg(cmd[1]);
	}
}
void DS3231::writeControlByte(unsigned char con, int byte){
	char data1[2];
	char data2[2];

	data1[0] = 0x0F;
	data1[1] = con;

	data2[0] = 0x0E;
	data2[1] = con;

	if(byte == 1){
		writeReg(data1[0], data1[1]);
	}
	else if(byte == 2){
		writeReg(data2[0], data2[1]);
	}
	else{
		cout << "The parameter value entered must be 1 or 2 " << endl;
	}
}

void DS3231::TriggerAlarm(int Alarm){
	char data[2];
	unsigned char status
	data[0] = 0x0F;

	readControlByte(1);
	status=data[1];

	//not sure what todo from here

}
