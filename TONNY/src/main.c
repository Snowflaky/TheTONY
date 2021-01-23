/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Authors: Sara Nielsen Jensen, Erik Helmuth Rame & Josefine Dano
   Last changed date:  $Date:  24.01.2021
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f30x_conf.h"
#include "ANSI.h"
#include "string.h"
#include "inout.h"
#include "print.h"
#include "movement.h"
#include "game.h"
#include "timer.h"


int main(void)
{
    uart_init (115200);//1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200

    initTimer15();//Game timer:
    initTimer2();//Sound timer
    setFreq(0);

    initGPIO();//Initialize GPIO

//LCD Display
    lcd_init(); //initialize display
    uint8_t buffer[512];//initialize buffer array
    memset (buffer,0x00,512);//set buffer to all 0's (clear LCD screen)

    setLed(3);

//resets colors and clears screen
    color(15,0);
    clrscr();

    uint8_t startLevel=0, v=0, levelMenu=0, useMenu=0, creditMenu=0, u=0, shooting=0, shooting2=0, shooting3=0, pause=0, powerFlag=0, ADFlag1=1, ADFlag2=1, enemyFlag=1;
    timeFlagGame=0;
    int32_t fuel=10000;
    uint16_t score=0, highscore;
    int8_t lives=3;

//flags for printing enemies, asteroids and nets
//TONNY's Spaceship is built
    struct ship_t ship;
    struct ship_t oldShip;
    initShip(&ship,2,19);

//Asteroid belt and enemy nets initiated
    //old... structs are initiated, but no values are declared.
    //these filled when game is initiated
    struct asteroid_t asteroid1, asteroid2, asteroid3, asteroid4, asteroid5, asteroid6, asteroid7;
    struct asteroid_t oldAsteroid1, oldAsteroid2, oldAsteroid3, oldAsteroid4, oldAsteroid5, oldAsteroid6, oldAsteroid7;
    struct asteroid_t dodge1, dodge2, dodge3, dodge4, dodge5, dodge6, dodge7, dodge8, dodge9, dodge10;
    struct asteroid_t oldDodge1, oldDodge2, oldDodge3, oldDodge4, oldDodge5, oldDodge6, oldDodge7, oldDodge8, oldDodge9, oldDodge10;
    initAsteroid(&asteroid1, 20, 2, 0, 1);
    initAsteroid(&asteroid2, 35, 20, 0, 1);
    initAsteroid(&asteroid3, 50, 2, 0, 1);
    initAsteroid(&asteroid4, 62, 32, 0, 1);
    initAsteroid(&asteroid5, 70, 10, 0, 1);
    initAsteroid(&asteroid6, 41, 33, 0, 1);
    initAsteroid(&asteroid7, 83, 18, 0, 1);
    initAsteroid(&dodge1, 139, 7, -1, 0);
    initAsteroid(&dodge2, 117, 16, -1, 0);
    initAsteroid(&dodge3, 130, 23, -1, 0);
    initAsteroid(&dodge4, 107, 29, -1, 0);
    initAsteroid(&dodge5, 125, 34, -1, 0);
    initAsteroid(&dodge6, 126, 11, -1, 0);
    initAsteroid(&dodge7, 98, 20, -1, 0);
    initAsteroid(&dodge8, 50, 35, -1, 0);
    initAsteroid(&dodge9, 70, 28, -1, 0);
    initAsteroid(&dodge10, 82, 31, -1, 0);

//Bullet initiated
    struct bullet_t bullet, bullet2, bullet3, oldBullet, oldBullet2, oldBullet3;
    initBullet(&bullet,3,ship.position.y,1,0);
    initBullet(&bullet2,3,ship.position.y,1,0);
    initBullet(&bullet3,3,ship.position.y,1,0);

//Power up initiated
    struct bullet_t power, oldPower;
    initBullet(&power, 135, 16, -1, 0);

//Enemies are initiated
    struct enemy e1, e2, e3;
    initEnemy(&e1,1,135,22);
    initEnemy(&e2,2,135,30);
    initEnemy(&e3,1,135,13);

    /* ********************************************************************************
    //The first time you run the program please include the following code. This clears the address for the highscore.
    FLASH_Unlock();
    FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
    FLASH_ErasePage(0x0800F800);//erase page
    FLASH_ProgramHalfWord(0x0800F800,0);//write data
    FLASH_Lock();//lock flash*/

//Main menu
    while(1){

        window(50,90,15,27,4);
        gotoxy(65,17);
        printf("MAIN MENU");
        gotoxy(62,19);
        printf("1. Game levels");
        gotoxy(62,21);
        printf("2. How to play");
        gotoxy(62,23);
        printf("3. Credits");
        v = keyInput();

//How to play
        if (v==6){
            setFreq(0);
            useMenu=1;
            clrscr();
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
            while(useMenu==1){
                v = keyInput();
                if (v==8){
                    clrscr();
                    useMenu=0;
                }
            }
        }
//Credits
        if (v==7){
            setFreq(0);
            creditMenu=1;
            clrscr();
            gotoxy(0,10);
            printf("This game is made by:\nSara\nJosefine\n& Erik\n");
            printf("\nPress 0 to go back to main menu");
            while(creditMenu==1){
                v = keyInput();
                if (v==8){
                    clrscr();
                    creditMenu=0;
                }
            }
        }
//Level menu
        if (v==5){
            setFreq(0);
            levelMenu=1;
            clrscr();
            ADFlag1=0;
            ADFlag2=0;
            timeFlagPrint=1;
            timeFlagA1=0;
            timeFlagBullet=0;
            timeFlagA2=0;
            timeFlagEnemy=0;
            timeFlagGame=0;
            enemyFlag=0;
            powerFlag=0;
            fuel=10000;
            score=0;
            lives=3;
            shooting=0;
            shooting2=0;
            shooting3=0;
            pause=0;
            initBullet(&power, 135, 16, -1, 0);
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
            while(levelMenu==1){
                v = keyInput();
                if (v==5){
                    clrscr();
                    window(0,140,0,40,4);
                    timeFlagGame=0;
                    startLevel=1;
                    levelMenu=0;
                }
                if (v==6){
                    clrscr();
                    window(0,140,0,40,4);
                    timeFlagGame=0;
                    startLevel=2;
                    levelMenu=0;
                }
                if (v==7){
                    clrscr();
                    window(0,140,0,40,4);
                    timeFlagGame=0;
                    startLevel=3;
                    levelMenu=0;
                }
                if (v==8){
                    clrscr();
                    levelMenu=0;
                }
            }
        }
//This code plays the title melody in main menu
        uint16_t q=16;
        if (toneFlag<10){
            setFreq(123*q);
        }
        else if (toneFlag>10 && toneFlag<20){
            setFreq(82*q);
        }
        else if (toneFlag>20 && toneFlag<30){
            setFreq(98*q);
        }
        else if (toneFlag>30 && toneFlag<40){
            setFreq(61*q);
        }
        else if (toneFlag>40 && toneFlag<50){//5
            setFreq(82*q);
        }
        else if (toneFlag>50 && toneFlag<60){
            setFreq(49*q);
        }
        else if (toneFlag>60 && toneFlag<70){
            setFreq(41*q);
        }
        else if (toneFlag>70 && toneFlag<80){
            setFreq(123*q);
        }
        else if (toneFlag>80 && toneFlag<90){
            setFreq(110*q);
        }
        else if (toneFlag>90 && toneFlag<100){//10
            setFreq(73*q);
        }
        else if (toneFlag>100 && toneFlag<110){
            setFreq(93*q);
        }
        else if (toneFlag>110 && toneFlag<120){
            setFreq(55*q);
        }
        else if (toneFlag>120 && toneFlag<130){
            setFreq(73*q);
        }
        else if (toneFlag>130 && toneFlag<140){
            setFreq(46*q);
        }
        else if (toneFlag>140 && toneFlag<150){ //15
            setFreq(36*q);
        }
        else if (toneFlag>150 && toneFlag<160){
            setFreq(110*q);
        }
        else if (toneFlag>160 && toneFlag<170){
            setFreq(123*q);
        }
        else if (toneFlag>170 && toneFlag<180){
            setFreq(93*q);
        }
        else if (toneFlag>180 && toneFlag<190){
            setFreq(98*q);
        }
        else if (toneFlag>190 && toneFlag<200){ //20
            setFreq(65*q);
        }
        else if (toneFlag>200 && toneFlag<210){
            setFreq(49*q);
        }
        else if (toneFlag>210 && toneFlag<220){
            setFreq(65*q);
        }
        else if (toneFlag>220 && toneFlag<230){
            setFreq(49*q);
        }
        else if (toneFlag>230 && toneFlag<240){
            setFreq(41*q);
        }
        else if (toneFlag>240 && toneFlag<250){ //25
            setFreq(110*q);
        }
        else if (toneFlag>250 && toneFlag<260){
            setFreq(55*q);
        }
        else if (toneFlag>260 && toneFlag<270){
            setFreq(36*q);
        }
        else if (toneFlag>270 && toneFlag<280){
            setFreq(73*q);
        }
        else if (toneFlag>280 && toneFlag<290){
            setFreq(36*q);
        }
        else if (toneFlag>290 && toneFlag<300){ //30
            setFreq(93*q);
        }
        else if (toneFlag>300 && toneFlag<310){
            setFreq(73*q);
        }
        else if (toneFlag>310 && toneFlag<320){
            setFreq(110);
        }
        else{
            toneFlag=0;
        }

//GAME WHILE LOOP BEGINS
        while(startLevel>0){

//Prints enemies when their position has updated
            if (enemyFlag ==1) {
                drawEnemy(e1);
                if (startLevel<3){
                    drawEnemy(e2);
                }
                if (startLevel==2){
                    drawEnemy(e3);
                }
                enemyFlag=0;
            }

//Prints asteroids and enemy nets when their position has updated
            if (ADFlag1 == 1) {
                printAsteroid(asteroid1, oldAsteroid1);
                printAsteroid(asteroid2, oldAsteroid2);
                printAsteroid(asteroid4, oldAsteroid4);
                printDodge(dodge1, oldDodge1);
                printDodge(dodge3, oldDodge3);
                printDodge(dodge5, oldDodge5);
                if (startLevel == 3){
                    printDodge(dodge8,oldDodge8);
                    printDodge(dodge9,oldDodge9);
                    printDodge(dodge10,oldDodge10);
                }

                ADFlag1=0;
            }

            if (ADFlag2 == 1) {
                printAsteroid(asteroid3, oldAsteroid3);
                printAsteroid(asteroid5, oldAsteroid5);
                printDodge(dodge2, oldDodge2);
                printDodge(dodge4, oldDodge4);
                if(startLevel>=2){
                    printAsteroid(asteroid6, oldAsteroid6);
                    printAsteroid(asteroid7, oldAsteroid7);
                    printDodge(dodge6, oldDodge6);
                    printDodge(dodge7, oldDodge7);
                }
                ADFlag2=0;
            }
//LED showing how much time is left
            if( fuel>5000){
                setLed(0);
                setLed(2);//red
            }
            else if (fuel<5000 && fuel > 2500){
                setLed(0);
                setLed(4);//yellow
            }
            else if (fuel < 2500){
                setLed(0);
                setLed(1);//green
            }
            else {
                    setLed(2);
                    setLed(3);
            }

//Prints the spaceship and bullet, and reads input from player
            if(timeFlagPrint==1){
                if (powerFlag==1){
                    printPowerShip(ship, oldShip);
                }
                else{
                    printShip(ship, oldShip);
                }
                printBullet(bullet, oldBullet);
                printBullet(bullet2, oldBullet2);
                printBullet(bullet3, oldBullet3);
                u=keyInput();
//If boss-key is initiated
                if(u==3){
                    boss();
                    pause=1;
                    setFreq(0);
                    while (pause==1){
                        u=keyInput();
                        if(u==8){
                            pause=0;
                            u=0;
                            clrscr();
                            window(0,140,0,40,4);
                        }
                    }
                }
                moveShip(u,&ship, &oldShip);
                if (shooting3==0 && shooting>0 && shooting2>0){
                    shooting3=startBullet(ship,u);
                }
                if (shooting2==0 && shooting>0){
                    shooting2=startBullet(ship,u);
                }
                if (shooting==0){//Starts bullet when trigger is pulled
                    shooting=startBullet(ship,u);//shooting gets ships y-axis position as it's value
                }

                bullet.position.y=shooting; // bullet gets ships y-axis position at bullet initiation
                bullet2.position.y=shooting2;
                bullet3.position.y=shooting3;

                if (fuel<6000 && power.position.x>2){
                    printPower(power, oldPower);
                    movePower(&power, &oldPower);
                }

                if (fuel<2000){
                    powerFlag=0;
                }

//If time runs out or you have no lives left it is GAME OVER!
                if (fuel<=0 || lives<=0){
                    clrscr();
                    setLed(0);
                    setLed(3);//blue LED
                    while(pause==0){
                        u=keyInput();
                        gotoxy(70,20);
                        printf("GAME OVER");
                        gotoxy(59,22);
                        printf("Press 0 to return to main menu");
                        if (u==8){
                            shooting = 0;
                            bullet.position.x=3;
                            gotoxy(oldBullet.position.x,oldBullet.position.y);
                            printf(" ");
                            shooting2=0;
                            bullet2.position.x=3;
                            gotoxy(oldBullet2.position.x,oldBullet2.position.y);
                            printf(" ");
                            shooting3=0;
                            bullet3.position.x=3;
                            gotoxy(oldBullet3.position.x,oldBullet3.position.y);
                            printf(" ");
                            startLevel=0;
                            pause=1;
                        }
                        //to be continued...
                    }
                }
                //ends game and returns to main menu
                if (u==8) {
                    clrscr();
                    setLed(0);
                    setLed(3);//blue LED
                    shooting = 0;
                    bullet.position.x=3;
                    gotoxy(oldBullet.position.x,oldBullet.position.y);
                    printf(" ");
                    shooting2 = 0;
                    bullet2.position.x=3;
                    gotoxy(oldBullet2.position.x,oldBullet2.position.y);
                    printf(" ");
                    shooting3 = 0;
                    bullet3.position.x=3;
                    gotoxy(oldBullet3.position.x,oldBullet3.position.y);
                    printf(" ");
                    startLevel=0;
                    //pause=1;
                }

                timeFlagPrint=0;
            }


    //Updates bullet position
            if(timeFlagBullet>=1){//change this number for change of bullet speed
                if (shooting>0){//only updates position if bullet has been started
                    moveBullet(shooting, &bullet, &oldBullet);
                }
                if (shooting2>0){
                    moveBullet(shooting2, &bullet2, &oldBullet2);
                }
                if (shooting3>0){
                    moveBullet(shooting3, &bullet3, &oldBullet3);
                }
                if (bullet.position.x==139){//bullet position and initialization is reset when the bullet reaches the end of the game field
                    shooting=0;
                }
                if (bullet2.position.x==139){//bullet position and initialization is reset when the bullet reaches the end of the game field
                    shooting2=0;
                }
                if (bullet3.position.x==139){//bullet position and initialization is reset when the bullet reaches the end of the game field
                    shooting3=0;
                }
                timeFlagBullet=0;
            }

    //Moves asteroids and enemy nets
            if(timeFlagA1>=10){//change this number for change of asteroid speed
                moveAsteroid(asteroid1.position.x,&asteroid1,&oldAsteroid1);
                moveAsteroid(asteroid2.position.x,&asteroid2,&oldAsteroid2);
                moveAsteroid(asteroid4.position.x,&asteroid4,&oldAsteroid4);
                moveDodge(dodge1.position.y,&dodge1,&oldDodge1);
                moveDodge(dodge3.position.y,&dodge3,&oldDodge3);
                moveDodge(dodge5.position.y,&dodge5,&oldDodge5);
                if (startLevel == 3){
                    moveDodge(dodge8.position.y, &dodge8, &oldDodge8);
                    moveDodge(dodge9.position.y, &dodge9, &oldDodge9);
                    moveDodge(dodge10.position.y, &dodge10, &oldDodge10);
                }

            //checks if net hits ship
                if ((compDoSh(ship,dodge1)==1 || compDoSh(ship,dodge2)==1 || compDoSh(ship,dodge3)==1 ||
                    compDoSh(ship,dodge4)==1 || compDoSh(ship,dodge5)==1 || compDoSh(ship,dodge6)==1 ||
                    compDoSh(ship,dodge7)==1 || compDoSh(ship,dodge8)==1 || compDoSh(ship,dodge9)==1 ||
                    compDoSh(ship,dodge10)==1) && powerFlag!=1){
                    lives-=1;//when nets hits, one life is lost
                    gotoxy(ship.position.x+2,ship.position.y+2);
                    printf("OW!");
                }
                timeFlagA1=0;
                ADFlag1=1;//Asteroid and net positions have updated
            }
            if(timeFlagA2>=6){//change this number for change of asteroid speed
                moveAsteroid(asteroid3.position.x,&asteroid3,&oldAsteroid3);
                moveAsteroid(asteroid5.position.x,&asteroid5,&oldAsteroid5);
                moveDodge(dodge2.position.y,&dodge2,&oldDodge2);
                moveDodge(dodge4.position.y,&dodge4,&oldDodge4);
                if (startLevel>=2){
                    moveAsteroid(asteroid6.position.x,&asteroid6,&oldAsteroid6);
                    moveAsteroid(asteroid7.position.x,&asteroid7,&oldAsteroid7);
                    moveDodge(dodge6.position.y,&dodge6,&oldDodge6);
                    moveDodge(dodge7.position.y,&dodge7,&oldDodge7);
                }
                timeFlagA2=0;
                ADFlag2=1;//Asteroid and net positions have updated
            }

    // Detects asteroid hit, by comparing bullet and asteroid positions
            //if hit detected, bullet bounces back
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
            if (compBuAs(bullet2,asteroid1)==1){
                bullet2.velocity.x=-1;
            }
            if (compBuAs(bullet2,asteroid2)==1){
                bullet2.velocity.x=-1;
            }
            if (compBuAs(bullet2,asteroid3)==1){
                bullet2.velocity.x=-1;
            }
            if (compBuAs(bullet2,asteroid4)==1){
                bullet2.velocity.x=-1;
            }
            if (compBuAs(bullet2,asteroid5)==1){
                bullet2.velocity.x=-1;
            }
            if (compBuAs(bullet3,asteroid1)==1){
                bullet3.velocity.x=-1;
            }
            if (compBuAs(bullet3,asteroid2)==1){
                bullet3.velocity.x=-1;
            }
            if (compBuAs(bullet3,asteroid3)==1){
                bullet3.velocity.x=-1;
            }
            if (compBuAs(bullet3,asteroid4)==1){
                bullet3.velocity.x=-1;
            }
            if (compBuAs(bullet3,asteroid5)==1){
                bullet3.velocity.x=-1;
            }
            if (startLevel>=2){
                if (compBuAs(bullet,asteroid6)==1){
                    bullet.velocity.x=-1;
                }
                if (compBuAs(bullet,asteroid7)==1){
                    bullet.velocity.x=-1;
                }
                if (compBuAs(bullet2,asteroid6)==1){
                    bullet2.velocity.x=-1;
                }
                if (compBuAs(bullet2,asteroid7)==1){
                    bullet2.velocity.x=-1;
                }
                if (compBuAs(bullet3,asteroid6)==1){
                    bullet3.velocity.x=-1;
                }
                if (compBuAs(bullet3,asteroid7)==1){
                    bullet3.velocity.x=-1;
                }
            }
    //Restarts reflected bullets
            if (bullet.position.x==3 && bullet.velocity.x==-1){
                shooting=0;
                bullet.velocity.x=1;
            }
            if (bullet2.position.x==3 && bullet2.velocity.x==-1){
                shooting2=0;
                bullet2.velocity.x=1;
            }
            if (bullet3.position.x==3 && bullet3.velocity.x==-1){
                shooting3=0;
                bullet3.velocity.x=1;
            }


    //Erases enemy when hit, resets bullet, adds +500 to score
            if (compBuEn(bullet,e1)==1){
                eraseEnemy(e1);
                e1.position.x = 135;
                e1.position.y = e1.firsty;
                score+=500;
                setFreq(15000);
                shooting = 0;
                bullet.position.x=3;
                gotoxy(oldBullet.position.x,oldBullet.position.y);
                printf(" ");
            }
            if (compBuEn(bullet2,e1)==1){
                eraseEnemy(e1);
                e1.position.x = 135;
                e1.position.y = e1.firsty;
                score+=500;
                setFreq(15000);
                shooting2 = 0;
                bullet2.position.x=3;
                gotoxy(oldBullet2.position.x,oldBullet2.position.y);
                printf(" ");
            }
            if (compBuEn(bullet3,e1)==1){
                eraseEnemy(e1);
                e1.position.x = 135;
                e1.position.y = e1.firsty;
                score+=500;
                setFreq(15000);
                shooting3 = 0;
                bullet3.position.x=3;
                gotoxy(oldBullet3.position.x,oldBullet3.position.y);
                printf(" ");
            }
            if(startLevel<3 && compBuEn(bullet,e2)==1){
                eraseEnemy(e2);
                e2.position.x = 135;
                e2.position.y = e2.firsty;
                e2.velocity.x=-1;
                e2.velocity.y=0;
                score+=500;
                setFreq(15000);
                shooting = 0;//resets bullet
                bullet.position.x=3;
                gotoxy(oldBullet.position.x,oldBullet.position.y);
                printf(" ");
            }
            if(startLevel<3 && compBuEn(bullet2,e2)==1){
                eraseEnemy(e2);
                e2.position.x = 135;
                e2.position.y = e2.firsty;
                e2.velocity.x=-1;
                e2.velocity.y=0;
                score+=500;
                setFreq(15000);
                shooting2 = 0;//resets bullet
                bullet2.position.x=3;
                gotoxy(oldBullet2.position.x,oldBullet2.position.y);
                printf(" ");
            }
            if(startLevel<3 && compBuEn(bullet3,e2)==1){
                eraseEnemy(e2);
                e2.position.x = 135;
                e2.position.y = e2.firsty;
                e2.velocity.x=-1;
                e2.velocity.y=0;
                score+=500;
                setFreq(15000);
                shooting3 = 0;//resets bullet
                bullet3.position.x=3;
                gotoxy(oldBullet3.position.x,oldBullet3.position.y);
                printf(" ");
            }

            if(startLevel==2 && compBuEn(bullet,e3)==1){
                eraseEnemy(e3);
                e3.position.x = 135;
                e3.position.y = e3.firsty;
                score+=500;
                setFreq(15000);
                shooting = 0;//resets bullet
                bullet.position.x=3;
                gotoxy(oldBullet.position.x,oldBullet.position.y);
                printf(" ");
            }
            if(startLevel==2 && compBuEn(bullet2,e3)==1){
                eraseEnemy(e3);
                e3.position.x = 135;
                e3.position.y = e3.firsty;
                score+=500;
                setFreq(15000);
                shooting2 = 0;//resets bullet
                bullet2.position.x=3;
                gotoxy(oldBullet2.position.x,oldBullet2.position.y);
                printf(" ");
            }
            if(startLevel==2 && compBuEn(bullet3,e3)==1){
                eraseEnemy(e3);
                e3.position.x = 135;
                e3.position.y = e3.firsty;
                score+=500;
                setFreq(15000);
                shooting3 = 0;//resets bullet
                bullet3.position.x=3;
                gotoxy(oldBullet3.position.x,oldBullet3.position.y);
                printf(" ");
            }

            if (compBuPo(bullet,power)==1){
                shooting = 0;
                bullet.position.x=3;
                gotoxy(oldBullet.position.x,oldBullet.position.y);
                printf(" ");
                power.position.x=3;
                powerFlag=1;
            }
            if (compBuPo(bullet2,power)==1){
                shooting2 = 0;
                bullet2.position.x=3;
                gotoxy(oldBullet2.position.x,oldBullet2.position.y);
                printf(" ");
                power.position.x=3;
                powerFlag=1;
            }
            if (compBuPo(bullet3,power)==1){
                shooting3 = 0;
                bullet3.position.x=3;
                gotoxy(oldBullet3.position.x,oldBullet3.position.y);
                printf(" ");
                power.position.x=3;
                powerFlag=1;
            }

    //Enemy positions is updated every 1/25 second
    //If enemies get TONNY he loses time
            if (timeFlagEnemy>=4){
                eraseEnemy(e1);
                enemyNextPos(&e1);
                enemyMotion(&e1);
                if (enemyBreach(e1)==1){//Detects that the enemy has breached and subtracts 1000 from time
                    fuel-=1000;
                }
                if (startLevel<3){
                    eraseEnemy(e2);
                    enemyNextPos(&e2);
                    enemyMotion(&e2);
                    timeFlagEnemy=0;
                    if (enemyBreach(e2)==1){//Detects that the enemy has breached and subtracts 1000 from time
                        fuel-=1000;
                    }
                }
                if (startLevel == 2){
                    eraseEnemy(e3);
                    enemyNextPos(&e3);
                    enemyMotion(&e3);
                    timeFlagEnemy=0;
                    if (enemyBreach(e3)==1){//Detects that the enemy has breached and subtracts 1000 from time
                        fuel-=1000;
                    }
                }
                enemyFlag=1;//enemies position has updated
                //setLed(3);

            }


            if (fuel>7540){
                setFreq(80);
            }
            else if (fuel<7540 && fuel>5690){
                setFreq(140);
            }
            else if (fuel<5690 && fuel>4200){
                setFreq(280);
            }
            else if (fuel<4200 && fuel>3020){
                setFreq(560);
            }
            else if (fuel<3020 && fuel>2090){
                setFreq(1120);
            }
            else if (fuel<2090 && fuel>1380){
                setFreq(2240);
            }
            else if (fuel<1380 && fuel>850){
                setFreq(4480);
            }
            else if (fuel<850 && fuel>460){
                setFreq(8960);
            }
            else if (fuel<460 && fuel>180){
                setFreq(17920);
            }
            else if (fuel<180 ){
                setFreq(20000);
            }

    //gameTime is the remaining time the game
    //counts down every 10th of a second
            fuel=fuel-timeFlagGame;//genstart timeFlagGame!!!!!!!!!!
            highscore = updateHighscore(highscore,score);

    // Casting integers to strings
            char strFuel[10];
            sprintf(strFuel, "%ld", fuel);//convert goTime to string
            char strScore[10];
            sprintf(strScore, "%d", score);//convert score to string
            char strLives[5];
            sprintf(strLives, "%d", lives);//convert lives to string
            char strHighscore[10];
            sprintf(strHighscore, "%d", highscore);


            memset (buffer,0x00,512);
            lcd_push_buffer(buffer);

    //Puts strings in buffer:
            lcd_write_string("Fuel:",0,1,&buffer);
            lcd_write_string(strFuel,35,1,&buffer);

            lcd_write_string("Lives:",0,2,&buffer);
            lcd_write_string(strLives,40,2,&buffer);

            lcd_write_string("Score:",0,3,&buffer);
            lcd_write_string(strScore,40,3,&buffer);

            lcd_write_string("Highscore:",0,4,&buffer);
            lcd_write_string(strHighscore,60,4,&buffer);

    //Push buffer
            lcd_push_buffer(buffer);


            if (startLevel == 0){
                clrscr();

            }
        }//   END OF GAME WHILE LOOP

    }//END OF PRIORITY WHILE LOOP!!!

}//END OF MAIN!!!!!


/*
    struct asteroid_t dodge1;
    dodge1.position.x=139;
    dodge1.position.y=7;
    dodge1.velocity.x=-1;
    struct asteroid_t oldDodge1;
    struct asteroid_t dodge2;
    dodge2.position.x=117;
    dodge2.position.y=16;
    dodge2.velocity.x=-1;
    struct asteroid_t oldDodge2;
    struct asteroid_t dodge3;
    dodge3.position.x=130;
    dodge3.position.y=23;
    dodge3.velocity.x=-1;
    struct asteroid_t oldDodge3;
    struct asteroid_t dodge4;
    dodge4.position.x=107;
    dodge4.position.y=29;
    dodge4.velocity.x=-1;
    struct asteroid_t oldDodge4;
    struct asteroid_t dodge5;
    dodge5.position.x=125;
    dodge5.position.y=34;
    dodge5.velocity.x=-1;
    struct asteroid_t oldDodge5;
    struct asteroid_t dodge6;
    dodge6.position.x=126;
    dodge6.position.y=11;
    dodge6.velocity.x=-1;
    struct asteroid_t oldDodge6;
    struct asteroid_t dodge7;
    dodge7.position.x=98;
    dodge7.position.y=20;
    dodge7.velocity.x=-1;
    struct asteroid_t oldDodge7;
    struct asteroid_t dodge8;
    dodge8.position.x=50;
    dodge8.position.y=35;
    dodge8.velocity.x=-1;
    struct asteroid_t oldDodge8;
    struct asteroid_t dodge9;
    dodge9.position.x=70;
    dodge9.position.y=28;
    dodge9.velocity.x=-1;
    struct asteroid_t oldDodge9;
    struct asteroid_t dodge10;
    dodge10.position.x=82;
    dodge10.position.y=31;
    dodge10.velocity.x=-1;
    struct asteroid_t oldDodge10;
    */

/*
    struct asteroid_t asteroid1;
    asteroid1.position.x=20;
    asteroid1.position.y=2;
    asteroid1.velocity.y=1;
    struct asteroid_t oldAsteroid1;
    struct asteroid_t asteroid2;
    asteroid2.position.x=35;
    asteroid2.position.y=20;
    asteroid2.velocity.y=1;
    struct asteroid_t oldAsteroid2;
    struct asteroid_t asteroid3;
    asteroid3.position.x=50;
    asteroid3.position.y=2;
    asteroid3.velocity.y=1;
    struct asteroid_t oldAsteroid3;
    struct asteroid_t asteroid4;
    asteroid4.position.x=62;
    asteroid4.position.y=32;
    asteroid4.velocity.y=1;
    struct asteroid_t oldAsteroid4;
    struct asteroid_t asteroid5;
    asteroid5.position.x=70;
    asteroid5.position.y=10;
    asteroid5.velocity.y=1;
    struct asteroid_t oldAsteroid5;
    struct asteroid_t asteroid6;
    asteroid6.position.x=41;
    asteroid6.position.y=33;
    asteroid6.velocity.y=1;
    struct asteroid_t oldAsteroid6;
    struct asteroid_t asteroid7;
    asteroid7.position.x=83;
    asteroid7.position.y=18;
    asteroid7.velocity.y=1;
    struct asteroid_t oldAsteroid7;*/


/*
    bullet.position.x = 3;
    bullet.position.y = ship.position.y;
    bullet.velocity.x = 1;
    struct bullet_t oldBullet;
    struct bullet_t bullet2;
    bullet2.position.x = 3;
    bullet2.position.y = ship.position.y;
    bullet2.velocity.x = 1;
    struct bullet_t oldBullet2;
    struct bullet_t bullet3;
    bullet3.position.x = 3;
    bullet3.position.y = ship.position.y;
    bullet3.velocity.x = 1;
    struct bullet_t oldBullet3;*/
/*
    struct enemy e1;
    e1.enemyType = 1;
    e1.position.x = 135;
    e1.position.y = 22;
    decideVel(&e1);
    e1.firstx = 135;
    e1.firsty = e1.position.y;

    struct enemy e2;
    e2.enemyType = 2;
    e2.position.x = 135;
    e2.position.y = 30;
    decideVel(&e2);
    e2.firstx = 135;
    e2.firsty = e2.position.y;

    struct enemy e3;
    e3.enemyType = 1;
    e3.position.x = 135;
    e3.position.y = 13;
    decideVel(&e3);
    e3.firstx = 135;
    e3.firsty = e3.position.y;
*/



/*RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000;//0000 0000 0000 0000
    TIM2->ARR = 0x0000003F;//set reload value to 63 (microseconds)
    TIM2->PSC = 0x0000;//set prescale to 0 (microseconds)
    TIM2->CR1 |= 0x0001;//enable timer
    TIM2->DIER |= 0x0001; // Enable timer 2 interrupts
    NVIC_SetPriority(TIM2_IRQn, 0001); // Set interrupt priority=1 (high)
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt*/
