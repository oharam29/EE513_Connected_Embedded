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
using namespace std;

int main(){
	DS3231 rtc(1, 0x68);
	//get current date and time of module
	rtc.getDateTime();
	//set time on the module
	rtc.setDateTime(2021, 5, 6,5, 9, 30, 0);
	//get the new date and time of module
	rtc.getDateTime();
	//get temp of rtc module
	rtc.getTemp();
	//set alarm 1
	rtc.setAlarm(1, 30, 4, 7, 30, 24);
	//get alarm 1
	rtc.getAlarm(1);
	//set alarm 2
	rtc.setAlarm(2, 6, 3, 7, 30, 0);
	//get alarm 2
	rtc.getAlarm(2)



	rtc.close();
	return 0;
}
