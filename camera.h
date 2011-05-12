/*
 * CAMERA.H
 */
/* #define DEBUG 1 */

#include <htc.h>
#include <conio.h>

#define LENGTH(a) (sizeof(a)/sizeof((a)[0]))

void check_switches(void);
void sleep(int);
void putch(char c);
void initialize(void);
void command(unsigned char *);

unsigned char msg[];
unsigned char stp[];

