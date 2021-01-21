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


volatile uint32_t timeFlagPrint;
volatile uint32_t timeFlagA1;
volatile uint32_t timeFlagBullet;
volatile uint32_t timeFlagA2;
volatile uint32_t timeFlagEnemy;
volatile uint32_t timeFlagGame;
volatile uint32_t toneFlag;

struct timing_t {
    volatile uint32_t mikroSec, milliSec, centiSec, second, minute;
};

volatile struct timing_t timetime;



void lcd_write_string (char text[], uint16_t slice, uint8_t row, uint8_t (*buff)[512]);
void setLed(uint8_t color);
void setFreq(uint16_t freq);
void TIM1_BRK_TIM15_IRQHandler();



#endif
