#include "movement.h"
#include "string.h"



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

//Moves power up
void movePower (struct bullet_t *power, struct bullet_t *oldPower){
//Moves power up -1 along x-axis
//Input: pointer to power struct and old power struct
    (*oldPower).position.x=(*power).position.x;
    (*oldPower).position.y=(*power).position.y;
    (*power).position.x=(*power).position.x+(*power).velocity.x;
}

void enemyMotion (struct enemy (*e)) {
//Moves enemy in pattern determined by enemy type
    if ((*e).position.x<5){
        (*e).position.x=135;
    }
    if ((*e).enemyType == 1) {
        //Trang (enemy type 1) moves in a zig zag motion, shifting y-velocity based on y-position.
        if (((*e).position.y - (*e).firsty) >= 4 || ((*e).position.y - (*e).firsty) <= -4) {
            (*e).velocity.y *= -1;
        }
    }
    else if ((*e).enemyType == 2) {
        //Skwoq (enemy type 2) moves in a square wave.
            //Moves upwards
        if ((((*e).position.x - (*e).firstx == -10) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -30) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -50) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -70) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -90) && ((*e).position.y - (*e).firsty == 0)) ||
            (((*e).position.x - (*e).firstx == -110) && ((*e).position.y - (*e).firsty == 0))) {
            (*e).velocity.y = -1;
            (*e).velocity.x = 0;
        }
            //Moves forward
        else if (((*e).position.y - (*e).firsty == -5 && (*e).position.x - (*e).firstx == -10) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -30)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -50)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -70)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -90)) ||
            (((*e).position.y - (*e).firsty == -5) && ((*e).position.x - (*e).firstx == -110))) {
            (*e).velocity.y = 0;
            (*e).velocity.x = -1;
        }
            //Moves downward
        else if (((*e).position.x - (*e).firstx == -20 && (*e).position.y - (*e).firsty == -5) ||
            (((*e).position.x - (*e).firstx == -40) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -60) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -80) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -100) && ((*e).position.y - (*e).firsty == -5)) ||
            (((*e).position.x - (*e).firstx == -120) && ((*e).position.y - (*e).firsty == -5))) {
            (*e).velocity.y = 1;
            (*e).velocity.x = 0;
        }
            //Moves forward
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

void enemyNextPos (struct enemy *e) {
    //Calculate new position for Trang. Input is a pointer.
    uint32_t k = 1;
    (*e).position.x = (*e).position.x + (*e).velocity.x*k;
    (*e).position.y = (*e).position.y + (*e).velocity.y*k;
}



