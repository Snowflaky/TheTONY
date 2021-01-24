#include "inout.h"
#include "string.h"

/******************************/
/*** Input/Output functions ***/
/******************************/


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

//Initiates GPIO pin A, B and C
void initGPIO(){
    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;
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

//Detect if keyboard input is w (up), s (down), b (boss) or p (shoot, phew!)
uint8_t keyInput(){
//Any other key --> return 0.
    uint8_t x=0;
    uint8_t input=uart_get_char();
    if (input=='w'){
        x=1;
    }
    else if (input=='s'){
        x=2;
    }
    else if (input=='b'){   //boss-button
        x=3;
    }
    else if (input=='p'){  //phew! gun trigger
        x=4;
    }
    else if (input=='1'){
        x=5;
    }
    else if (input=='2'){
        x=6;
    }
    else if (input=='3'){
        x=7;
    }
    else if (input=='0'){
        x=8;
    }
    return x;
}

void resetHS() {
    FLASH_Unlock();
    FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
    FLASH_ErasePage(0x0800F800);//erase page
    FLASH_ProgramHalfWord(0x0800F800,0);//write data
    FLASH_Lock();//lock flash
}
