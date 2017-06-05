

// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// ADS1115
// This code is designed to work with the ADS1115_I2CADC I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADS1115_I2CADC#tabs-0-product_tabset-2
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define Tb 48
#define Db 9650
#define Tc 11
#define Dc 24890

float temp_calc(int D);

int file;
	
	
	void main()
	{
	// Create I2C bus
	
	char *bus = "/dev/i2c-1";
	if ((file = open(bus, O_RDWR)) < 0)
	{
	printf("Failed to open the bus. \n");
	exit(1);
	}
	// Get I2C device, ADS1115 I2C address is 0x48(72)
	ioctl(file, I2C_SLAVE, 0x48);
	// Select configuration register(0x01)
	// AINP = AIN0 and AINN = AIN1, +/- 2.048V
	// Continuous conversion mode, 128 SPS(0x84, 0x83)
	
	// Read 2 bytes of data from register(0x00)
	// raw_adc msb, raw_adc lsb
	
	// Output data to screen
	float f=0.0;
	int D, a0, a1, a2, a3;
	
	while(1) {
		
		a0 = read_adc(0);
		a1 = read_adc(1);
		a2 = read_adc(2);
		a3 = read_adc(3);
		
		

	printf("A0=%d A1=%d A2=%d A3=%d Temp=%f\n", a0,a1,a2,a3, temp_calc(a3));
	
	
	}
	
	}
		

int read_adc(int channel) {
	
	char config[3] = {0};
	config[0] = 0x01;
	
	if (channel== 0) config[1] = 0xD3;	
	if (channel== 1) config[1] = 0xE3;	
	if (channel== 2) config[1] = 0xF3;	
	if (channel== 3) config[1] = 0xC3;
   

	config[2] = 0x83;
	write(file, config, 3);
	
	char reg[1] = {0x00};
	write(file, reg, 1);
	char data[2]={0};
	if(read(file, data, 2) != 2)
	{
	printf("Error : Input/Output Error \n");
	}
	else
	{
	// Convert the data
	int raw_adc = (data[0] * 256 + data[1]);
	if  (raw_adc >32767) raw_adc =0; 
    delay(1000);
	return raw_adc;
	}

}

void delay(int k) {
	int i=0;
	while(k>0) {
	for(i =0; i<=100000; i++);
	k=k-1;
    }
}


float temp_calc(int D) {
	
float f= (1.0*(Tb-Tc)*(D-Dc)/(Db-Dc)+Tc);
printf("\n T = %f   %d\n",f, D);
return f;
}
