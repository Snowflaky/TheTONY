#include "30010_io.h"
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <charset.h>
#include <time.h>
#include "ANSI.h"

#ifndef _game_H_
#define _game_H_


struct enemy {
    uint8_t enemyType;
    struct vector_t position;
    struct vector_t velocity;
    int32_t firsty;
    int32_t firstx;
};

struct ship_t {
    struct vector_t position;
};

struct asteroid_t {
    struct vector_t position;
    struct vector_t velocity;
    uint8_t ADFlag1;
    uint8_t ADFlag2;
    uint8_t startLevel;
};

struct bullet_t {
    struct vector_t position;
    struct vector_t velocity;
};


uint8_t compBuAs(struct bullet_t bullet, struct asteroid_t asteroid);
uint8_t compBuEn(struct bullet_t bullet, struct enemy e);
uint8_t compDoSh(struct ship_t ship, struct asteroid_t dodge);
uint16_t updateHighscore (uint16_t highscore, uint16_t score);
uint8_t enemyBreach(struct enemy e);


#endif
