/*
 * PELCO.C
 */
#include	"camera.h"
#include	"pelco.h"


void
command(unsigned char *m)
{
#ifdef DEBUG
  putch('[');
  putch('@'+m[0]); // CameraID A,B,C,D
  putch(' ');

  // Command
  if (m[2]) { putch(m[2]); }
  else      { putch('S');  }
  putch(' ');
  if (PAN_SPEED > 0x10) { putch('f'); }
  else                  { putch('s');  }

  putch(']');
  putch('\n');
  return;
#else
  unsigned char chksum = 0;
  putch(0xFF);
  *m = CAM;
  for (i=0; i<MSGLENGTH; i++ )
    {
      chksum += *m;
      putch(*m++);
    }
    putch(chksum);
#endif
}

void
check_switches(void)
{ 
   unsigned char newcam = CAM;
   unsigned char newcmd = CMD;
   switch (PORTB & 0x3F)
     {
     case CAM1_SW:      /* UP TO FOUR CAMERAS */
       newcam = CAM1;
       break;
     case CAM2_SW:
       newcam = CAM2;
       break;
     case CAM3_SW:
       newcam = CAM3;
       break;
     case CAM4_SW:
       newcam = CAM4;
       break;

     case UP_SW:          /* PAN AND TILT */
       newcmd = UP;
       break;
     case DOWN_SW:
       newcmd = DOWN;
       break;
     case LEFT_SW:
       newcmd = LEFT;
       break;
     case RIGHT_SW:
       newcmd = RIGHT;
       break;

   // Zooming alters the pan/tilt speed

     case IN_SW:                   /* ZOOM */
       if (speed[CAM] > 0x10) speed[CAM]--;
       newcmd = IN;
       break;

     case OUT_SW:
       if (speed[CAM] < 0x20 ) speed[CAM]++;
       newcmd = OUT;
       break;
  
      default:
	newcmd = 0;
     }

   if (newcam != CAM)
     {
       command(stp);  // Send halt to previous camera
       CAM = newcam;
       RA0 = RA1 = RB6 = RB7 = 1;
       switch(newcam)
	 {
	 case CAM1: RB7 = 0; break;
	 case CAM2: RA1 = 0; break;
	 case CAM3: RA0 = 0; break;
	 case CAM4: RB6 = 0; break;
	 }
     }
   else if (newcmd != CMD)
     {
       CMD = newcmd;
       if (newcmd)
	 {
	   PAN_SPEED = TILT_SPEED = speed[CAM];
	   command(msg);
	   stp[0] = CAM; // Prepare Stop Command for this camera
	 }
       else
	 {
	   command(stp);
	 }
     }
}
