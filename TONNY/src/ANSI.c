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
    printf("%c[%lu;%luH", ESC, r, c);
}

void window(uint8_t x11, uint8_t y11, uint8_t x22, uint8_t y22) {
    //Size of window:
    uint8_t x1 = x11;
    uint8_t y1 = y11;
    uint8_t x2 = x22;
    uint8_t y2 = y22;
    uint8_t i;

    for (i = 0; i < (y2-y1); i++) {
        gotoxy(x1,y1+i);
        color(0,7);
        printf(" ");
    }
    for (i = 0; i < (y2-y1); i++) {
        gotoxy(x2,y1+i);
        color(0,7);
        printf(" ");
    }
    for (i = 0; i < ((x2-x1)+1); i++) {
        gotoxy(x1+i,y1);
        color(0,7);
        printf(" ");
    }
    for (i = 0; i < (x2-x1); i++) {
        gotoxy((x1+i)+1,y2);
        color(0,7);
        printf(" ");
    }
    color(15,0);
    printf(" ");
}

void moveShip(){
    while(1){
        if (uart_get_char()=='d'){
            printf("up");
        }
    }
}

/*void fixtrangPos(struct trang (*t)) {
//Converts Trangs position and velocity to 18.14 (from 32.0)
    (*t).position.x=(*t).position.x<<14;
    (*t).position.y=(*t).position.y<<14;
    (*t).velocity.x=(*t).velocity.x<<14;
    (*t).velocity.y=(*t).velocity.y<<14;
}*/

//The next few functions are dedicated to the harbringer of
//Cosine DOOM, TRANG!!!
void trangNextPos(struct trang (*t)) {
//Calculate new position for Trang. Input is a pointer.
    uint32_t k = 2;
    (*t).position.x = (*t).position.x + (*t).velocity.x*k;
    (*t).position.y = (*t).position.y + (*t).velocity.y*k;
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
}

void eraseTrang (struct trang t) { //erases Trang with the same
    color(0,0);                    //method as Trang is drawn
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
}

void trangZag (struct trang (*t)) { //Moves Trang in a zig zag
    uint8_t firsty = (*t).position.y;// motion
    if ((*t).position.y - firsty > 4 || (*t).position.y - firsty < -4) {
        (*t).velocity.y *= -1; //If the current y position is
    }                          //farther from starting position
}                              //than 3, y velocity is reversed

// generate random numbers in range [lower, upper].
uint8_t randoms(uint8_t lower, uint8_t upper) {
    uint8_t num = (rand() % (upper - lower + 1)) + lower;
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


//The following functions are dedicated to the warriors of the
//Alik'r homeworlds; the one the call SQWOG
void sqwogNextPos(struct sqwog (*t)) {
//Calculate new position for Trang. Input is a pointer.
    uint32_t k = 2;
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
}

void sqwogBox (struct sqwog (*t)) { //Moves Sqwog in a square
    for (uint8_t i = 0; i < 5; i++) {    // motion
        for (uint8_t j = 0; j < 3; j++) {
            (*t).velocity.x = -1;
        }
        (*t).velocity.x = 0;
        for (uint8_t j = 0; j < 3; j++) {
            (*t).velocity.y = -1;
        }
        (*t).velocity.y = 0;
        for (uint8_t j = 0; j < 3; j++) {
            (*t).velocity.x = -1;
        }
        (*t).velocity.x = 0;
        for (uint8_t j = 0; j < 3; j++) {
            (*t).velocity.y = 1;
        }
        (*t).velocity.x = 0;
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







