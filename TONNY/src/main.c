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
    uart_init (115200);//1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
    //Initiate timer (interrupt is set to one every 0.01 second)
    RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000;//0000 0000 0000 0000
    TIM2->ARR = 0x0000003F;//set reload value to 999
    TIM2->PSC = 0x0000;//set prescale to 639
    TIM2->CR1 |= 0x0001;//enable timer
    TIM2->DIER |= 0x0001; // Enable timer 2 interrupts
    NVIC_SetPriority(TIM2_IRQn, 0001); // Set interrupt priority=1 (high)
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt

    uint8_t u=0;
    uint8_t shooting=0;


    color(15,0);
    clrscr();
    window(0,140,0,40,7);

    struct ship_t ship;
    ship.position.x = 2;
    ship.position.y=19;
    ship.hp=0;
    struct ship_t oldShip;

    struct asteroid_t asteroid1;
    asteroid1.position.x=20;
    asteroid1.position.y=2;
    asteroid1.velocity.x=0;
    asteroid1.velocity.y=1;
    struct asteroid_t oldAsteroid1;
    struct asteroid_t asteroid2;
    asteroid2.position.x=35;
    asteroid2.position.y=20;
    asteroid2.velocity.x=0;
    asteroid2.velocity.y=1;
    struct asteroid_t oldAsteroid2;
    struct asteroid_t asteroid3;
    asteroid3.position.x=50;
    asteroid3.position.y=2;
    asteroid3.velocity.x=0;
    asteroid3.velocity.y=1;
    struct asteroid_t oldAsteroid3;
    struct asteroid_t asteroid4;
    asteroid4.position.x=62;
    asteroid4.position.y=32;
    asteroid4.velocity.x=0;
    asteroid4.velocity.y=1;
    struct asteroid_t oldAsteroid4;
    struct asteroid_t asteroid5;
    asteroid5.position.x=70;
    asteroid5.position.y=10;
    asteroid5.velocity.x=0;
    asteroid5.velocity.y=1;
    struct asteroid_t oldAsteroid5;

    struct bullet_t bullet;
    bullet.position.x = 3;
    bullet.position.y = ship.position.y;
    bullet.velocity.x = 1;
    bullet.velocity.y = 0;
    struct bullet_t oldBullet;

    //randomTra1=randoms(5,35);
    struct trang tra1;
    tra1.random = randoms(9,32);
    tra1.position.x = 135;
    tra1.position.y = tra1.random;
    tra1.velocity.x = -1;
    tra1.velocity.y = 1;
    tra1.hp = 2;
    tra1.firstx = 135;
    tra1.firsty = tra1.random;

    struct trang tra2;
    tra2.random = randoms(9,32);
    tra2.position.x = 135;
    tra2.position.y = tra2.random;
    tra2.velocity.x = -1;
    tra2.velocity.y = 1;
    tra2.hp = 2;
    tra2.firstx = 135;
    tra2.firsty = tra2.random;

    struct sqwog sqr;
    sqr.position.x = 135;
    sqr.position.y = 20;
    sqr.velocity.x = -1;
    sqr.velocity.y = 0;
    sqr.hp = 2;
    sqr.firstx = 135;
    sqr.firsty = 20;

    while(1){
        if(timeFlagPrint==1){
            drawTrang(tra1);
            drawTrang(tra2);
            printShip(ship, oldShip);
            printAsteroid(asteroid1, oldAsteroid1);
            printAsteroid(asteroid2, oldAsteroid2);
            printAsteroid(asteroid3, oldAsteroid3);
            printAsteroid(asteroid4, oldAsteroid4);
            printAsteroid(asteroid5, oldAsteroid5);
            printBullet(bullet, oldBullet);
            u=keyInput();
            moveShip(u,&ship, &oldShip);
            if (shooting==0){
                shooting=startBullet(ship,u);
            }
            bullet.position.y=shooting;

            timeFlagPrint=0;
        }

        /*if (timeFlagDrawT==15){

            timeFlagDrawT=0;
        }*/


        if(timeFlagBullet>=1){//change this number for change of bullet speed
            if (shooting>0){
                moveBullet(shooting, &bullet, &oldBullet);
            }
            if (bullet.position.x==139){
                shooting=0;
            }
            timeFlagBullet=0;
        }

        if(timeFlagA1>=10){//change this number for change of asteroid speed
            moveAsteroid(asteroid1.position.x,&asteroid1,&oldAsteroid1);
            moveAsteroid(asteroid2.position.x,&asteroid2,&oldAsteroid2);
            moveAsteroid(asteroid4.position.x,&asteroid4,&oldAsteroid4);
            timeFlagA1=0;
        }
        if(timeFlagA2>=6){//change this number for change of asteroid speed
            moveAsteroid(asteroid3.position.x,&asteroid3,&oldAsteroid3);
            moveAsteroid(asteroid5.position.x,&asteroid5,&oldAsteroid5);
            timeFlagA2=0;
        }

        if (compBuAs(bullet,asteroid1)==1){
            bullet.velocity.x=-1;
        }
        if (compBuAs(bullet,asteroid2)==1){
            bullet.velocity.x=-1;
        }
        if (compBuAs(bullet,asteroid3)==1){
            bullet.velocity.x=-1;
        }
        if (compBuAs(bullet,asteroid4)==1){
            bullet.velocity.x=-1;
        }
        if (compBuAs(bullet,asteroid5)==1){
            bullet.velocity.x=-1;
        }

        if (bullet.position.x==3 && bullet.velocity.x==-1){
            shooting=0;
            bullet.velocity.x=1;
        }
        if (timeFlagTra>=4){
            eraseTrang(tra1);
            trangNextPos(&tra1);
            trangZag(&tra1);
            eraseTrang(tra2);
            trangNextPos(&tra2);
            trangZag(&tra2);
            timeFlagTra=0;
        }
    }



    /*while(){
    boss(keyInput());
    }

    struct ship_t ship;
    ship.position.x = 2;
    ship.position.y=19;
    ship.hp=0;
    uint8_t u = 0;
    uint8_t w = 0;
    uint8_t v = 0;
    uint8_t shooting = 0;

    struct asteroid_t asteroid;
    asteroid.position.x=10;
    asteroid.position.y=2;
    asteroid.velocity.x=0;
    asteroid.velocity.y=1;

    struct bullet_t bullet;
    bullet.position.x = 3;
    bullet.position.y = ship.position.y;
    bullet.velocity.x = 1;
    bullet.velocity.y = 0;

   struct sqwog skr;
   skr.position.x = 110;
   skr.position.y = 20;
   skr.velocity.x = -1;
   skr.velocity.y = 0;
   skr.hp = 5;
   skr.firsty = 20;
   skr.firstx = 110;

    while(1){
   //read
        if (time.centiSec==25 || time.centiSec==50 || time.centiSec==75 || time.centiSec==99){
            w=1;
        }
        else{
            w=0;
        }
        w=1;
        //countFlag

        if (timeFlag3==20){
            //printAsteroid(asteroid);
            //drawSqwog(skr);
            timeFlag3=0;
        }
        if (timeFlag2==9){
            printShip(ship);
            if (shooting>0){
                printBullet(bullet);
            }
            timeFlag2=0;
        }

       if (timeFlag==1){
            u=keyInput();
            moveShip(u,&ship);
            shooting=startBullet(ship,u);
            gotoxy(3,47);
            printf("%d",shooting);
            if (timeFlag4==25){
                //if(shooting>0){
                    bullet.position.y=shooting;
                    bullet.position.x++;
                    moveBullet(shooting, &bullet);
                    gotoxy(3,45);
                    printf("%d",bullet.position.x);


                //moveAsteroid(asteroid.position.x,&asteroid);

                if (bullet.position.x==139){
                    shooting=0;
                timeFlag4=0;
                }
            }

            /*if (w==1){

                eraseSqwog(skr);
                sqwogBox(&skr);
                sqwogNextPos(&skr);

            }*/
            /*if (asteroidFlag==1){
                struct asteroid_t asteroid
            }
        timeFlag=0;
        }
        // dette skal være timemr afhængig
    }*/



   /* uint8_t y=0;
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
