#include "game.h"
#include "string.h"

void initShip(struct ship_t *ship, int32_t x, int32_t y){
    (*ship).position.x=x;
    (*ship).position.y=y;
}

void initAsteroid(struct asteroid_t *asteroid, int32_t x, int32_t y, int32_t vx, int32_t vy){
    (*asteroid).position.x=x;
    (*asteroid).position.y=y;
    (*asteroid).velocity.x=vx;
    (*asteroid).velocity.y=vy;

}

void initEnemy(struct enemy *e, uint8_t type, int32_t x, int32_t y){
    (*e).enemyType=type;
    (*e).position.x=x;
    (*e).position.y=y;
    (*e).firstx=(*e).position.x;
    (*e).firsty=(*e).position.y;
    if ((*e).enemyType == 1) {
        (*e).velocity.x = -1;
        (*e).velocity.y = 1;
    } else if ((*e).enemyType == 2) {
        (*e).velocity.x = -1;
        (*e).velocity.y = 0;
    }
}



//Compares asteroid and bullet positions
uint8_t compBuAs(struct bullet_t bullet, struct asteroid_t asteroid){
//if collision is detected, 1 is returned
    uint8_t g=0;
    if ((bullet.position.x==asteroid.position.x-1 || bullet.position.x==asteroid.position.x) && bullet.position.y==asteroid.position.y){
        g=1;
    }

    return g;
}

//Compares enemy and bullet positions
uint8_t compBuEn(struct bullet_t bullet, struct enemy e){
    //if collision is detected, 1 is returned
    uint8_t g=0;
    if ((bullet.position.x==e.position.x ||
         bullet.position.x==e.position.x + 1 ||
         bullet.position.x==e.position.x + 2 ||
         bullet.position.x==e.position.x - 1 ||
         bullet.position.x==e.position.x - 1)
        &&
        (bullet.position.y==e.position.y ||
        bullet.position.y==e.position.y+1 ||
        bullet.position.y==e.position.y+2 ||
        bullet.position.y==e.position.y-1 ||
        bullet.position.y==e.position.y-2)){
    g=1;
    }
    return g;
}

//Compares nets and ship positions
uint8_t compDoSh(struct ship_t ship, struct asteroid_t dodge){
    //if collision is detected, 1 is returned
    uint8_t g=0;
    if (ship.position.x==dodge.position.x-1 && (ship.position.y==dodge.position.y ||
                                                ship.position.y==dodge.position.y+1 ||
                                                ship.position.y==dodge.position.y-1 )){
        g=1;
    }
    return g;
}

//Checks if new score is higher than previous highscore and updates it in the flash memory
uint16_t updateHighscore (uint16_t highscore, uint16_t score){
//input:current highscore, current score
    //reading from flash:
    highscore = *(uint16_t *)(0x0800F800);
    if(score > highscore){
        highscore = score;
            //unlock flash:
        FLASH_Unlock();
        FLASH_ClearFlag( FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR );
        FLASH_ErasePage(0x0800F800);//erase page
        FLASH_ProgramHalfWord(0x0800F800,highscore);//write data
        FLASH_Lock();//lock flash
       }
    return highscore;
}

//Detects if enemy has passed spaceship
uint8_t enemyBreach(struct enemy e){
//returns TRUE, if enemy passes TONNY
//input: enemy structure
    uint8_t breach=0;
    if (e.position.x<6){
        breach=1;
    }
    return breach;
}
