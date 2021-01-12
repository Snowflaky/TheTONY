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

    color(0,0);
    clrscr();
    //window();

    struct trang tra;
    tra.position.x = 110;
    tra.position.y = 21;
    tra.velocity.x = -2;
    tra.velocity.y = 0;
    tra.hp = 5;

    //fixtrangPos(&tra);

    for(int i = 0; i < 20; i++) {
        trangNextPos(&tra);
        drawTrang(tra);
        eraseTrang(tra);
    }

    while(1){}
}
