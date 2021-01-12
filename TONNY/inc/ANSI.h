#include "30010_io.h"
#include "stm32f30x_conf.h"
#include "stdio.h"
#include "stdint.h"


#ifndef _ansi_H_
#define _ansi_H_
#define ESC 0x1B
#define FIX14_SHIFT 14
#define FIX14_MULT(a, b) ( (a)*(b) >> FIX14_SHIFT )
#define FIX14_DIV(a, b) ( ((a) << FIX14_SHIFT) / b )
