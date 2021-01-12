#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "stdio.h"
#include "stdint.h"


#ifndef _ANSI_H_
#define _ANIS_H_
#define ESC 0x1B
#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( ((a) << FIX14_SHIFT) / b )



void hello();
void fgcolor(uint8_t foreground);
void bgcolor(uint8_t background);
void color(uint8_t foreground, uint8_t background);
void resetbgcolor();
void clrscr ();
void gotoxy (uint32_t c, uint32_t r);
void window();

#endif
