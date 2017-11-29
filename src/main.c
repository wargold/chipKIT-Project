#include <stddef.h>
#include <stdint.h>
#include <pic32mx.h>
#include "Screen.h"
#include "Temp.h"
#include "Games.h"

void addIconToMem( uint8_t *memory, const uint8_t *icon, int height, int width, int startCol, int startPage);

int counter;  //When counter hits 10 temperature is updated.
uint8_t displayMem[128 * 4]; //Represents the 128 * 32 pixel screen.
uint8_t animationPages[5];

const uint8_t snowflake[] = {24,153,90,60,255,60,90,153,24};

const uint8_t sun[] = {
	1,2,228,240,248,204,252,252,252,252,204,248,240,228,2,1,
	128,64,39,15,31,48,48,48,48,48,48,31,15,39,64,128};

void tempUpdate(int8_t tmp) {
	char temp[5];
	tempToChar(temp,tmp); //10 is the temperatureeee.
	charToDisplayValues(temp, 5, displayMem, 40);
}
/*
 * Updates the weather animation movement.
 */
void animationUpdate(int8_t tmp) {
	if(tmp < 10) {
		int i;
		for(i = 0; i < 5; i++) {
			if(animationPages[i] == 3) {
				animationPages[i] = 1;
			}
			else {
				(animationPages[i])++;
			}
		}
		for(i = 0; i < 5; i++) {
			addIconToMem(displayMem, snowflake,8,9, 29 * i, animationPages[i]);
		}
	}
	else {
		addIconToMem(displayMem, sun,16,16,105,0);
	}
}

void addIconToMem( uint8_t *memory, const uint8_t *icon, int height, int width, int startCol, int startPage) {
	int page;
	int col;
	int iconindx = 0;
	for(page = startPage; page < (startPage + height / 8); page++) {
		for( col = startCol; col < (startCol + width); col++, iconindx++) {
			memory[128 * page + col] = icon[iconindx];
		}
	}
}

/*
 * Clears the given rows and columns of the memory array passed as parameter.
 */
void clearMem(uint8_t *memory, int startPage, int endPage, int startCol, int endCol) {
	int row;
	int col;
	for(row = startPage;row <= endPage; row++) {
		for(col = startCol; col <= endCol; col++) {
			memory[(128 * row) + col] = 0;
		}
	}
}

void mem_init() {
	clearMem(displayMem, 0,3,0,127);
	char tempStr[] = {'T','e','m','p',':'};
	charToDisplayValues(tempStr, 5, displayMem, 0);
}

void timer_init() {
	T2CON = 0;
	T2CON = 0x070;
	TMR2 = 0;
	IFS(0) &= ~(1 << 8);
	PR2 = (80000000 / 256) / 10;
	T2CON |= 1 << 15; //start timer;
}
void user_isr(){return;}

void loopAway(){
	while(!(IFS(0) & ( 1 << 8 )));
	counter++;
	int8_t tmp;
	if(counter == 10) {
		tmp = 15;
		
		if(PORTD & ( 1 << 8) ) {
			startColdGame(displayMem);
			clearMem(displayMem,0,3,0,127);
			mem_init();
		}
		
		tempUpdate(tmp);
		clearMem(displayMem, 0, 1, 100, 127);
		clearMem(displayMem, 1,3, 0, 127);
		charToDisplayValues("Slide SW1", 9, displayMem, 128 * 3);
		animationUpdate(tmp);
		display_Update(displayMem,0,3);
		counter = 0;
	}
	IFS(0) &= ~( 1 << 8);
}

int main(void) {
	TRISD |=  1 << 8;
	int i;
	for(i = 0; i < 5; i++){
		animationPages[i] = i % 3 + 1;
	}
	counter = 0;
	init_disp();
	mem_init();
	display_Update(displayMem,0,3);
	timer_init();	
	while(1){
		loopAway();
	}
}
