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

int main(void)
{
    uart_init (9600);

    //HeJSa

    hello();

  while(1)
  {

  }
}
