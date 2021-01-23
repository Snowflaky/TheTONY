#include "timer.h"

/****************************/
/***** Timer Functions ******/
/****************************/

//Initiate timer 15:
void initTimer15(){
    RCC->APB2ENR |= RCC_APB2Periph_TIM15; //enable clock line to timer 15?
    TIM15->CR1 = 0x0000;
    TIM15->ARR = 0x003F; //set reload value to 63 (microseconds)
    TIM15->PSC = 0x0000; //set prescale to 0 (microseconds)
    TIM15->DIER |= 0x0001; //enable timer interrupt
    NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0);
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
    TIM15->CR1 |= 0x0001; //enable timer
}

//Initiate timer 2:
void initTimer2(){
    RCC->APB1ENR |= 0x00000001; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000; // Disable timer
    TIM2->ARR = 1000; // Set auto reload value
    TIM2->PSC = 0x0C7F;//PRESCALER_VALUE; // Set prescale value
        //0x003F = millieconds
        //0x18FF = centiseconds
        //0x0C7F
    TIM2->CR1 |= 0x0001; // Enable timer
        //configure counter compare register:
    TIM2->CCER &= ~TIM_CCER_CC3P; // Clear CCER register
    TIM2->CCER |= 0x00000001 << 8; // Enable OC3 output
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear CCMR2 register
    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
    TIM2->CCMR2 |= TIM_OCMode_PWM1; // Set output mode to PWM1
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3PE;
    TIM2->CCMR2 |= TIM_OCPreload_Enable;
    TIM2->CCR3 = 500; // Set duty cycle to 50 %
        //connect pin PB10 to timer (for alternate function):
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Enable clock line for GPIO bank B
    GPIOB->MODER &= ~(0x00000003 << (10 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000002 << (10 * 2)); // Set mode register
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);
}

void TIM1_BRK_TIM15_IRQHandler() {
//Counts 100ths of a second, seconds and minutes.
    timeFlagPrint=1;
    timetime.mikroSec++;
    if (timetime.mikroSec>=1000){//milliseconds
        timetime.milliSec++;
        timetime.mikroSec=0;
        if (timetime.milliSec>=10) {//centiseconds
            timeFlagBullet++;
            timeFlagA1++;
            timeFlagA2++;
            timeFlagEnemy++;
            toneFlag++;
            timetime.centiSec++;
            timetime.milliSec=0;
            if (timetime.centiSec>=50){//seconds
                timetime.second++;
                timetime.centiSec=0;
                if (timetime.second%10==0){
                    timeFlagGame++;
                }
                if (timetime.second>=60){//minutes
                    timetime.minute++;
                    timetime.second=0;
                }
            }
        }
    }
    TIM15->SR &= ~0x0001; // Clear interrupt bit
 }

//Changes frequency of buzzer sound
void setFreq(uint16_t freq){
    uint32_t reload = 64e6 / freq / (0x01FF + 1) - 1;
    TIM2->ARR = reload; // Set auto reload value
    TIM2->CCR3 = reload/2; // Set compare register
    TIM2->EGR |= 0x01;
}
