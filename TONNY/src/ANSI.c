#include "ansi.h"
#include "string.h"

#define ESC 0x1B

//function for setting foreground color (letters and symbols)
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

//function for setting background color (can draw colored spaces)
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

//foreground and background set in one function
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

//Clears screen
void clrscr () {
    printf("%c[2J", ESC);
}

//Moves cursor to position (c,r).
void gotoxy (uint32_t c, uint32_t r) {
    printf("%c[%lu;%luH", ESC, r, c);
}

//Outline game borders. Border is drawn by printing colored background spaces.
void window(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t lineColor) {
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
    for (i = 0; i <= (x2-x1); i++) {
    //Lower border
        gotoxy((x1+i),y2);
        color(0,lineColor);
        printf(" ");
    }
    color(15,0);//resets background and foreground colors
    printf(" ");
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

//Moves spaceship
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

//Prints spaceship
void printShip (struct ship_t ship, struct ship_t oldShip) {
//Delete ship at old position and prints spaceship at position (x,y).
//Input: pointer to ship structure.
    color(0,0);
    gotoxy(oldShip.position.x + 1,oldShip.position.y);
    printf(" ");
    gotoxy(oldShip.position.x,oldShip.position.y - 1);
    printf(" ");
    gotoxy(oldShip.position.x,oldShip.position.y + 1);
    printf(" ");
    gotoxy(oldShip.position.x,oldShip.position.y);
    printf(" ");
    gotoxy(oldShip.position.x + 1,oldShip.position.y - 1);
    printf(" ");
    gotoxy(oldShip.position.x + 1,oldShip.position.y + 1);
    printf(" ");
//Draw Tonny's ship in it's current position
    color(0,1);
    gotoxy(ship.position.x + 1,ship.position.y);
    printf("O");
    color(9,1);
    gotoxy(ship.position.x,ship.position.y - 1);
    printf("D");
    gotoxy(ship.position.x,ship.position.y + 1);
    printf("D");
    color(10,1);
    gotoxy(ship.position.x,ship.position.y);
    printf("=");
    color(1,0);
    printf("%c[1m", ESC);
    gotoxy(ship.position.x + 1,ship.position.y - 1);
    printf("-");
    gotoxy(ship.position.x + 1,ship.position.y + 1);
    printf("-");
    printf("%c[22m", ESC);
    color(15,0);
}

//movement pattern of asteroids
void moveAsteroid (uint8_t x, struct asteroid_t *asteroid, struct asteroid_t *oldAsteroid) {
//Moves asteroid 1 downwards (along y-axis).
//Input: x-axis position, pointer to asteroid structure.
    if ((*asteroid).position.y>=39){
        gotoxy((*asteroid).position.x,(*asteroid).position.y);
        printf(" ");
        (*asteroid).position.y=2;
    }
    (*oldAsteroid).position.x=(*asteroid).position.x;
    (*oldAsteroid).position.y=(*asteroid).position.y;
    uint8_t k=1;
    (*asteroid).position.x=x;
    (*asteroid).position.y=(*asteroid).position.y+(*asteroid).velocity.y*k;
}

//Prints asteroids
void printAsteroid (struct asteroid_t asteroid, struct asteroid_t oldAsteroid){
//Delete asteroid at old position and prints asteroid at position (x,y).
//Input: pointer to asteroid structure.
    //if (asteroid.position.y<40){
        gotoxy(oldAsteroid.position.x,oldAsteroid.position.y);
        printf(" ");
        /*printf("%c[1D",ESC);
        printf(" ");*/
        gotoxy(asteroid.position.x,asteroid.position.y);
        bgcolor(3);
        printf(" ");
        bgcolor(0);
    //}
}

//Button to press when the boss walks by (function for ending game)
void boss (){
//Clears screen and shows a window stating "WORKING HARD!!!" (The boss will never know you were playing a computer game)
//Function is only initiated if boss key 'b' is pressed.
//Input: return from keyInput function.
        clrscr();
        window(60,90,20,30,1);
        gotoxy(67,25);
        printf("WORKING HARD!!!");
        gotoxy(67,27);
        printf("Press '0'");
}

//Initiates bullet
uint8_t startBullet (struct ship_t ship, uint8_t p){
//returns ship y-axis position
//input: ship, key-input
    uint8_t k = 0;
    if (p==4){//shoot key(p) is pressed, bullet starts at ship position
        k=ship.position.y;
    }
    return k;
}

//Moves bullet
void moveBullet (uint8_t y, struct bullet_t *bullet, struct bullet_t *oldBullet) {
//Moves asteroid 1 downwards (along y-axis).
//Input: x-axis position, point    (*dodge).position.y=y;er to asteroid structure.
    if ((*bullet).position.x>=139){//when bullet reaches the end of the field its position is reset
        (*bullet).position.x=3;
    }
    (*oldBullet).position.x=(*bullet).position.x;//updates old x-coordinate
    (*oldBullet).position.y=(*bullet).position.y;//updates old y-coordinate
    uint8_t k=1;
    (*bullet).position.x=(*bullet).position.x + (*bullet).velocity.x*k;//movement of bullet in x-direction
    (*bullet).position.y=y;//has the same constant height as the spaceship when bullet was fired(
}

//Prints bullet
void printBullet (struct bullet_t bullet, struct bullet_t oldBullet) {
    if(bullet.position.x>3){//if bullets has been initiated..
        gotoxy(oldBullet.position.x,oldBullet.position.y);//deletes old bullet print
        printf(" ");
        if (bullet.position.x<139){//only prints bullet as long as it is within the game field
            gotoxy(bullet.position.x,bullet.position.y);
            fgcolor(6);
            printf("%c[1m",ESC);
            printf("=");
            fgcolor(15);
            printf("%c[22m",ESC);
        }
    }
}

void TIM2_IRQHandler() {
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
    TIM2->SR &= ~0x0001; // Clear interrupt bit
 }

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

//Converts Trangs position and velocity to 18.14 (from 32.0)
void fixtrangPos(struct trang (*t)) {
    (*t).position.x=(*t).position.x<<14;
    (*t).position.y=(*t).position.y<<14;
    (*t).velocity.x=(*t).velocity.x<<14;
    (*t).velocity.y=(*t).velocity.y<<14;
}

//The next few functions are dedicated to the harbringer of
//Cosine DOOM, TRANG!!!
void trangNextPos(struct trang (*t)) {
//Calculates new position for Trang. Input is a pointer.
    uint32_t k = 1;
    if ((*t).position.x<5){
        (*t).position.x=135;
    }
    (*t).position.x = (*t).position.x + (*t).velocity.x*k;
    (*t).position.y = (*t).position.y + (*t).velocity.y*k;
}

//Detects if enemy has passed spaceship
uint8_t enemyBreach(struct enemy e){
//returns TRUE, if enemy passes TONNY
//input: enemy structure
    uint8_t breach=0;
    if (e.position.x<6){
        breach=1;
    }
    return breach;
}

//The Trang enemy ships
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

//Erases Trang
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

//Moves Trang in a zig zag motion
void trangZag (struct trang (*t)) {
    uint8_t firsty = (*t).firsty;
    if ((*t).position.y - firsty > 4 || (*t).position.y - firsty < -4) {
        (*t).velocity.y *= -1; //If the current y position is
    }                          //farther from starting position
}                              //than 3, y velocity is reversed

// generates random numbers in range [lower, upper].
uint8_t randoms(uint8_t lower, uint8_t upper) {
    time_t t;
    /* Intializes random number generator */
    srand((unsigned) time(&t));
    return(rand() % (upper + 1 - lower) + lower);
    //srand(time(&somesec));
    //int32_t num = (srand(time(0)) % (upper - lower + 1)) + lower;
    /*
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    printf ("Current local time and date: %s", asctime(timeinfo));
    */
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

//The Sqwog enemy ships
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

//Erases Sqwog
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

//Moves Sqwog in a square motion
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

//Compares asteroid and bullet positions
uint8_t compBuAs(struct bullet_t bullet, struct asteroid_t asteroid){
//if collision is detected, 1 is returned
    uint8_t g=0;
    if (bullet.position.x==asteroid.position.x-1 && bullet.position.y==asteroid.position.y){
        g=1;
    }

    return g;
}

//Compares enemy and bullet positions
uint8_t compBuEn(struct bullet_t bullet, struct enemy e){
    //if collision is detected, 1 is returned
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
        printf("%c[1m", ESC);
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
        printf("%c[22m", ESC);
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
        printf("%c[1m", ESC);
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
        printf("%c[22m", ESC);
        color(15,0);
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

void decideVel (struct enemy (*e)) {
    if ((*e).enemyType == 1) {
        (*e).velocity.x = -1;
        (*e).velocity.y = 1;
    } else if ((*e).enemyType == 2) {
        (*e).velocity.x = -1;
        (*e).velocity.y = 0;
    }
}

void enemyMotion (struct enemy (*e)) {
    uint8_t k=1;
    if ((*e).position.x<5){
        (*e).position.x=135;
    }
    if ((*e).enemyType == 1) {
        //uint32_t firsty = (*e).firsty;   // motion
        if (((*e).position.y - (*e).firsty) >= 4 || ((*e).position.y - (*e).firsty) <= -4) {
            (*e).velocity.y *= -1; //If the current y position is
        }               //farther from starting y position than 4,
    }                   // y velocity is reversed (Trang). This motion
    else if ((*e).enemyType == 2) { // pattern is specified by enemyType
        if ((((*e).position.x - (*e).firstx == -10) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -30) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -50) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -70) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -90) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -110) && ((*e).position.y - (*e).firsty == 0))) {
            (*e).velocity.y = -1; //This second if (else if) statement
            (*e).velocity.x = 0;  //specifies the motion of sqwog, and
        }          //also uses the enemyType variable to choose function
        else if (((*e).position.y - (*e).firsty == -5 && (*e).position.x - (*e).firstx == -10) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -30)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -50)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -70)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -90)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -110))) {
            (*e).velocity.y = 0;
            (*e).velocity.x = -1;
        }
        else if (((*e).position.x - (*e).firstx == -20 && (*e).position.y - (*e).firsty == -5) ||
            (((*e).position.x - (*e).firstx == -40) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -60) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -80) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -100) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -120) && ((*e).position.y - (*e).firsty == -5))) {
            (*e).velocity.y = 1;
            (*e).velocity.x = 0;
        }
        else if (((*e).position.y - (*e).firsty == 0 && (*e).position.x - (*e).firstx == -20) ||
            (((*e).position.y - (*e).firsty == 0) && ((*e).position.x - (*e).firstx == -40)) ||
            (((*e).position.y - (*e).firsty == 0) && ((*e).position.x - (*e).firstx == -60)) ||
            (((*e).position.y - (*e).firsty == 0) && ((*e).position.x - (*e).firstx == -80)) ||
            (((*e).position.y - (*e).firsty == 0) && ((*e).position.x - (*e).firstx == -100)) ||
            (((*e).position.y - (*e).firsty == 0) && ((*e).position.x - (*e).firstx == -120))) {
            (*e).velocity.y = 0;
            (*e).velocity.x = -1;
        }
    }
}

//Moves enemies nets
void moveDodge (uint8_t y, struct asteroid_t *dodge, struct asteroid_t *oldDodge) {
//Moves nets -1 along x-axis
//Input: y-axis position, pointer to dodge structure.
    if ((*dodge).position.x<=2){
        gotoxy((*dodge).position.x,(*dodge).position.y);
        printf(" ");
        (*dodge).position.x=139;
    }
    (*oldDodge).position.x=(*dodge).position.x;
    (*oldDodge).position.y=(*dodge).position.y;
    uint8_t k=1;
    (*dodge).position.x=(*dodge).position.x+(*dodge).velocity.x*k;
    (*dodge).position.y=y;
}

//Prints enemies nets
void printDodge (struct asteroid_t dodge, struct asteroid_t oldDodge){
//Delete nets at old position and prints nets at position (x,y).
//Input: pointer to dodge structure.
    //if (dodge.position.y<40)
        gotoxy(oldDodge.position.x,oldDodge.position.y);
        printf(" ");
        gotoxy(dodge.position.x,dodge.position.y);
        fgcolor(13);
        printf("%c[1m", ESC);
        printf("#");
        fgcolor(15);
        printf("%c[22m", ESC);
}
////Compares nets and ship positions
uint8_t compDoSh(struct ship_t ship, struct asteroid_t dodge){
    //if collision is detected, 1 is returned
    uint8_t g=0;
    if (ship.position.x==dodge.position.x-1 && (ship.position.y==dodge.position.y ||
                                                ship.position.y==dodge.position.y+1 ||
                                                ship.position.y==dodge.position.y-1 )){
        g=1;
    }
    return g;
}

//Checks if new score is higher than previous highscore and updates it
uint16_t updateHighscore (uint16_t highscore, uint16_t score){
//input:current highscore, current score
    if(score > highscore){
       highscore = score;
       }
    return highscore;
}

