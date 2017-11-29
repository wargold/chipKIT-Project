#include <stdint.h>
#include <stddef.h>
#include <pic32mx.h>

void clearMem(uint8_t *memory, int startPage, int endPage, int startCol, int endCol);
void addIconToMem( uint8_t *memory, const uint8_t *icon, int height, int width, int startCol, int startPage);

#define BTN4 (PORTD & (4 << 5))
#define BTN3 (PORTD & (2 << 5))
#define BTN2 (PORTD & (1 << 5))

const uint8_t scarySnowMan[] = {
	0,0,0,0,0,0,0,240,240,240,240,240,240,240,240,240,240,240,0,0,0,0,0,0,0,
	0,0,0,0,7,7,7,63,209,163,165,169,161,169,165,163,209,63,7,7,7,0,0,0,0,
	4,4,31,4,4,4,62,65,128,128,128,182,182,128,128,128,128,65,62,4,4,4,31,4,4};

const uint8_t HIRO[] = {
	252,130,90,90,66,90,90,130,252,
	128,193,97,63,63,109,205,141,12};

const uint8_t snowballPic[] = {6,7,6};

uint8_t snowManDelay;
uint8_t GAMEOVER;
const uint8_t const snowBallSpeed = 2;

struct gameObject {
	uint8_t x;
	uint8_t y;
	uint8_t width; 
	uint8_t height;
	const uint8_t *picture;
};

struct gameCharacter {
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t height;
	const uint8_t *picture; //The picture.
	const uint8_t *extraPicture1;
};


void init_control_and_timer() {
	T4CON = 0;
	TMR4 = 0;
	T4CON = 0x070;
	IFS(0) &= ~( 1 << 16 );
	PR4 = (80000000 / 256) / 30; //30FPS.
	T4CON |= 1 << 15;
	TRISD |= (7 << 5); //Button 2,3,4 as input.
}

/*
 * Sets each character and object's icon, start position and dimensions.
 */
void init_GameObjects(struct gameCharacter* hero, struct gameCharacter* snowman, struct gameObject* snowball) {
	hero->picture = HIRO;
	hero->x = 10;
	hero->y = 2;
	hero->width = 9; //9 pixels wide.
	hero->height = 16; //16 pixels high.

	snowman->picture = scarySnowMan;
	snowman->height = 24;
	snowman->width = 25;
	snowman->x = 102; 
	snowman->y = 1;

	snowball->x = snowman->x;
	snowball->y = snowman->y + 1;
	snowball->height = 8;
	snowball->width = 3;
	snowball->picture = snowballPic;
}


void updatePositions(struct gameCharacter *hero, struct gameCharacter *snowman, struct gameObject *snowball, uint8_t *gameMemory) {
	clearMem(gameMemory, snowman -> y, snowman -> y + 2, snowman -> x, snowman ->x + snowman -> width);
	clearMem(gameMemory, snowball -> y, snowball -> y, snowball -> x, snowball ->x + snowball -> width);
	clearMem(gameMemory, hero -> y, hero -> y + 1, hero -> x, hero ->x + hero -> width);

	uint8_t heroOldX = hero -> x;
	uint8_t heroOldY = hero -> y;
	uint8_t snowBallOldX = snowball -> x;
	uint8_t snowBallOldY = snowball -> y;

	if ( BTN4 ) { //User wants to move left or down.
		if(BTN2 && hero->y != 2) {hero->y += 1;}
		else if(!BTN2 && hero -> x >= 5)
			hero -> x -= 5;
	}
	if( BTN3 ) { //User wants to move up or right.
		if(BTN2 && hero->y != 0) {hero->y -= 1;}
		else if(!BTN2 && hero->x != 115) {
			hero->x += 5;
		}
	}

	if(snowManDelay == 5) {
		snowManDelay = 0;
		if (hero -> y) {
			snowman -> y = 1;
		} 
		else {
			snowman -> y = 0;
		}
	}
	if( snowball -> x < snowBallSpeed * (hero ->x / 30) + 3) {
		snowball -> x = snowman -> x; 
		if(hero -> x % 2 == 0) {
			snowball -> y = hero -> y ;
		}
		else {
			snowball -> y = hero -> y + 1;	
		}
	}
	else {
		snowball -> x -= snowBallSpeed * (hero -> x / 30) + 3;
	}
	if( snowBallOldX >= heroOldX && snowball -> x <= hero -> x && (snowball -> y == hero -> y || snowball -> y == hero -> y + 1) ) {
		GAMEOVER = 1;
	}
	addIconToMem(gameMemory, hero->picture, hero->height,hero->width,hero->x,hero->y);
	addIconToMem(gameMemory, snowball->picture, snowball->height, snowball->width, snowball->x, snowball->y);
	addIconToMem(gameMemory, snowman->picture,snowman->height,snowman->width,snowman->x,snowman->y);
}

void mainLoop(uint8_t *gameMemory) {
	struct gameCharacter hero;
	struct gameCharacter snowman;
	struct gameObject snowball;
	GAMEOVER = 0;
	snowManDelay = 0;
	init_GameObjects(&hero, &snowman, &snowball);
	init_control_and_timer();
	while( !GAMEOVER ) {
		while(!(IFS(0) & (1 << 16)));
		snowManDelay++;
		updatePositions( &hero, &snowman, &snowball, gameMemory);
		display_Update(gameMemory,0,3);
		IFS(0) &= ~(1 << 16);
		TMR4 = 0;
	}
	uint8_t endCounter = 0;
	while(endCounter < 30) { //Wait 3 seconds to show player how/where he died.
		while(!(IFS(0) & (1 << 16)));
		endCounter++;
		IFS(0) &= ~ ( 1 << 16 );
	}
}

void startColdGame(uint8_t *gameMemory) {
	clearMem(gameMemory, 0, 3, 0, 127);
	mainLoop(gameMemory);
	T4CON = 0; //Stop timer.
}
