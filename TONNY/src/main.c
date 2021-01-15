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
    uart_init (115200);
        //1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200

    //Initiate timer (interrupt is set to one every 0.01 second)
    RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000;//0000 0000 0000 0000
    TIM2->ARR = 0x0000F9FF;//set reload value to 999
    TIM2->PSC = 0x0000;//set prescale to 639
    TIM2->CR1 |= 0x0001;//enable timer
    TIM2->DIER |= 0x0001; // Enable timer 2 interrupts
    NVIC_SetPriority(TIM2_IRQn, 0001); // Set interrupt priority=1 (high)
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt


    //hello();
    bgcolor(0);

    color(15,0);
    clrscr();
    //window(2, 2, 139, 39);

<<<<<<< Updated upstream
    awakenSqwog(1);
    awakenSqwog(1);
    awakenSqwog(1);
    awakenSqwog(1);
    awakenSqwog(1);
=======
    clrscr();
    window(0,140,0,40,7);
    /*while(){
    boss(keyInput());
    }*/

    struct ship_t ship;
    ship.position.x = 2;
    ship.position.y=19;
    ship.hp=0;
    uint8_t u = 0;
    uint8_t w = 0;
    uint8_t v = 0;

    struct asteroid_t asteroid;
    asteroid.position.x=10;
    asteroid.position.y=2;

    while(1){
   //read
        if (time.centiSec==25 || time.centiSec==50 || time.centiSec==75 || time.centiSec==99){
            w=1;
        }
        else{
            w=0;
        }
        //countFlag

        if (timeFlag3==20){

            printAsteroid(asteroid);

            timeFlag3=0;
        }

        if (timeFlag2==9){

            printShip(ship);

            timeFlag2=0;
        }

        if (timeFlag==1){
            u=keyInput();
            moveShip(u,&ship);

            if (w==1){
                moveAsteroid(asteroid.position.x, &asteroid);
            }
            /*if (asteroidFlag==1){
                struct asteroid_t asteroid
            }*/
        timeFlag=0;
        }
        // dette skal være timemr afhængig

        //
    }


    /*struct bullet_t bullet;
        bullet.position.x = 3;
        bullet.position.y = ship.position.y;
        bullet.velocity.x = 1;
        bullet.velocity.y = 0;
    uint8_t y=0;
    while(1){
        while (y<1){
            y=keyInput();
        }
        moveBullet(y,&bullet);
        printf("%d\n",bullet.position.x);
        //printBullet(bullet);
    }*/
>>>>>>> Stashed changes

    clrscr();
    window(0,140,0,40,7);
    /*while(){
    boss(keyInput());
    }*/

    struct ship_t ship;
    ship.position.x = 2;
    ship.position.y=19;
    ship.hp=0;
    uint8_t u = 0;
    uint8_t w = 0;
    uint8_t v = 0;

    struct asteroid_t asteroid;
    asteroid.position.x=10;
    asteroid.position.y=2;

    while(1){
   //read
        if (time.centiSec==25 || time.centiSec==50 || time.centiSec==75 || time.centiSec==99){
            w=1;
        }
        else{
            w=0;
        }
        //countFlag

        if (timeFlag3==20){

            printAsteroid(asteroid);

            timeFlag3=0;
        }

        if (timeFlag2==9){

            printShip(ship);

            timeFlag2=0;
        }

        if (timeFlag==1){
            u=keyInput();
            moveShip(u,&ship);

            if (w==1){
                moveAsteroid(asteroid.position.x, &asteroid);
            }
            /*if (asteroidFlag==1){
                struct asteroid_t asteroid
            }*/
        timeFlag=0;
        }
        // dette skal være timemr afhængig

        //
    }


    /*struct bullet_t bullet;
        bullet.position.x = 3;
        bullet.position.y = ship.position.y;
        bullet.velocity.x = 1;
        bullet.velocity.y = 0;
    uint8_t y=0;
    while(1){
        while (y<1){
            y=keyInput();
        }
        moveBullet(y,&bullet);
        printf("%d\n",bullet.position.x);
        //printBullet(bullet);
    }*/

    while(1){}
}
