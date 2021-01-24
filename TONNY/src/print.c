#include "print.h"
#include "string.h"

#define ESC 0x1B

/***********************/
/*** Print functions ***/
/***********************/

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
    printf("%c[1m", ESC);//bold script
    gotoxy(ship.position.x + 1,ship.position.y - 1);
    printf("-");
    gotoxy(ship.position.x + 1,ship.position.y + 1);
    printf("-");
    printf("%c[22m", ESC);//normal script
    color(15,0);
}

//Super-ship
void printPowerShip (struct ship_t ship, struct ship_t oldShip) {
//Same function as printShip, but prints ship in another color to indicate power up
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
    color(0,2);
    gotoxy(ship.position.x + 1,ship.position.y);
    printf("O");
    color(9,2);
    gotoxy(ship.position.x,ship.position.y - 1);
    printf("D");
    gotoxy(ship.position.x,ship.position.y + 1);
    printf("D");
    color(10,2);
    gotoxy(ship.position.x,ship.position.y);
    printf("=");
    color(1,0);
    printf("%c[1m", ESC);//bold script
    gotoxy(ship.position.x + 1,ship.position.y - 1);
    printf("-");
    gotoxy(ship.position.x + 1,ship.position.y + 1);
    printf("-");
    printf("%c[22m", ESC);//normal script
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
            printf("%c[1m",ESC);//bold script
            printf("=");
            fgcolor(15);
            printf("%c[22m",ESC);//normal script
        }
    }
}

//Prints enemies nets
void printDodge (struct asteroid_t dodge, struct asteroid_t oldDodge){
//Delete nets at old position and prints nets at position (x,y).
//Input: pointer to dodge structure.
    gotoxy(oldDodge.position.x,oldDodge.position.y);
    printf(" ");
    gotoxy(dodge.position.x,dodge.position.y);
    fgcolor(13);
    printf("%c[1m", ESC);//bold script
    printf("#");
    fgcolor(15);
    printf("%c[22m", ESC);//normal script
}

//Prints power up and deletes power up at previous position
void printPower (struct bullet_t power, struct bullet_t oldPower){
//Delete power up at old position and prints power up at position (x,y).
//Input: pointer to power up structure.
    if (power.position.x>2){//erase
        gotoxy(oldPower.position.x,oldPower.position.y);
        printf(" ");
    }
    if (power.position.x>3){//print
        gotoxy(power.position.x,power.position.y);
        color(14,2);
        printf("%c[1m", ESC);//bold script
        printf("¤");
        color(15,0);
        printf("%c[22m", ESC);//normal script
    }
}

//Prints the enemies
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

//Erases the enemies
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

void printMain(){
    window(50,90,15,27,4);
    gotoxy(65,17);
    printf("MAIN MENU");
    gotoxy(62,19);
    printf("1. Game levels");
    gotoxy(62,21);
    printf("2. How to play");
    gotoxy(62,23);
    printf("3. Credits");
}

void printUserMenu(){
    gotoxy(0,10);
    printf("The spaceship moves up when you press 'w' and down when you press 's'.\nTo shoot at your enemy, press 'p'.\n");
    printf("The enemies have different paths of movements, so observe them closely!\nThey also throw quantum nets.");
    printf("They damage your spaceship so be careful to avoid them.\nIt can only withstand 2 hits. A third is fatal.\n");
    printf("Space is also filled with asteroid which may help you or hinder you in your quest to shoot enemies.\n");
    printf("You only have so much fuel in your tank and your spaceship burns through it as you travel through space.\n");
    printf("Every time an enemy get past you they suck a little bit of it.\nWhen your tank is empty the game ends.\n");
    printf("Your fuel level, lives and score is displayed on the LCD screen\n");
    printf("An LED in your control-room indicates your fuel levels.\nIt changes at a half-tank and a quarter-tank.\n");
    printf("And if you are wondering what that sound is, don't worry.\nThat's just the sound of your fuel tank emptying!\n");
    printf("You can always end a game and return to the main menu by pressing '0'\n");
    printf("\nPress 0 to go back to main menu\n\n");
    printf("*********************************************************************\n");
    printf("      PSSSTTT!!!\n");
    printf("If your Boss suddenly walks by while you are playing, just press 'B',\n");
    printf("he won't suspect a thing ;)\n");
    printf("*********************************************************************\n");
}

void printCredit(){
    gotoxy(0,10);
    printf("This game is made by:\nSara\nJosefine\n& Erik\n");
    printf("\nPress 0 to go back to main menu");
}

void printLevel(){
    window(50,90,15,27,4);
    gotoxy(64,17);
    printf("CHOOSE LEVEL");
    gotoxy(62,19);
    printf("1. Level 1");
    gotoxy(62,21);
    printf("2. Level 2");
    gotoxy(62,23);
    printf("3. Level 3");
    gotoxy(57,25);
    printf("Press 0 to go back to main menu");
}
