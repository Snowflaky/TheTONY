#include "30010_io.h"
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef _ANSI_H_
#define _ANIS_H_
#define ESC 0x1B
#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( ((a) << FIX14_SHIFT) / b )

volatile uint8_t timeFlagPrint;
volatile uint8_t timeFlagA1;
volatile uint8_t timeFlagBullet;
volatile uint8_t timeFlagA2;
volatile uint8_t timeFlagTra;
volatile uint8_t timeFlagDrawT;

struct time_t {
    volatile uint32_t mikroSec, milliSec, centiSec, second, minute;
};

volatile struct time_t time;

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
    struct vector_t position;
    struct vector_t velocity;
    uint8_t hp;
    int8_t firsty;
    int8_t firstx;
};

struct sqwog { //One of the baddest bois around!, he's got two things inside him,- burning hatred for the player, and 180 degrees
    struct vector_t position;
    struct vector_t velocity;
    uint8_t hp;
    int8_t firsty;
    int8_t firstx;
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

//TRANG BOIIIIIIII
//void fixtrangPos(struct trang (*t));
void trangNextPos(struct trang (*t));
void drawTrang (struct trang t);
void eraseTrang (struct trang t);
void trangZag(struct trang (*t));
uint8_t randoms(uint8_t lower, uint8_t upper);
void awakenTrang(uint8_t spawn);

void sqwogNextPos(struct sqwog (*t));
void drawSqwog (struct sqwog t);
void eraseSqwog (struct sqwog t);
void sqwogBox (struct sqwog (*t));
void awakenSqwog(uint8_t spawn);

#endif
