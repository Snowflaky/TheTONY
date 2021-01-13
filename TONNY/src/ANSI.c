#include "ansi.h"

#define ESC 0x1B


void hello (){
    printf("Hello World!\n");
}

void fgcolor(uint8_t foreground) {
/*  Value      foreground     Value     foreground
    ------------------------------------------------
      0        Black            8       Dark Gray
      1        Red              9       Light Red
      2        Green           10       Light Green
      3        Brown           11       Yellow
      4        Blue            12       Light Blue
      5        Purple          13       Light Purple
      6        Cyan            14       Light Cyan
      7        Light Gray      15       White
*/
  uint8_t type = 22;             // normal text
	if (foreground > 7) {
	  type = 1;                // bold text
		foreground -= 8;
	}
  printf("%c[%d;%dm", ESC, type, foreground+30);
}

void bgcolor(uint8_t background) {
/* IMPORTANT:   When you first use this function you cannot get back to true white background in HyperTerminal.
   Why is that? Because ANSI does not support true white background (ANSI white is gray to most human eyes).
                The designers of HyperTerminal, however, preferred black text on white background, which is why
                the colors are initially like that, but when the background color is first changed there is no
 	              way comming back.
   Hint:        Use resetbgcolor(); clrscr(); to force HyperTerminal into gray text on black background.

    Value      Color
    ------------------
      0        Black
      1        Red
      2        Green
      3        Brown
      4        Blue
      5        Purple
      6        Cyan
      7        Gray
*/
    printf("%c[%dm", ESC, background+40);
}

void color(uint8_t foreground, uint8_t background) {
// combination of fgcolor() and bgcolor() - uses less bandwidth
    uint8_t type = 22;             // normal text
	if (foreground > 7) {
        type = 1;                // bold text
		foreground -= 8;
	}
    printf("%c[%d;%d;%dm", ESC, type, foreground+30, background+40);
}

void resetbgcolor() {
// gray on black text, no underline, no blink, no reverse
    printf("%c[m", ESC);
}

void clrscr () {
    printf("%c[2J", ESC);
}

void gotoxy (uint32_t c, uint32_t r) {
//Moves cursor to position (c,r).
    printf("%c[%lu;%luH", ESC, r, c);
}

void window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t lineColor) {
//Outline game borders. Border is drawn by printing colored background spaces.
//Input: x start point, x end point, y start point, y end point, color of border (number between 0 and 7 (see bgcolor))
    //Game borders
    /*
    uint8_t x1 = 0;
    uint8_t y1 = 0;
    uint8_t x2 = 140;
    uint8_t y2 = 40;*/
    uint8_t i;
    for (i = 0; i < (y2-y1); i++) {
    //Left border
        gotoxy(x1,y1+i);
        color(0,lineColor);
        printf(" ");
    }
    for (i = 0; i < (y2-y1); i++) {
    //Right border
        gotoxy(x2,y1+i);
        color(0,lineColor);
        printf(" ");
    }
    for (i = 0; i < ((x2-x1)+1); i++) {
    //Upper border
        gotoxy(x1+i,y1);
        color(0,lineColor);
        printf(" ");
    }
    for (i = 0; i < (x2-x1); i++) {
    //Lower border
        gotoxy((x1+i)+1,y2);
        color(0,lineColor);
        printf(" ");
    }
    color(15,0);//resets background and foreground colors
    printf(" ");
}

uint8_t keyInput(){
//Detect if keyboard input is w (up), s (down), b (boss) or p (shoot, phew!)
//Any other key --> return 0.
    uint8_t x=0;
    uint8_t input=uart_get_char();
    if (input=='w'){
        x=1;
    }
    else if (input=='s'){
        x=2;
    }
    else if (input=='b'){
        x=3;
    }
    else if (input=='p'){  //phew!
        x=4;
    }
    return x;
}

void moveShip (uint8_t x, struct ship_t *ship){
//If up/down keyInput (x) detected, function updates spaceship position.
//Input: keyInput (up/down/null), pointer to ship structure.
    if (x==1 && (*ship).position.y>2){
        (*ship).position.y--;
    }
    if (x==2 && (*ship).position.y<39){
        (*ship).position.y++;
    }
}

void printShip (struct ship_t ship) {
//Delete ship at old position and prints spaceship at position (x,y).
//Input: pointer to ship structure.
    printf("%c[1D",ESC);
    printf(" ");
    gotoxy(ship.position.x,ship.position.y);
    printf("o");
}

void moveAsteroid (uint8_t x, struct asteroid_t *asteroid) {
//Moves asteroid 1 downwards (along y-axis).
//Input: x-axis position, pointer to asteroid structure.
    (*asteroid).position.x=x;
    (*asteroid).position.y++;
}

void printAsteroid (struct asteroid_t asteroid){
//Delete asteroid at old position and prints asteroid at position (x,y).
//Input: pointer to asteroid structure.
    if (asteroid.position.y<40){
        printf("%c[1D",ESC);
        printf(" ");
        gotoxy(asteroid.position.x,asteroid.position.y);
        printf("l");//Insert asteroid graphic
    }

}

void boss (uint8_t x){
//Clears screen and shows a window stating "WORKING HARD!!!" (The boss will never know you were playing a computer game)
//Function is only initiated if boss key 'b' is pressed.
//Input: return from keyInput function.
    if (x==3){
        clrscr();
        window(60,90,20,30,1);
        gotoxy(63,25);
        printf("WORKING HARD!!!\n");
    }
}



void moveBullet (struct bullet_t *bullet, uint8_t y){
    //uint8_t k = 1;
    if (y==4){
        (*bullet).position.x++;// = (*bullet).position.x++// (*bullet).velocity.x*k;
        (*bullet).position.y = y;
    }
}

void printBullet (struct bullet_t bullet) {
    printf("%c[1D",ESC);
    printf(" ");
    gotoxy(bullet.position.x,bullet.position.y);
    printf("Q");
}

void TIM2_IRQHandler() {
//Counts 100ths of a second, seconds and minutes.
    time.centiSec++;
    timeFlag=1;
    if (time.centiSec>=100){
        time.second++;
        time.centiSec=0;
        if (time.second>=60){
            time.minute++;
            time.second=0;
        }
    }
    TIM2->SR &= ~0x0001; // Clear interrupt bit
 }

void countFlag(uint8_t *v){
    if (timeFlag==1){
        (*v)++;
    }
}




    /*uint32_t centiSec;
    uint32_t sec;
    centiSec++;
    timeFlag=1;
    if (centiSec>=100){
        sec++;
        secFlag=1;
        if (sec%3==0){
            asteroidFlag=1;
        }
    }
TIM2->SR &= ~0x0001; // Clear interrupt bit
}*/


/*void lcd_write_string (char text[], uint16_t slice, uint8_t row, uint8_t (*buff)[512]){
//Writes string on LCD display on location specified by slice and row.
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
}*/

/*void fixtrangPos(struct trang (*t)) {
//Converts balls position and velocity to 18.14 (from 32.0)
    (*t).x=(*t).x<<14;
    (*t).y=(*t).y<<14;
    (*t).vx=(*t).vx<<14;
    (*t).vy=(*t).vy<<14;
}

void trangNextPos(struct trang (*t)) {
//Calculate new position for trang enemy Input is a pointer.
    int32_t k = 1.3;
    (*t).x = (*t).x + (*t).vx*k;
    (*t).y = (*t).y + (*t).vy*k;
}

void drawTrang (struct trang t) {
    color(0,0);
    //print the dark spots on trang
    gotoxy(t.x>>14,t.y>>14);
    color(6,5);
    gotoxy(t.x>>14,t.y>>14);
    printf("-");
    gotoxy((t.x>>14 + 1),(t.y>>14 - 1));
    printf("-");
    gotoxy((t.x>>14 + 1),(t.y>>14 + 1));
    printf("-");
    //print the shields of trang
    color(2,2);
    gotoxy(t.x>>14 - 2,t.y>>14);
    printf(" ");
    gotoxy(t.x>>14 - 1,t.y>>14 - 1);
    printf(" ");
    gotoxy(t.x>>14 - 1,t.y>>14 + 1);
    printf(" ");
    gotoxy(t.x>>14,t.y>>14 - 2);
    printf(" ");
    gotoxy(t.x>>14,t.y>>14 + 2);
    printf(" ");
    gotoxy(t.x>>14 + 1,t.y>>14 - 3);
    printf(" ");
    gotoxy(t.x>>14 + 1,t.y>>14 + 3);
    printf(" ");
}
*/

