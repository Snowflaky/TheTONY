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

//Resets background color
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

/*
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
    /* Intializes pseudo - random number generator */
    //srand((unsigned) time(&t));
    //return(rand() % (upper + 1 - lower) + lower);
    /* Intializes random number generator */
    //srand((unsigned) time(&t));
    //return(rand() % (upper + 1 - lower) + lower);
    //srand(time(&somesec));
    //int32_t num = (srand(time(0)) % (upper - lower + 1)) + lower;
    /*
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    printf ("Current local time and date: %s", asctime(timeinfo));
}

void awakenTrang(uint8_t spawn) {    //Bring Trang, the bringer
    if (spawn == 1) {                //of doom, to LIFE
        struct trang tra;
        tra.position.x = 110;
        tra.position.y = randoms(4,36);
        tra.velocity.x = -2;
        tra.velocity.y = -2;

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
        for(int i = 0; i < 20; i++) {
            sqwogNextPos(&skr);
            sqwogBox(&skr);
            drawSqwog(skr);
            eraseSqwog(skr);
        }
    }
}

*/
