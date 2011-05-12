    /*
 * PELCO.H
 */

#ifdef DEBUG  // TEXT VERSIONS OF IDENTIFIERS FOR DEBUGGING

#define LEFT  'L'
#define RIGHT 'R'
#define UP    'U'
#define DOWN  'D'
#define IN    'I'
#define OUT   'O'

#else         // ACTUAL VALUES USED FOR PELCO-D PROTOCOL

#define LEFT  0x04     // PELCO COMMANDS
#define RIGHT 0x02
#define UP    0x08
#define DOWN  0x10
#define IN    0x20
#define OUT   0x40

#endif

#define LEMUR 1

#ifdef LEMUR
#define CAM1  1
#define CAM2  7
#define CAM3  3
#define CAM4  2
#else
#define CAM1  3    // CAMERA IDENTIFIERS
#define CAM2  7
#define CAM3  1
#define CAM4  2
#endif

#define CAM1_SW  0x1E  // ACTIVE-LOW INPUT SWITCHES
#define CAM2_SW  0x2E  // 1011 10
#define CAM3_SW  0x0E  // 0011 10
#define CAM4_SW  0x0A  // 0010 10

#define UP_SW    0x0D  // 0011 01
#define DOWN_SW  0x09  // 0010 01
#define LEFT_SW  0x1D  // 0111 01
#define RIGHT_SW 0x2D  // 1011 01

#define IN_SW    0x30  // 1100 00
#define OUT_SW   0x34  // 1101 00


unsigned char i;
 // Pan/Tilt speed for each camera
unsigned char speed[4] = { 0x20, 0x20, 0x20, 0x20 };

unsigned char msg[5] = { CAM1, 0x00, 0x00, 0x20, 0x20 };
unsigned char stp[5] = { CAM1, 0x00, 0x00, 0x20, 0x20 };

#define MSGLENGTH   5

#define CAM        msg[0]
#define CMD        msg[2]
#define PAN_SPEED  msg[3]
#define TILT_SPEED msg[4]







