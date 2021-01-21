#include "30010_io.h"
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <charset.h>
#include <time.h>
#include "ANSI.h"
#include "game.h"


#ifndef _print_H_
#define _print_H_
#define ESC 0x1B


void window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t lineColor);
void printShip (struct ship_t ship, struct ship_t oldShip);
void printAsteroid (struct asteroid_t asteroid, struct asteroid_t oldAsteroid);
void printBullet (struct bullet_t bullet, struct bullet_t oldBullet);
void printDodge (struct asteroid_t dodge, struct asteroid_t oldDodge);
void drawEnemy (struct enemy e);
void eraseEnemy (struct enemy e);
void boss ();



#endif
