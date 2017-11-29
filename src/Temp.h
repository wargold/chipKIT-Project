#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of integer sizes and the like, part 2 */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

#define TEMP_ADDR 0x48 //7 bit address of temp-sensor in binary is 1001000.

int8_t getTemp( void );
int8_t potens(int x, int y);

/*
 * Converts a signed 8 bit int in the range -99:99 to a string, if int8_t is negative
 * a '-' is set as first char of string. Final string is stored in the parameter
 * char pointer *temp which needs to allocate at least 3 bytes.  
 */
void int8ToChar(int8_t x, char *temp) {
	int neg = x & ( 1 << 7); //Check signed bit.
	if( neg ) { //If negative, take two's complement. 
		x = (~x) + 1;
	}
	int i;
	for(i = 0; i < 7; i++) {
		if(x & (1 << i)) {
			if(temp[2] + potens(2,i) > 9) {
				int8_t ental = (temp[2] + potens(2,i)) % 10;
				temp[1] += ((temp[2] + potens(2,i)) - ental) / 10; //Tiotalet.
				temp[2] = ental;
			}
			else {
				temp[2] += potens(2,i);
			}
		}
	}
	if(neg) {temp[0] = '-';}
	else {temp[0] = 0;}
	temp[1] += 0x30; //Convert the numbers to char hex.
	temp[2] += 0x30;
}
/*
 * Changes a string to say " 17c".
 */
void tempToChar(char *disp, int8_t temp) {
	//int8_t temp = getTemp(); TEMPORARILY DISABLED UNTIL WE HAVE WORKING TEMP SENSOR.
	char tempAsChar[3] = {0,0,0};
	int8ToChar(temp,tempAsChar);
	disp[0] = 0;
	disp[1] = tempAsChar[0];
	disp[2] = tempAsChar[1];
	disp[3] = tempAsChar[2];
	disp[4] = 'c';
}


/* Initializes an I2C register */
void setup_I2C( void ) {
	I2C1RCV = 0; //Clear the recieve register.
	I2C1CON = 0; //Clear control register.
	I2C1BRG = 0x0C2; //Set baud rate.
	I2C1CON |= 0x8000; //Start the shizzle.
}

/* Asserts a start condition on the I2C1 port */
void start_condition( void ) {
	I2C1CONSET = 1;
	while(I2C1CON & 0x1); //Wait until start sequence is done.
}
/* Asserts a stop condition on the I2C1 port */
void stop_condition( void ) {
	I2C1CONSET = 1 << 2;
	while(I2C1CON & 4); //Wait until stop sequence is done.
}
/*
 * Puts a byte on the I2C1TRN registry
 * for delivery to slave.
 */
void send_data(uint8_t data) {
	I2C1TRN = data;
	while(I2C1STAT & 0xc001); //Wait until data has been transferred & we've recieved an ACK.
}
/*Recieves data from a slave.
 * If NACKenable == 1 then returns a nack, else returns
 * an ack.
 */
uint8_t recieve_data(int NACKenable){
	int data;
	I2C1CONSET = 1 << 3; //Enable Master reception.
	while(I2C1CON & 0x8); //Wait until we've recieved everything we need.
	I2C1STATCLR = 1 << 6; //Clear overflow bit.
	data = I2C1RCV; //Set data to content of RCV register, where slaves send information.
	I2C1CON |= NACKenable << 5; //Assert that we want to send an ACK or a NACK. 
	I2C1CONSET = 1 << 4; //Send the (N)ACK.
	while(I2C1CON & 0x10); //Wait for (N)ACK to be recieved. 
	return data;
}


/*Returns the current temperature as an int8_t*/
int8_t getTemp( void ) {
	int temp;
	start_condition();
	send_data(TEMP_ADDR << 1); //Indicate a write on the temp-sensor. 
	send_data(0x0); //"Write" the temperature registry pointer to the sensor. 
	start_condition();
	send_data((TEMP_ADDR << 1) | 1); //Indicate a read on the temp-sensor. 
	temp = recieve_data(0) << 8; //Store first byte in bit positions 15-8 of temp, send back an ACK. 
	temp |= recieve_data(1); //Store 2nd byte in bit positions 7-0 of temp, send back NACK. 
	stop_condition();
	return ((int8_t) (temp >> 8));
}
/*
 * Returns @param base to the power of @param exponent.
 * Designed for use with only positive exponents. 
 */
int8_t potens(int base, int exponent) {
	int8_t sum = 1;
	int i;
	for(i = 0; i < exponent; i++) {
		sum *= base;
	}
	return sum;
}
