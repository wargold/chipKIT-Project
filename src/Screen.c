#include <stddef.h>
#include <stdint.h>
#include <pic32mx.h> 

#define CMD (PORTFCLR = 0x10) //Enter command mode.
#define DATA (PORTFSET = 0x10) //Enter data mode.
#define ADDRESS_MODE_HORIZONTAL { spi_sendData(0x20); spi_sendData(0x0); delay(100);}
#define ADDRESS_MODE_PAGE {spi_sendData(0x20); spi_sendData(0x2); delay(100);} 
#define PAGE_SELECT(a) {spi_sendData(0xb0 + (a & 3)); delay(100);} //Only for use with page adress mode.
#define PAGE_COLUMN_SELECT(a) {spi_sendData(0x00 + (a & 0x0f)); spi_sendData(0x10 + (a & 0xf0));delay(100);} //Only for use with page address mode.
#define PAGE_SPAN(a,b) { spi_sendData(0x22); spi_sendData(a); spi_sendData(b); delay(100);} //Only for use with horizontal address mode.
#define COLUMN_SPAN(a,b) { spi_sendData(0x21); spi_sendData(a); spi_sendData(b); delay(100);} //Only for use with horizontal address mode.


const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

void spi_sendData(uint8_t byte) {
	while(!(SPI2STAT & 0x08)); //Wait for previous transfers to finish.
	SPI2BUF = byte;
	while(!(SPI2STAT & 1)); //Wait for recieve buffer to be full.
}

void delay(int ms) {
	int i;
	for(i = 0; i < ms; i++);
}

/*
 * Updates given pages of the display with content from pointer displayMem.
 */
void display_Update(uint8_t *displayMem, uint8_t startPage, uint8_t endPage) {
	CMD; //cmd mode.
	ADDRESS_MODE_HORIZONTAL;
	COLUMN_SPAN(0,127);
	PAGE_SPAN(startPage,endPage);
	int page;
	int col;
	DATA;
	for(page = startPage; page <= endPage; page++) {
		for(col = 0; col <= 127; col++) {
			spi_sendData(displayMem[(page * 128) + col]);
		}
	}
}
/*

 * Takes an array of chars and writes font values that display the chars to array
 * pointer *val. Val should reserve 8 times the memory of *str.  
 */
void charToDisplayValues(char *str, int strLen, uint8_t *val, int startIndx) {
	int strIndx;
	int fontIndx;
	int start = startIndx;
	for(strIndx = 0; strIndx < strLen; strIndx++) {
		for(fontIndx = 0; fontIndx < 8; fontIndx++, start++) {
			val[start] = font[(str[strIndx] * 8) + fontIndx];
		}
	}
}


/*
 * Initializes the display according to the
 * safe steps given in the datasheet.
 */
void init_disp( void ) {
	OSCCON &= ~0x180000;
	OSCCON |= 0x080000;

	/* Initialize display GPIO ports. */
	TRISG = 0xffff;
	TRISF = 0xfffe;

	TRISGCLR = 0x200; //Pin 9 used to reset OLED.
	TRISFCLR = 0x70; //Pins 4,5,6 used for data/command, VBAT enable, VDD enable respectively.
	PORTGSET = 0x200; //Reset initially off.
	PORTFSET = 0x60; //VBAT and VDD initially off.
	PORTFCLR = 0x10; //Display in command mode initially.

	SPI2CON = 0; //Clear con register.
	SPI2BUF = 0; //Clear buffer.
	SPI2STATCLR = 0x40;//Clear SPIROV bit.
	SPI2CONSET = 0x60; //Set CKP bit & master enable.
	SPI2BRG = 15; //Could be changed.
	SPI2CONSET = 0x8000; //Turn SPI port on.

	//We're already in command mode.
	PORTFCLR = 1 << 6; //Turn VDD on.
	delay(1000000);	
	spi_sendData(0xAE); //Set display OFF. 	
	PORTGCLR = 1 << 9; //Reset is driven low = being reset.
	delay(10);	
	PORTGSET = 1 << 9; //Reset is driven high = normal operation.
	delay(10);	

	spi_sendData(0x8D);
	spi_sendData(0x14);
	spi_sendData(0xD9);
	spi_sendData(0xF1);

	PORTFCLR = 0x20; //Enable VBAT.
	delay(1000000);
	spi_sendData(0xA1);
	spi_sendData(0xC8);
	spi_sendData(0xDA);
	spi_sendData(0x20);
	spi_sendData(0xAF);
}

