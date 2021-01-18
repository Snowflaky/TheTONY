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

    /*clrscr();
    while(1){
    gotoxy(1,1);
    printf("         ");
    gotoxy(1,1);
    printf("%d",time.milliSec);
    gotoxy(1,3);
    printf("         ");
    gotoxy(1,3);
    printf("%d",time.centiSec);
    gotoxy(1,5);
    printf("         ");
    gotoxy(1,5);
    printf("%d",time.second);
    gotoxy(1,7);
    printf("         ");
    gotoxy(1,7);
    printf("%d",time.minute);
    }*/


    uint8_t u=0;
    uint8_t shooting=0;
    int32_t goTime=100000;
    uint16_t score=0;
    uint8_t pause=0;
    uint8_t lives=3;

    lcd_init(); //initialize display
    uint8_t buffer[512];//initialize buffer array
    memset (buffer,0x00,512);//set buffer to all 0's (clear LCD screen)

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

    struct asteroid_t dodge1;
    dodge1.position.x=139;
    dodge1.position.y=7;
    dodge1.velocity.x=-1;
    dodge1.velocity.y=0;
    struct asteroid_t oldDodge1;
    struct asteroid_t dodge2;
    dodge2.position.x=117;
    dodge2.position.y=16;
    dodge2.velocity.x=-1;
    dodge2.velocity.y=0;
    struct asteroid_t oldDodge2;
    struct asteroid_t dodge3;
    dodge3.position.x=130;
    dodge3.position.y=23;
    dodge3.velocity.x=-1;
    dodge3.velocity.y=0;
    struct asteroid_t oldDodge3;
    struct asteroid_t dodge4;
    dodge4.position.x=107;
    dodge4.position.y=29;
    dodge4.velocity.x=-1;
    dodge4.velocity.y=0;
    struct asteroid_t oldDodge4;
    struct asteroid_t dodge5;
    dodge5.position.x=125;
    dodge5.position.y=34;
    dodge5.velocity.x=-1;
    dodge5.velocity.y=0;
    struct asteroid_t oldDodge5;

    struct bullet_t bullet;
    bullet.position.x = 3;
    bullet.position.y = ship.position.y;
    bullet.velocity.x = 1;
    bullet.velocity.y = 0;
    struct bullet_t oldBullet;

    //randomTra1=randoms(5,35);
    struct enemy e1;
    e1.randomNo = randoms(9,32);
    e1.enemyType = 1;
    e1.position.x = 135;
    e1.position.y = e1.randomNo;
    e1.velocity.x = -1;
    e1.hp = 2;
    e1.firstx = 135;
    e1.firsty = e1.randomNo;

    struct enemy e2;
    e2.randomNo = randoms(9,32);
    e2.enemyType = 2;
    e2.position.x = 135;
    e2.position.y = e2.randomNo;
    e2.velocity.x = -1;
    e2.hp = 2;
    e2.firstx = 135;
    e2.firsty = e2.randomNo;

    if (e1.enemyType==1){
        e1.velocity.y=1;
    }
    else if (e1.enemyType==2){
        e1.velocity.y=0;
    }
    if (e2.enemyType==1){
        e2.velocity.y=1;
    }
    else if (e2.enemyType==2){
        e2.velocity.y=0;
    }

    while(1){

        //If time runs out or you have no lives left it is GAME OVER!
        if (goTime<=0 || lives==0){
            while(pause==0){
                clrscr();
                gotoxy(70,20);
                printf("GAME OVER");
                //to be continued...
            }
        }

        //Prints the spaceship, asteroids, Trangs, Sqwoqs and their nets
        if(timeFlagPrint==1){
            drawEnemy(e1);
            drawEnemy(e2);
            printShip(ship, oldShip);
            printAsteroid(asteroid1, oldAsteroid1);
            printAsteroid(asteroid2, oldAsteroid2);
            printAsteroid(asteroid3, oldAsteroid3);
            printAsteroid(asteroid4, oldAsteroid4);
            printAsteroid(asteroid5, oldAsteroid5);
            printDodge(dodge1, oldDodge1);
            printDodge(dodge2, oldDodge2);
            printDodge(dodge3, oldDodge3);
            printDodge(dodge4, oldDodge4);
            printDodge(dodge5, oldDodge5);
            printBullet(bullet, oldBullet);
            u=keyInput();
            moveShip(u,&ship, &oldShip);
            if (shooting==0){
                shooting=startBullet(ship,u);
            }
            bullet.position.y=shooting;
            gotoxy(40,6);
            printf("score: %d",score);

            timeFlagPrint=0;
        }


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
            moveDodge(dodge1.position.y,&dodge1,&oldDodge1);
            moveDodge(dodge3.position.y,&dodge3,&oldDodge3);
            moveDodge(dodge5.position.y,&dodge5,&oldDodge5);
            if (compDoSh(ship,dodge1)==1 || compDoSh(ship,dodge2)==1 || compDoSh(ship,dodge3)==1 ||
            compDoSh(ship,dodge4)==1 || compDoSh(ship,dodge5)==1){
            lives-=1;
            }
            gotoxy(100,12);
            printf("lives: %d",lives);
            timeFlagA1=0;
        }
        if(timeFlagA2>=6){//change this number for change of asteroid speed
            moveAsteroid(asteroid3.position.x,&asteroid3,&oldAsteroid3);
            moveAsteroid(asteroid5.position.x,&asteroid5,&oldAsteroid5);
            moveDodge(dodge2.position.y,&dodge2,&oldDodge2);
            moveDodge(dodge4.position.y,&dodge4,&oldDodge4);
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

        if(compBuEn(bullet,e1)==1){
            eraseEnemy(e1);
            e1.position.x = 135;
            e1.position.y = e1.randomNo;
            score+=500;
        }
        if(compBuEn(bullet,e2)==1){
            eraseEnemy(e2);
            e2.position.x = 135;
            e2.position.y = e2.randomNo;
            score+=500;
        }




        if (bullet.position.x==3 && bullet.velocity.x==-1){
            shooting=0;
            bullet.velocity.x=1;
        }
        if (timeFlagTra>=4){
            eraseEnemy(e1);
            enemyNextPos(&e1);
            enemyMotion(&e1);
            if (enemyBreach(e1)==1){
                goTime-=1000;
                gotoxy(100,12);
            }
            eraseEnemy(e2);
            enemyNextPos(&e2);
            enemyMotion(&e2);
            timeFlagTra=0;
            if (enemyBreach(e2)==1){
                goTime-=1000;
                gotoxy(100,13);
            }
        }

        gotoxy(100,10);
        printf("                     ");
        gotoxy(100,10);
        goTime=goTime-timeFlagScore;
        printf("time: %d",goTime);



        // Casting integers to strings
        char strGoTime[10];
        sprintf(strGoTime, "%d", goTime);//convert goTime to string
        char strScore[10];
        sprintf(strScore, "%d", score);//convert score to string
        char strLives[5];
        sprintf(strLives, "%d", lives);//convert lives to string

        //put strings in buffer:
        lcd_write_string("Time:",0,1,&buffer);
        lcd_write_string(strGoTime,35,1,&buffer);

        lcd_write_string("Lives:",0,2,&buffer);
        lcd_write_string(strLives,40,2,&buffer);

        lcd_write_string("Score:",0,3,&buffer);
        lcd_write_string(strScore,40,3,&buffer);

        //push buffer
        lcd_push_buffer(buffer);




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
