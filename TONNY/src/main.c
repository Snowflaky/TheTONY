/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f30x_conf.h"
#include "ANSI.h"

int main(void)
{
    uart_init (9600);

    hello();
    clrscr();
    window(0,140,0,40,7);

//Asteroid and ship will not print together.
    struct ship_t ship;
    ship.position.x = 2; // remember fixpoint 18.14
    ship.position.y=19;
    ship.hp=0;
    uint8_t y = 0;
    while(1){
        while (y<1){
            y=keyInput();
        }
        moveShip(y,&ship);
        printShip(ship);
        y=0;

    }

    struct asteroid_t asteroid;
    asteroid.position.x=0;
    asteroid.position.y=2;
    while(1){
        moveAsteroid(10, &asteroid);
        printAsteroid(asteroid);
    }




   /* struct trang trang;
    trang.x = 130;
    trang.y = 25;
    trang.vx = -2;
    trang.vy = -1;
    trang.hp = 5;

    fixtrangPos(&trang);
    trangNextPos(&trang);
    drawTrang(trang);
*/
    while(1)
  {
        //printf("%c",uart_get_char());

}}
