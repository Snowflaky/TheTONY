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

void window() {
    //Size of window:
    uint8_t x1 = 0;
    uint8_t y1 = 0;
    uint8_t x2 = 140;
    uint8_t y2 = 40;
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

void fixtrangPos(struct trang (*t)) {
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


