#include "30010_io.h"
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <charset.h>
#include <time.h>

#ifndef _ANSI_H_
#define _ANIS_H_
#define ESC 0x1B
#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( ((a) << FIX14_SHIFT) / b )

volatile uint32_t timeFlagPrint;
volatile uint32_t timeFlagA1;
volatile uint32_t timeFlagBullet;
volatile uint32_t timeFlagA2;
volatile uint32_t timeFlagTra;
volatile uint32_t timeFlagDrawT;
volatile uint32_t timeFlagScore;

struct timing_t {
    volatile uint32_t mikroSec, milliSec, centiSec, second, minute;
};

volatile struct timing_t timetime;

struct vector_t {
    int32_t x, y;
};

struct position {
    int32_t x, y;
};

struct velocity {
    int32_t vx, vy;
};

struct trang { //One of the baddest bois around!, he's got two things inside him,- burning hatred for the player, and 180 degrees
    uint8_t random;
    struct vector_t position;
    struct vector_t velocity;
    uint8_t hp;
    int8_t firsty;
    int8_t firstx;
};

struct sqwog { //Originally a general in the Alik'r army who was dishonoured by a great military defeat, he wanders the stars looking for players to kill such that he may regain his honour.
    struct vector_t position;
    struct vector_t velocity;
    uint8_t hp;
    int8_t firsty;
    int8_t firstx;
};

struct enemy {
    uint8_t randomNo;
    uint8_t enemyType;
    struct vector_t position;
    struct vector_t velocity;
    uint8_t hp;
    uint32_t firsty;
    uint32_t firstx;
};

struct ship_t {
    struct vector_t position;
    uint32_t hp;
};

struct asteroid_t {
    struct vector_t position;
    struct vector_t velocity;
};

struct bullet_t {
    struct vector_t position;
    struct vector_t velocity;
};

void hello();
void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgcolor();
void clrscr ();
void gotoxy (uint32_t c, uint32_t r);

void window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t lineColor);
uint8_t keyInput();

void moveShip (uint8_t x, struct ship_t *ship, struct ship_t *oldShip);
void printShip (struct ship_t ship, struct ship_t oldShip);

void moveAsteroid (uint8_t x, struct asteroid_t *asteroid, struct asteroid_t *oldAsteroid);
void printAsteroid (struct asteroid_t asteroid, struct asteroid_t oldAsteroid);

void boss (uint8_t x);
void lcd_write_string (char text[], uint16_t slice, uint8_t row, uint8_t (*buff)[512]);

uint8_t startBullet(struct ship_t ship, uint8_t p);
void moveBullet (uint8_t y, struct bullet_t *bullet, struct bullet_t *oldBullet);
void printBullet (struct bullet_t bullet, struct bullet_t oldBullet);

uint8_t compBuAs(struct bullet_t bullet, struct asteroid_t asteroid);
uint8_t compBuEn(struct bullet_t bullet, struct enemy e);
uint8_t compDoSh(struct ship_t ship, struct asteroid_t dodge);

uint8_t enemyBreach(struct enemy e);

//TRANG BOIIIIIIII
void fixtrangPos(struct trang (*t));
void trangNextPos(struct trang (*t));
void drawTrang (struct trang t);
void eraseTrang (struct trang t);
void trangZag(struct trang (*t));
uint8_t randoms(uint8_t lower, uint8_t upper);
void awakenTrang(uint8_t spawn);

//C'EST LE MEILLEUR SQUWOGUE
void sqwogNextPos(struct sqwog (*t));
void drawSqwog (struct sqwog t);
void eraseSqwog (struct sqwog t);
void sqwogBox (struct sqwog (*t));
void awakenSqwog(uint8_t spawn);

//Generalised enemy functions
void enemyNextPos (struct enemy *e);
void drawEnemy (struct enemy e);
void eraseEnemy (struct enemy e);
void decideVel (struct enemy *e);
void enemyMotion (struct enemy *e);

void moveDodge (uint8_t y, struct asteroid_t *dodge, struct asteroid_t *oldDodge);
void printDodge (struct asteroid_t dodge, struct asteroid_t oldDodge);

#endif
