/*
 * MAIN.C
 */

#include "camera.h"

#define LENGTH(a) (sizeof(a)/sizeof((a)[0]))

#ifdef DEBUG
char text[13] = "Hey KayJay.\n";

int flash;
void
flasher(void)
{
  flash++;
  if (flash % 300 == 0)
    {
       RA0 = 1;
       sleep(400);
       RA0 = 0;
    }
}
#endif

void
sleep(int c)
{
int i,j,loop;
   while(c--)
     {
       loop = 50;
       for (i=0; i < loop; i++)
	 {
	       j = i;
	 }
     }
}



void
main(void)
{
  initialize();
  while(1)
    {
      check_switches();
      sleep(50);

#ifdef DEBUG
      flasher();
#endif

    }
}
