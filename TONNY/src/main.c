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
    moveShip();

    color(15,0);
    clrscr();
    //window(2, 2, 139, 39);

    awakenSqwog(1);
    awakenSqwog(1);
    awakenSqwog(1);
    awakenSqwog(1);
    awakenSqwog(1);


    fixtrangPos(&trang);
    trangNextPos(&trang);
    drawTrang(trang);

    while(1){}
        //printf("%c",uart_get_char());
}
