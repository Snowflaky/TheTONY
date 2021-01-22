#include "print.h"
#include "string.h"

#define ESC 0x1B

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
        color(0,1);
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


