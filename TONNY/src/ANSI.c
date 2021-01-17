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

void moveShip (uint8_t x, struct ship_t *ship, struct ship_t *oldShip){
//If up/down keyInput (x) detected, function updates spaceship position.
//Input: keyInput (up/down/null), pointer to ship structure.
    (*oldShip).position.x=(*ship).position.x;
    (*oldShip).position.y=(*ship).position.y;
    if (x==1 && (*ship).position.y>3){
        (*ship).position.y--;
    }
    if (x==2 && (*ship).position.y<38){
        (*ship).position.y++;
    }
}

void printShip (struct ship_t ship, struct ship_t oldShip) {
//Delete ship at old position and prints spaceship at position (x,y).
//Input: pointer to ship structure.
    gotoxy(oldShip.position.x,oldShip.position.y);
    printf(" ");
    gotoxy(ship.position.x,ship.position.y);
    printf("o");
}

void moveAsteroid (uint8_t x, struct asteroid_t *asteroid, struct asteroid_t *oldAsteroid) {
//Moves asteroid 1 downwards (along y-axis).
//Input: x-axis position, pointer to asteroid structure.
    if ((*asteroid).position.y>=39){
        gotoxy((*asteroid).position.x,(*asteroid).position.y);
        printf(" ",ESC);
        (*asteroid).position.y=2;
    }
    (*oldAsteroid).position.x=(*asteroid).position.x;
    (*oldAsteroid).position.y=(*asteroid).position.y;
    uint8_t k=1;
    (*asteroid).position.x=x;
    (*asteroid).position.y=(*asteroid).position.y+(*asteroid).velocity.y*k;
}

void printAsteroid (struct asteroid_t asteroid, struct asteroid_t oldAsteroid){
//Delete asteroid at old position and prints asteroid at position (x,y).
//Input: pointer to asteroid structure.
    //if (asteroid.position.y<40){
        gotoxy(oldAsteroid.position.x,oldAsteroid.position.y);
        printf(" ",ESC);
        /*printf("%c[1D",ESC);
        printf(" ");*/
        gotoxy(asteroid.position.x,asteroid.position.y);
        bgcolor(3);
        printf(" ");
        bgcolor(0);
    //}
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

uint8_t startBullet (struct ship_t ship, uint8_t p){
    uint8_t k = 0;
    if (p==4){
        k=ship.position.y;//(*bullet).position.x++;// = (*bullet).position.x++// (*bullet).velocity.x*k;
        //(*bullet).position.y = (*bullet).position.y;
    }
    return k;
}

void moveBullet (uint8_t y, struct bullet_t *bullet, struct bullet_t *oldBullet) {
//Moves asteroid 1 downwards (along y-axis).
//Input: x-axis position, pointer to asteroid structure.
    if ((*bullet).position.x>=139){
        (*bullet).position.x=3;
    }
    (*oldBullet).position.x=(*bullet).position.x;
    (*oldBullet).position.y=(*bullet).position.y;
    uint8_t k=1;
    (*bullet).position.x=(*bullet).position.x + (*bullet).velocity.x*k;
    (*bullet).position.y=y;
}

void printBullet (struct bullet_t bullet, struct bullet_t oldBullet) {
    if(bullet.position.x>3){
        gotoxy(oldBullet.position.x,oldBullet.position.y);
        printf(" ");
        if (bullet.position.x<139){
            gotoxy(bullet.position.x,bullet.position.y);
            printf("Q");
        }
    }
}

void TIM2_IRQHandler() {
//Counts 100ths of a second, seconds and minutes.
    timeFlagPrint=1;
    timeFlagDrawT++;
    time.mikroSec++;
    if (time.mikroSec>=1000){
        time.milliSec++;
        time.mikroSec=0;
        if (time.milliSec>=10) {
            timeFlagBullet++;
            timeFlagA1++;
            timeFlagA2++;
            timeFlagTra++;
            time.centiSec++;
            time.milliSec=0;
            if (time.centiSec>=500){
                timeFlagScore++;
            }
            if (time.centiSec>=50){
                time.second++;
                time.centiSec=0;
                if (time.second%10==0){
                    timeFlagScore++;
                }
                if (time.second>=60){
                    time.minute++;
                    time.second=0;
                }
            }

        }
    }
    TIM2->SR &= ~0x0001; // Clear interrupt bit
 }



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

void fixtrangPos(struct trang (*t)) {
//Converts Trangs position and velocity to 18.14 (from 32.0)
    (*t).position.x=(*t).position.x<<14;
    (*t).position.y=(*t).position.y<<14;
    (*t).velocity.x=(*t).velocity.x<<14;
    (*t).velocity.y=(*t).velocity.y<<14;
}

//The next few functions are dedicated to the harbringer of
//Cosine DOOM, TRANG!!!
void trangNextPos(struct trang (*t)) {
//Calculate new position for Trang. Input is a pointer.
    uint32_t k = 1;
    if ((*t).position.x<5){
        (*t).position.x=135;
    }
    (*t).position.x = (*t).position.x + (*t).velocity.x*k;
    (*t).position.y = (*t).position.y + (*t).velocity.y*k;
}

uint8_t enemyBreach(struct enemy e){
    uint8_t breach=0;
    if (e.position.x<6){
        breach=1;
    }
    return breach;
}

void drawTrang (struct trang t) {
    color(0,0);
    //print the Trang alien ship
    gotoxy(t.position.x,t.position.y);
    color(6,5);
    gotoxy(t.position.x,t.position.y);
    printf("<");
    gotoxy(t.position.x + 1,t.position.y - 1);
    printf("-");
    gotoxy(t.position.x + 1,t.position.y + 1);
    printf("-");
    //print the Trang alien ships (ineffective) shields
    color(2,0);
    gotoxy(t.position.x - 2,t.position.y);
    printf("<");
    gotoxy(t.position.x - 1,t.position.y - 1);
    printf("/");
    gotoxy(t.position.x - 1,t.position.y + 1);
    printf("\\");
    gotoxy(t.position.x,t.position.y - 2);
    printf("/");
    gotoxy(t.position.x,t.position.y + 2);
    printf("\\");
    color(15,0);
}

void eraseTrang (struct trang t) { //erases Trang with the same
    //color(0,0);                    //method as Trang is drawn
    //erase the Trang alien ship
    gotoxy(t.position.x,t.position.y);
    color(0,0);
    gotoxy(t.position.x,t.position.y);
    printf(" ");
    gotoxy(t.position.x + 1,t.position.y - 1);
    printf(" ");
    gotoxy(t.position.x + 1,t.position.y + 1);
    printf(" ");
    //erase the shields of the Trang alien ship
    color(0,0);
    gotoxy(t.position.x - 2,t.position.y);
    printf(" ");
    gotoxy(t.position.x - 1,t.position.y - 1);
    printf(" ");
    gotoxy(t.position.x - 1,t.position.y + 1);
    printf(" ");
    gotoxy(t.position.x,t.position.y - 2);
    printf(" ");
    gotoxy(t.position.x,t.position.y + 2);
    printf(" ");
    color(15,0);
}

void trangZag (struct trang (*t)) { //Moves Trang in a zig zag
    uint8_t firsty = (*t).firsty;   // motion
    if ((*t).position.y - firsty > 4 || (*t).position.y - firsty < -4) {
        (*t).velocity.y *= -1; //If the current y position is
    }                          //farther from starting position
}                              //than 3, y velocity is reversed

// generate random numbers in range [lower, upper].
uint8_t randoms(uint8_t lower, uint8_t upper) {
    uint8_t num=0;
    num = (rand() % (upper - lower + 1)) + lower;
}

void awakenTrang(uint8_t spawn) {    //Bring Trang, the bringer
    if (spawn == 1) {                //of doom, to LIFE
        struct trang tra;
        tra.position.x = 110;
        tra.position.y = randoms(4,36);
        tra.velocity.x = -2;
        tra.velocity.y = -2;
        tra.hp = 5;

        for(int i = 0; i < 20; i++) {
            trangNextPos(&tra);
            trangZag(&tra);
            drawTrang(tra);
            eraseTrang(tra);
        }
    }
}

//The following functions are dedicated to the dishonoured warrior
//of the Alik'r homeworlds; the one the call  GENERAL SQWOG
void sqwogNextPos(struct sqwog (*t)) {
//Calculate new position for Trang. Input is a pointer.
    uint8_t k = 1;
    (*t).position.x = (*t).position.x + (*t).velocity.x*k;
    (*t).position.y = (*t).position.y + (*t).velocity.y*k;
}

void drawSqwog (struct sqwog t) {
    color(0,0);
    //print the Sqwog alien ship
    gotoxy(t.position.x,t.position.y);
    color(11,3);
    gotoxy(t.position.x,t.position.y);
    printf("O");
    gotoxy(t.position.x,t.position.y - 1);
    printf("|");
    gotoxy(t.position.x,t.position.y + 1);
    printf("|");
    gotoxy(t.position.x - 1,t.position.y);
    printf("-");
    gotoxy(t.position.x + 1,t.position.y);
    printf("-");
    //print the Sqwog alien ships (also ineffective) shields
    color(9,0);
    gotoxy(t.position.x - 2,t.position.y);
    printf("<");
    gotoxy(t.position.x - 1,t.position.y - 1);
    printf("/");
    gotoxy(t.position.x - 1,t.position.y + 1);
    printf("\\");
    gotoxy(t.position.x,t.position.y - 2);
    printf("^");
    gotoxy(t.position.x,t.position.y + 2);
    printf("v");
    gotoxy(t.position.x + 1,t.position.y - 1);
    printf("\\");
    gotoxy(t.position.x + 1,t.position.y + 1);
    printf("/");
    gotoxy(t.position.x + 2,t.position.y);
    printf(">");
}

void eraseSqwog (struct sqwog t) { //erases Sqwog with the same
    color(0,0);                    //method as Sqwog is drawn
    //erase the Sqwog alien ship
    gotoxy(t.position.x,t.position.y);
    gotoxy(t.position.x,t.position.y);
    printf(" ");
    gotoxy(t.position.x,t.position.y - 1);
    printf(" ");
    gotoxy(t.position.x,t.position.y + 1);
    printf(" ");
    gotoxy(t.position.x - 1,t.position.y);
    printf(" ");
    gotoxy(t.position.x + 1,t.position.y);
    printf(" ");
    //erase the shields of the Sqwog alien ship
    color(0,0);
    gotoxy(t.position.x - 2,t.position.y);
    printf(" ");
    gotoxy(t.position.x - 1,t.position.y - 1);
    printf(" ");
    gotoxy(t.position.x - 1,t.position.y + 1);
    printf(" ");
    gotoxy(t.position.x,t.position.y - 2);
    printf(" ");
    gotoxy(t.position.x,t.position.y + 2);
    printf(" ");
    gotoxy(t.position.x + 1,t.position.y - 1);
    printf(" ");
    gotoxy(t.position.x + 1,t.position.y + 1);
    printf(" ");
    gotoxy(t.position.x + 2,t.position.y);
    printf(" ");
    color(15,0);
}

void sqwogBox (struct sqwog (*t)) { //Moves Sqwog in a square
    if (((*t).position.x - (*t).firstx == -5) && ((*t).position.y - (*t).firsty == 0)) {
        (*t).velocity.y = -1;
        (*t).velocity.x = 0;
    }  else if ((*t).position.y - (*t).firsty == -5 && (*t).position.x - (*t).firstx == -5) {
        (*t).velocity.y = 0;
        (*t).velocity.x = -1;
    } else if ((*t).position.x - (*t).firstx == -10 && (*t).position.y - (*t).firsty == -5) {
        (*t).velocity.y = 1;
        (*t).velocity.x = 0;
    } else if ((*t).position.y - (*t).firsty == 0 && (*t).position.x - (*t).firstx == -10) {
        (*t).velocity.y = 0;
        (*t).velocity.x = -1;
    }
}

void awakenSqwog(uint8_t spawn) {    //Bring Sqwog, the bringer
    if (spawn == 1) {                //of inconvenience, to LIFE
        struct sqwog skr;
        skr.position.x = 110;
        skr.position.y = randoms(4,35);
        skr.velocity.x = -2;
        skr.hp = 5;

        for(int i = 0; i < 20; i++) {
            sqwogNextPos(&skr);
            sqwogBox(&skr);
            drawSqwog(skr);
            eraseSqwog(skr);
        }
    }
}

uint8_t compBuAs(struct bullet_t bullet, struct asteroid_t asteroid){
    uint8_t g=0;
    if (bullet.position.x==asteroid.position.x-1 && bullet.position.y==asteroid.position.y){
        g=1;
    }

    return g;
}

uint8_t compBuEn(struct bullet_t bullet, struct enemy e){
    uint8_t g=0;
    if (bullet.position.x==e.position.x && (bullet.position.y==e.position.y ||
                                              bullet.position.y==e.position.y+1 ||
                                              bullet.position.y==e.position.y+2 ||
                                              bullet.position.y==e.position.y-1 ||
                                              bullet.position.y==e.position.y-2)){
        g=1;
    }
    return g;
}

// The following functions are a generalised form of the
//aforementioned enemy generation functions

void enemyNextPos (struct enemy *e) {
    //Calculate new position for Trang. Input is a pointer.
    uint32_t k = 1;
    (*e).position.x = (*e).position.x + (*e).velocity.x*k;
    (*e).position.y = (*e).position.y + (*e).velocity.y*k;
}

void drawEnemy (struct enemy e) {
    if (e.enemyType == 1) {
        color(0,0);
        //print the Trang alien ship
        gotoxy(e.position.x,e.position.y);
        color(6,5);
        gotoxy(e.position.x,e.position.y);
        printf("<");
        gotoxy(e.position.x + 1,e.position.y - 1);
        printf("-");
        gotoxy(e.position.x + 1,e.position.y + 1);
        printf("-");
        //print the Trang alien ships (ineffective) shields
        color(2,0);
        gotoxy(e.position.x - 2,e.position.y);
        printf("<");
        gotoxy(e.position.x - 1,e.position.y - 1);
        printf("/");
        gotoxy(e.position.x - 1,e.position.y + 1);
        printf("\\");
        gotoxy(e.position.x,e.position.y - 2);
        printf("/");
        gotoxy(e.position.x,e.position.y + 2);
        printf("\\");
        color(15,0);
    } else if (e.enemyType == 2) {
        color(0,0);
        //print the Sqwog alien ship
        gotoxy(e.position.x,e.position.y);
        color(11,2);
        gotoxy(e.position.x,e.position.y);
        printf("O");
        gotoxy(e.position.x,e.position.y - 1);
        printf("|");
        gotoxy(e.position.x,e.position.y + 1);
        printf("|");
        gotoxy(e.position.x - 1,e.position.y);
        printf("-");
        gotoxy(e.position.x + 1,e.position.y);
        printf("-");
        //print the Sqwog alien ships (also ineffective) shields
        color(9,0);
        gotoxy(e.position.x - 2,e.position.y);
        printf("<");
        gotoxy(e.position.x - 1,e.position.y - 1);
        printf("/");
        gotoxy(e.position.x - 1,e.position.y + 1);
        printf("\\");
        gotoxy(e.position.x,e.position.y - 2);
        printf("^");
        gotoxy(e.position.x,e.position.y + 2);
        printf("v");
        gotoxy(e.position.x + 1,e.position.y - 1);
        printf("\\");
        gotoxy(e.position.x + 1,e.position.y + 1);
        printf("/");
        gotoxy(e.position.x + 2,e.position.y);
        printf(">");
    }
    color(15,0);
}

void eraseEnemy (struct enemy e) {
    if (e.enemyType == 1) {
        //color(0,0);
        //erase the Trang alien ship
        gotoxy(e.position.x,e.position.y);
        color(0,0);
        gotoxy(e.position.x,e.position.y);
        printf(" ");
        gotoxy(e.position.x + 1,e.position.y - 1);
        printf(" ");
        gotoxy(e.position.x + 1,e.position.y + 1);
        printf(" ");
        //erase the shields of the Trang alien ship
        color(0,0);
        gotoxy(e.position.x - 2,e.position.y);
        printf(" ");
        gotoxy(e.position.x - 1,e.position.y - 1);
        printf(" ");
        gotoxy(e.position.x - 1,e.position.y + 1);
        printf(" ");
        gotoxy(e.position.x,e.position.y - 2);
        printf(" ");
        gotoxy(e.position.x,e.position.y + 2);
        printf(" ");
        color(15,0);
    } else if (e.enemyType == 2) {
        color(0,0);                    //method as Sqwog is drawn
        //erase the Sqwog alien ship
        gotoxy(e.position.x,e.position.y);
        gotoxy(e.position.x,e.position.y);
        printf(" ");
        gotoxy(e.position.x,e.position.y - 1);
        printf(" ");
        gotoxy(e.position.x,e.position.y + 1);
        printf(" ");
        gotoxy(e.position.x - 1,e.position.y);
        printf(" ");
        gotoxy(e.position.x + 1,e.position.y);
        printf(" ");
        //erase the shields of the Sqwog alien ship
        color(0,0);
        gotoxy(e.position.x - 2,e.position.y);
        printf(" ");
        gotoxy(e.position.x - 1,e.position.y - 1);
        printf(" ");
        gotoxy(e.position.x - 1,e.position.y + 1);
        printf(" ");
        gotoxy(e.position.x,e.position.y - 2);
        printf(" ");
        gotoxy(e.position.x,e.position.y + 2);
        printf(" ");
        gotoxy(e.position.x + 1,e.position.y - 1);
        printf(" ");
        gotoxy(e.position.x + 1,e.position.y + 1);
        printf(" ");
        gotoxy(e.position.x + 2,e.position.y);
        printf(" ");
        color(15,0);
    }
}

void enemyMotion (struct enemy *e) {
    uint8_t k=1;
    if ((*e).position.x<5){
        (*e).position.x=135;
    }
    if ((*e).enemyType == 1) {
        uint8_t firsty = (*e).firsty;   // motion
        if ((*e).position.y - firsty > 4 || (*e).position.y - firsty < -4) {
            (*e).velocity.y *= -1; //If the current y position is
        }                          //farther from starting position
    }                             //than 3, y velocity is reversed
    else if ((*e).enemyType == 2) {
        if (((*e).position.x - (*e).firstx == -5) && ((*e).position.y - (*e).firsty == 0)) {
            (*e).velocity.y = -1;
            (*e).velocity.x = 0;
        }
        else if ((*e).position.y - (*e).firsty == -5 && (*e).position.x - (*e).firstx == -5) {
            (*e).velocity.y = 0;
            (*e).velocity.x = -1;
        }
        else if ((*e).position.x - (*e).firstx == -10 && (*e).position.y - (*e).firsty == -5) {
            (*e).velocity.y = 1;
            (*e).velocity.x = 0;
        }
        else if ((*e).position.y - (*e).firsty == 0 && (*e).position.x - (*e).firstx == -10) {
            (*e).velocity.y = 0;
            (*e).velocity.x = -1;
        }
    }
}


/*void menu () {
    window()
}*/
