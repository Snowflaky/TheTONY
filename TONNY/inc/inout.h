#include "30010_io.h"
#include "stm32f30x_conf.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <charset.h>
#include <time.h>
#include "ANSI.h"

#ifndef _inout_H_
#define _inout_H_


void lcd_write_string (char text[], uint16_t slice, uint8_t row, uint8_t (*buff)[512]);
void initGPIO();
void setLed(uint8_t color);
uint8_t keyInput();
void resetHS();


#endif
