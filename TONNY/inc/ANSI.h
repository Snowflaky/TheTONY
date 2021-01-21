#include "30010_io.h"
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <charset.h>
#include <time.h>

#ifndef _ANSI_H_
#define _ANSI_H_
#define ESC 0x1B
#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( ((a) << FIX14_SHIFT) / b )



struct vector_t {
    int32_t x, y;
};


/*struct trang { //One of the baddest bois around!, he's got two things inside him,- burning hatred for the player, and 180 degrees
    uint8_t random;
    struct vector_t position;
    struct vector_t velocity;
    int8_t firsty;
    int8_t firstx;
};

struct sqwog { //Originally a general in the Alik'r army who was dishonoured by a great military defeat, he wanders the stars looking for players to kill such that he may regain his honour.
    struct vector_t position;
    struct vector_t velocity;
    int8_t firsty;
    int8_t firstx;
};*/


void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgcolor();
void clrscr ();
void gotoxy (uint32_t c, uint32_t r);

uint8_t keyInput();
/*
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
*/

#endif
