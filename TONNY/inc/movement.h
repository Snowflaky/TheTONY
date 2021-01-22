#include "30010_io.h"
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <charset.h>
#include <time.h>
#include "ANSI.h"
#include "game.h"

#ifndef _movement_H_
#define _movement_H_



void moveShip (uint8_t x, struct ship_t *ship, struct ship_t *oldShip);
void moveAsteroid (uint8_t x, struct asteroid_t *asteroid, struct asteroid_t *oldAsteroid);
uint8_t startBullet (struct ship_t ship, uint8_t p);
void moveBullet (uint8_t y, struct bullet_t *bullet, struct bullet_t *oldBullet);
void moveDodge (uint8_t y, struct asteroid_t *dodge, struct asteroid_t *oldDodge);
void movePower (struct bullet_t *power, struct bullet_t *oldPower);
void enemyMotion (struct enemy (*e));
//void decideVel (struct enemy (*e));
void enemyNextPos (struct enemy *e);


#endif
