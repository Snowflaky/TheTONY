#include <charset.h>
#include <time.h>
#include "stm32f30x_conf.h"

#ifndef _timer_H_
#define _timer_H_

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

void initTimer15();
void initTimer2();
void TIM1_BRK_TIM15_IRQHandler();
void setFreq(uint16_t freq);


#endif
