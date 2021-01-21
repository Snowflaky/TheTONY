#include "inout.h"
#include "string.h"




//Puts a string into buffer array, for writing on LCD display
void lcd_write_string (char text[], uint16_t slice, uint8_t row, uint8_t (*buff)[512]){
//Specifies writing location on LCD display by slice and row. (4 rows, 128 slices)
//each character needs 5 slices
//Input: string, slice (0-127), row (1-4), pointer to buffer array.
    uint8_t h;
    uint8_t g;
    if (row==2){
        slice+=128;
    }
    if (row==3){
        slice+=256;
    }
    if (row==4){
        slice+=384;
    }
    for (h=0;h<strlen(text);h++) {  //for each letter in string
        for (g=0;g<5;g++){  //for each slice in letter
            (*buff)[slice+g+6*h]=character_data[text[h]-0x20][g];
        }
   }
}

//Sets LED diode to a specific color
void setLed(uint8_t color) {
//0=reset, 1=red(PB4), 2=green(PC7), 3=blue(PA9), 4=yellow(PB4+PC7)
    if (color==0){
            //resets red LED
        GPIOB->OSPEEDR |= (0x00000003 << (4 * 2));
        GPIOB->OSPEEDR &= ~(0x00000002 << (4 * 2));
        GPIOB->OTYPER |= (0x0001 << (4));
        GPIOB->OTYPER &= ~(0x0000 << (4));
        GPIOB->MODER |= (0x00000003 << (4 * 2));
        GPIOB->MODER &= ~(0x00000001 << (4 * 2));
        GPIOB->ODR &= ~(0x0000 << 4);
            //resets green LED
        GPIOC->OSPEEDR |= (0x00000003 << (7 * 2));
        GPIOC->OSPEEDR &= ~(0x00000002 << (7 * 2));
        GPIOC->OTYPER |= (0x0001 << (7));
        GPIOC->OTYPER &= ~(0x0000 << (7));
        GPIOC->MODER |= (0x00000003 << (7 * 2));
        GPIOC->MODER &= ~(0x00000001 << (7 * 2));
        GPIOC->ODR &= ~(0x0000 << 7);
            //resets blue LED
        GPIOA->OSPEEDR |= (0x00000003 << (9 * 2));
        GPIOA->OSPEEDR &= ~(0x00000002 << (9 * 2));
        GPIOA->OTYPER |= (0x0001 << (9));
        GPIOA->OTYPER &= ~(0x0000 << (9));
        GPIOA->MODER |= (0x00000003 << (9 * 2));
        GPIOA->MODER &= ~(0x00000001 << (9 * 2));
        GPIOA->ODR &= ~(0x0000 << 9);
    }
    if (color==1) {//red
        GPIOB->OSPEEDR &= ~(0x00000003 << (4 * 2));
        GPIOB->OSPEEDR |= (0x00000002 << (4 * 2));
        GPIOB->OTYPER &= ~(0x0001 << (4));
        GPIOB->OTYPER |= (0x0000 << (4));
        GPIOB->MODER &= ~(0x00000003 << (4 * 2));
        GPIOB->MODER |= (0x00000001 << (4 * 2));
        GPIOB->ODR |= (0x0000 << 4); //set pin to low
    }
    if (color==2) {//green
        GPIOC->OSPEEDR &= ~(0x00000003 << (7 * 2));
        GPIOC->OSPEEDR |= (0x00000002 << (7 * 2));
        GPIOC->OTYPER &= ~(0x0001 << (7));
        GPIOC->OTYPER |= (0x0000 << (7));
        GPIOC->MODER &= ~(0x00000003 << (7 * 2));
        GPIOC->MODER |= (0x00000001 << (7 * 2));
        GPIOC->ODR |= (0x0000 << 7); //set pin to low
    }
    if (color==3) {//blue
        GPIOA->OSPEEDR &= ~(0x00000003 << (9 * 2));
        GPIOA->OSPEEDR |= (0x00000002 << (9 * 2));
        GPIOA->OTYPER &= ~(0x0001 << (9));
        GPIOA->OTYPER |= (0x0000 << (9));
        GPIOA->MODER &= ~(0x00000003 << (9 * 2));
        GPIOA->MODER |= (0x00000001 << (9 * 2));
        GPIOA->ODR |= (0x0000 << 9); //set pin to low

    }
    if (color==4){//yellow
        GPIOB->OSPEEDR &= ~(0x00000003 << (4 * 2));
        GPIOB->OSPEEDR |= (0x00000002 << (4 * 2));
        GPIOB->OTYPER &= ~(0x0001 << (4));
        GPIOB->OTYPER |= (0x0000 << (4));
        GPIOB->MODER &= ~(0x00000003 << (4 * 2));
        GPIOB->MODER |= (0x00000001 << (4 * 2));
        GPIOB->ODR |= (0x0000 << 4);

        GPIOC->OSPEEDR &= ~(0x00000003 << (7 * 2));
        GPIOC->OSPEEDR |= (0x00000002 << (7 * 2));
        GPIOC->OTYPER &= ~(0x0001 << (7));
        GPIOC->OTYPER |= (0x0000 << (7));
        GPIOC->MODER &= ~(0x00000003 << (7 * 2));
        GPIOC->MODER |= (0x00000001 << (7 * 2));
        GPIOC->ODR |= (0x0000 << 7);
    }
}

//Changes frequency of buzzer sound
void setFreq(uint16_t freq){
    uint32_t reload = 64e6 / freq / (0x01FF + 1) - 1;
    TIM2->ARR = reload; // Set auto reload value
    TIM2->CCR3 = reload/2; // Set compare register
    TIM2->EGR |= 0x01;
}

void TIM1_BRK_TIM15_IRQHandler() {
//Counts 100ths of a second, seconds and minutes.
    timeFlagPrint=1;
    timetime.mikroSec++;
    if (timetime.mikroSec>=1000){
        timetime.milliSec++;
        timetime.mikroSec=0;
        if (timetime.milliSec>=10) {
            timeFlagBullet++;
            timeFlagA1++;
            timeFlagA2++;
            timeFlagEnemy++;
            toneFlag++;
            timetime.centiSec++;
            timetime.milliSec=0;
            if (timetime.centiSec>=50){
                timetime.second++;
                timetime.centiSec=0;
                if (timetime.second%10==0){
                    timeFlagGame++;
                }
                if (timetime.second>=60){
                    timetime.minute++;
                    timetime.second=0;
                }
            }

        }
    }
    TIM15->SR &= ~0x0001; // Clear interrupt bit
 }
