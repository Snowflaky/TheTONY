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
    window();

    struct trang trang;
    trang.x = 130;
    trang.y = 25;
    trang.vx = -2;
    trang.vy = -1;
    trang.hp = 5;

    fixtrangPos(&trang);
    trangNextPos(&trang);
    drawTrang(trang);

  while(1){}
}
