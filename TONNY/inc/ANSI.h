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

struct vector_t {
    uint32_t x, y;
};

struct trang { //One of the baddest bois around!, he's got two things inside him,- burning hatred for the player, and 180 degrees
    struct vector_t position;
    struct vector_t velocity;
    uint8_t hp;
};

struct sqwog { //One of the baddest bois around!, he's got two things inside him,- burning hatred for the player, and 180 degrees
    struct vector_t position;
    struct vector_t velocity;
    uint8_t hp;
};

struct ship {
    struct position;
    uint32_t hp;
};

void hello();
void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgcolor();
void clrscr ();
void gotoxy (uint32_t c, uint32_t r);
void window(uint8_t x11, uint8_t y11, uint8_t x22, uint8_t y22);

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
