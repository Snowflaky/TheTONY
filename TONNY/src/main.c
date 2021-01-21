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


int main(void)
{
    uart_init (115200);//1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200
    //Initiate timer (interrupt is set to one every 0.01 second)
    /*RCC->APB1ENR |= RCC_APB1Periph_TIM2; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000;//0000 0000 0000 0000
    TIM2->ARR = 0x0000003F;//set reload value to 63 (microseconds)
    TIM2->PSC = 0x0000;//set prescale to 0 (microseconds)
    TIM2->CR1 |= 0x0001;//enable timer
    TIM2->DIER |= 0x0001; // Enable timer 2 interrupts
    NVIC_SetPriority(TIM2_IRQn, 0001); // Set interrupt priority=1 (high)
    NVIC_EnableIRQ(TIM2_IRQn); // Enable interrupt*/

//Game timer:
    RCC->APB2ENR |= RCC_APB2Periph_TIM15; //enable clock line to timer 15?
    TIM15->CR1 = 0x0000;
    TIM15->ARR = 0x003F; //set reload value to 63 (microseconds)
    TIM15->PSC = 0x0000; //set prescale to 0 (microseconds)
    TIM15->DIER |= 0x0001; //enable timer interrupt
    NVIC_SetPriority(TIM1_BRK_TIM15_IRQn, 0);
    NVIC_EnableIRQ(TIM1_BRK_TIM15_IRQn);
    TIM15->CR1 |= 0x0001; //enable timer

//Sound timer:
    RCC->APB1ENR |= 0x00000001; // Enable clock line to timer 2;
    TIM2->CR1 = 0x0000; // Disable timer
    TIM2->ARR = 1000; // Set auto reload value
    TIM2->PSC = 0x0C7F;//PRESCALER_VALUE; // Set pre-scaler value****************************
        //0x003F = millieconds
        //0x18FF = centiseconds
        //0x0C7F
    TIM2->CR1 |= 0x0001; // Enable timer
        //configure counter compare register:
    TIM2->CCER &= ~TIM_CCER_CC3P; // Clear CCER register
    TIM2->CCER |= 0x00000001 << 8; // Enable OC3 output
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3M; // Clear CCMR2 register
    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S;
    TIM2->CCMR2 |= TIM_OCMode_PWM1; // Set output mode to PWM1
    TIM2->CCMR2 &= ~TIM_CCMR2_OC3PE;
    TIM2->CCMR2 |= TIM_OCPreload_Enable;
    TIM2->CCR3 = 500; // Set duty cycle to 50 %
        //connect pin PB10 to timer (for alternate function):
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN; // Enable clock line for GPIO bank B
    GPIOB->MODER &= ~(0x00000003 << (10 * 2)); // Clear mode register
    GPIOB->MODER |= (0x00000002 << (10 * 2)); // Set mode register
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);

    setFreq(0);


    RCC->AHBENR |= RCC_AHBPeriph_GPIOA;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOB;
    RCC->AHBENR |= RCC_AHBPeriph_GPIOC;

    uint8_t startLevel=0;
    uint8_t v=0;
    uint8_t levelMenu=0;
    uint8_t useMenu=0;
    uint8_t creditMenu=0;
    uint8_t u=0;
    uint8_t shooting=0;
    int32_t fuel=10000;
    uint16_t score=0;
    uint8_t pause=0;
    int8_t lives=3;
    uint16_t highscore=0;


//flags for printing enemies, asteroids and nets
    uint8_t ADFlag1=1;
    uint8_t ADFlag2=1;
    uint8_t enemyFlag=1;

//LCD Display
    lcd_init(); //initialize display
    uint8_t buffer[512];//initialize buffer array
    memset (buffer,0x00,512);//set buffer to all 0's (clear LCD screen)

//resets colors and clears screen
    color(15,0);
    clrscr();

//TONNY's Spaceship is built
    struct ship_t ship;
    ship.position.x = 2;
    ship.position.y=19;
    struct ship_t oldShip;

//Asteroid belt initiated
    //old... structs are initiated, but no values are declared.
    //these filled when game is initiated
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
    struct asteroid_t asteroid6;
    asteroid6.position.x=41;
    asteroid6.position.y=33;
    asteroid6.velocity.x=0;
    asteroid6.velocity.y=1;
    struct asteroid_t oldAsteroid6;
    struct asteroid_t asteroid7;
    asteroid7.position.x=83;
    asteroid7.position.y=18;
    asteroid7.velocity.x=0;
    asteroid7.velocity.y=1;
    struct asteroid_t oldAsteroid7;

//Enemy nets initiated
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
    struct asteroid_t dodge6;
    dodge6.position.x=126;
    dodge6.position.y=11;
    dodge6.velocity.x=-1;
    dodge6.velocity.y=0;
    struct asteroid_t oldDodge6;
    struct asteroid_t dodge7;
    dodge7.position.x=98;
    dodge7.position.y=20;
    dodge7.velocity.x=-1;
    dodge7.velocity.y=0;
    struct asteroid_t oldDodge7;
    struct asteroid_t dodge8;
    dodge8.position.x=50;
    dodge8.position.y=35;
    dodge8.velocity.x=-1;
    dodge8.velocity.y=0;
    struct asteroid_t oldDodge8;
    struct asteroid_t dodge9;
    dodge9.position.x=70;
    dodge9.position.y=28;
    dodge9.velocity.x=-1;
    dodge9.velocity.y=0;
    struct asteroid_t oldDodge9;
    struct asteroid_t dodge10;
    dodge10.position.x=82;
    dodge10.position.y=31;
    dodge10.velocity.x=-1;
    dodge10.velocity.y=0;
    struct asteroid_t oldDodge10;

//Bullet initiated
    struct bullet_t bullet;
    bullet.position.x = 3;
    bullet.position.y = ship.position.y;
    bullet.velocity.x = 1;
    bullet.velocity.y = 0;
    struct bullet_t oldBullet;

//Enemies are initiated
    struct enemy e1;
    e1.randomNo = 22;//randoms(9,32);
    e1.enemyType = 1;
    e1.position.x = 135;
    e1.position.y =e1.randomNo;
    e1.velocity.x=-1;
    e1.firstx = 135;
    e1.firsty = e1.randomNo;

    struct enemy e2;
    e2.randomNo = 30;//randoms(9,32);
    e2.enemyType = 2;
    e2.position.x = 135;
    e2.position.y = e2.randomNo;
    decideVel(&e2);
    e2.firstx = 135;
    e2.firsty = e2.randomNo;

    struct enemy e3;
    e3.randomNo = 13;//randoms(15,30);
    e3.enemyType = 1;
    e3.position.x = 135;
    e3.position.y = e3.randomNo;
    e3.velocity.x=-1;
    e3.firstx = 135;
    e3.firsty = e3.randomNo;


//Determines velocity based on enemy type
    if (e1.enemyType==1){
        e1.velocity.y=1;
    }
    else if (e1.enemyType==2){
        e1.velocity.y=0;
    }
    /*if (e2.enemyType==1){
        e2.velocity.y=1;
    }
    else if (e2.enemyType==2){
        e2.velocity.y=0;
    }*/
    if (e3.enemyType==1){
        e3.velocity.y=1;
    }
    else if (e3.enemyType==2){
        e3.velocity.y=0;
    }

    setLed(3);

//Main menu
    while(1){
        //if(timeFlagPrint==1){
        setFreq(0);
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
            fuel=10000;
            score=0;
            lives=3;
            shooting=0;
            pause=0;

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


           // timeFlagPrint=0;

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
                printShip(ship, oldShip);
                printBullet(bullet, oldBullet);
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
                            clrscr();
                            window(0,140,0,40,4);
                        }
                    }
                }
                moveShip(u,&ship, &oldShip);
                if (shooting==0){//Starts bullet when trigger is pulled
                    shooting=startBullet(ship,u);//shooting gets ships y-axis position as it's value
                }
                bullet.position.y=shooting; // bullet gets ships y-axis position at bullet initiation

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
                            startLevel=0;
                            pause=1;
                        }
                        //to be continued...
                    }
                }
                timeFlagPrint=0;
            }

    //Updates bullet position
            if(timeFlagBullet>=1){//change this number for change of bullet speed
                if (shooting>0){//only updates position if bullet has been started
                    moveBullet(shooting, &bullet, &oldBullet);
                }
                if (bullet.position.x==139){//bullet position and initialization is reset when the bullet reaches the end of the game field
                    shooting=0;
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
                if (compDoSh(ship,dodge1)==1 || compDoSh(ship,dodge2)==1 || compDoSh(ship,dodge3)==1 ||
                    compDoSh(ship,dodge4)==1 || compDoSh(ship,dodge5)==1 || compDoSh(ship,dodge6)==1 ||
                    compDoSh(ship,dodge7)==1 || compDoSh(ship,dodge8)==1 || compDoSh(ship,dodge9)==1 || compDoSh(ship,dodge10)==1){
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
            if (startLevel>=2){
                if (compBuAs(bullet,asteroid6)==1){
                    bullet.velocity.x=-1;
                }
                if (compBuAs(bullet,asteroid7)==1){
                    bullet.velocity.x=-1;
                }
            }
    //Restarts reflected bullets
            if (bullet.position.x==3 && bullet.velocity.x==-1){
                shooting=0;
                bullet.velocity.x=1;
            }


    //Erases enemy when hit, resets bullet, adds +500 to score
           if(compBuEn(bullet,e1)==1){
                eraseEnemy(e1);
                e1.position.x = 135;
                e1.position.y = e1.randomNo;
                score+=500;
                setFreq(15000);
                shooting = 0;
                bullet.position.x=3;
                gotoxy(oldBullet.position.x,oldBullet.position.y);
                printf(" ");
            }
            if(startLevel<3 && compBuEn(bullet,e2)==1){
                eraseEnemy(e2);
                e2.position.x = 135;
                e2.position.y = e2.randomNo;
                score+=500;
                setFreq(15000);
                shooting = 0;//resets bullet
                bullet.position.x=3;
                gotoxy(oldBullet.position.x,oldBullet.position.y);
                printf(" ");
            }

            if(startLevel==2 && compBuEn(bullet,e3)==1){
                eraseEnemy(e3);
                e3.position.x = 135;
                e3.position.y = e3.randomNo;
                score+=500;
                setFreq(15000);
                shooting = 0;//resets bullet
                bullet.position.x=3;
                gotoxy(oldBullet.position.x,oldBullet.position.y);
                printf(" ");
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





