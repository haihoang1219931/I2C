/*
 * Simple I2C example
 *
 * Copyright 2017 Joel Stanley <joel@jms.id.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <errno.h>

#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command,
                                     int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
	return ioctl(file,I2C_SMBUS,&args);
}


static inline __s32 i2c_smbus_read_byte_data(int file, __u8 command)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,command,
	                     I2C_SMBUS_BYTE_DATA,&data))
		return -1;
	else
		return 0x0FF & data.byte;
}
void readInfo(char *path, uint8_t addr, unsigned char *listRegs,int numRegs,unsigned char *output,int lengthOutput){
	int file, rc;	
	file = open(path, O_RDWR);
	if (file < 0)
		err(errno, "Tried to open '%s'", path); 

	rc = ioctl(file, I2C_SLAVE, addr);
	if (rc < 0)
		err(errno, "Tried to set device address '0x%02x'", addr);
	
	int i=0;
	for(i = 0 ; i < numRegs; i++){
		output[i] = i2c_smbus_read_byte_data(file, listRegs[i]);
	}
	close(file);
}
int main(int argc, char **argv)
{
	unsigned char name[6] = {0xF0,0xF1,0xF2,0xF3,0xF4,0xF5};	
	unsigned char buff[6];
	readInfo("/dev/i2c-7",0x3c,name,sizeof(name),buff,sizeof(buff));
	printf("Board name: ");
	int i=0;
	for(i = 0 ; i < sizeof(buff); i++){
		printf("%c",buff[i]);
	}
	printf("\r\n");
}
