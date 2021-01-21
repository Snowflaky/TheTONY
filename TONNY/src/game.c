#include "game.h"
#include "string.h"



//Compares asteroid and bullet positions
uint8_t compBuAs(struct bullet_t bullet, struct asteroid_t asteroid){
//if collision is detected, 1 is returned
    uint8_t g=0;
    if (bullet.position.x==asteroid.position.x-1 && bullet.position.y==asteroid.position.y){
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

//Checks if new score is higher than previous highscore and updates it
uint16_t updateHighscore (uint16_t highscore, uint16_t score){
//input:current highscore, current score
    if(score > highscore){
       highscore = score;
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
