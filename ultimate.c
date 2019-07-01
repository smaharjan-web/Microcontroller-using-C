#include <stdio.h>
#include "address_map_nios2.h"
#include "nios2_ctrl_reg_macros.h"


extern volatile int timeout;	// used to synchronize line updates on 250ms
								// intervals set by the intervalTimer_ISR
								 
volatile int pixel_buffer_start = 0x08000000;// VGA pixel buffer
int resolution_x = 80; // VGA screen size
int resolution_y = 60; // VGA screen size

void VGA_text (int, int, char *);
void VGA_box (int, int, int, int, short);
void clear_screen(void);
void draw_hline(int, int, int, int, int);
void draw_box(int, int, int, int, int);
void plot_pixel(int, int, short int);
void put_jtag(volatile int *, char);
void clearscreen(void); 

volatile int * JTAG_UART_ptr = (int *) JTAG_UART_BASE; // JTAG UART address

int data, i;

int main(void) {
int ydir2 = 1;
int ydir1 =1;
int ydir = 1;

int win=0;

int color1;
int color2;
int barrier2 = 0;
int barrier3 = 0;
color2 = 0;

int x1 = 25;
int x2 = x1 + 3;
int y1 = 40; 
int y2 = 43;

int color = 224;

int barrierx1 = 59;
int barrierx2 = 60;
int barriery1 = 10;
int barriery2 = 20;

int barrier2x1 = 59;
int barrier2x2 = 60;
int barrier2y1 = 25;
int barrier2y2 = 35;

int barrier3x1 = 59;
int barrier3x2 = 60;
int barrier3y1 = 40;
int barrier3y2 = 50;

int endwallx1 = 79;
int endwallx2 = 80;
int endwally1 = 0;
int endwally2 = 60;

int xdir = 1;

int barrier22_x1;
int barrier22_x2;
int barrier22_y1;
int barrier22_y2;

int barrier22_x3;
int barrier22_x4;
int barrier22_y3;
int barrier22_y4;

int xc = 54;
int xd = 59;
int yc = 0;
int yd = 60;

int xe;
int xf;
int ye;
int yf;
int barrier33_x1;
int barrier33_x2;
int barrier33_y1;
int barrier33_y2;
int barrier33_y3;
int barrier33_y4;

	int ALT_x1; int ALT_x2; int ALT_y; 
	int ALT_inc_x; int ALT_inc_y;
	int blue_x1; int blue_y1; int blue_x2; int blue_y2;
		
	int screen_x; int screen_y; int char_buffer_x; int char_buffer_y;
	char color4;
	     

clear_screen();
/* read and echo characters */
while(1)
{
	
	volatile int * interval_timer_ptr = (int *) 0x10002000;	// interval timer base address
	volatile int * PS2_ptr = (int *) 0x10000100;			// PS/2 keyboard port address

	color1=224;
	
	timeout = 0; //Start the program with the "update line position now" flag cleared

	int counter = 0x00BEBC20;		// 1/(50 MHz) x (0xBEBC20) = 250 msec	
	//int counter = 0x02FAF080;		// 1/(50 MHz) x (0x02FAF080) = 1sec
	*(interval_timer_ptr + 0x2) = (counter & 0xFFFF);
	*(interval_timer_ptr + 0x3) = (counter >> 16) & 0xFFFF;

	
	*(interval_timer_ptr + 1) = 0x7; // STOP = 0, START = 1, CONT = 1, ITO = 1 
	
    *(PS2_ptr) = 0xFF;		// reset
	*(PS2_ptr + 1) = 0x1; 	//write to the PS/2 Control register to enable interrupts

	NIOS2_WRITE_IENABLE( 0xC3 );/* set interrupt mask bits for levels 0 (interval
								* timer), 1 (pushbuttons), 6 (audio), and 7 (PS/2) */

	NIOS2_WRITE_STATUS( 1 );		// enable Nios II interrupts
	
	
	draw_box(x1,y1,x2,y2,color1);
	draw_box(barrierx1,barriery1,barrierx2,barriery2,color1);	
	draw_box(barrier2x1,barrier2y1,barrier2x2,barrier2y2,color1);
	draw_box(barrier3x1,barrier3y1,barrier3x2,barrier3y2,color1);
	draw_box(endwallx1,endwally1,endwallx2,endwally2,color1);
	data = *(JTAG_UART_ptr); // read the JTAG_UART data register
	
if (data & 0x00008000) // check RVALID to see if there is new data
{
	data = data & 0x000000FF; // the data is in the least significant byte
if (data == 119){ //if "w" key is pressed then object moves upwards
	color = 224;
	printf("\n move up ");
	y1--;
	y2--;
	color2= 0;
	draw_box(x1,y1,x2,y2,color);
	draw_box(barrierx1,barriery1,barrierx2,barriery2,color);	
	draw_box(barrier2x1,barrier2y1,barrier2x2,barrier2y2,color);
	draw_box(barrier3x1,barrier3y1,barrier3x2,barrier3y2,color);
	draw_box(endwallx1,endwally1,endwallx2,endwally2,color);
	draw_box(x1,y1+1,x2,y2+1,color2);
}
else if (data == 97){ // if "a" key is pressed object moves to the left 
	color = 224;
	printf("\n move left ");
	color2 = 0;
	x1--;
	x2--;
	draw_box(x1,y1,x2,y2,color);
	draw_box(barrierx1,barriery1,barrierx2,barriery2,color);
	draw_box(barrier2x1,barrier2y1,barrier2x2,barrier2y2,color);
	draw_box(barrier3x1,barrier3y1,barrier3x2,barrier3y2,color);
	draw_box(endwallx1,endwally1,endwallx2,endwally2,color);
	draw_box(x1+1,y1,x2+1,y2,color2);
}
else if (data == 115){ // if "s" key is pressed then object moves downwards
	color =224;
	printf("\n move down ");
	color2 = 0;
	y1++;
	y2++;
	draw_box(x1,y1,x2,y2,color);
	draw_box(barrierx1,barriery1,barrierx2,barriery2,color);
	draw_box(barrier2x1,barrier2y1,barrier2x2,barrier2y2,color);
	draw_box(barrier3x1,barrier3y1,barrier3x2,barrier3y2,color);
	draw_box(endwallx1,endwally1,endwallx2,endwally2,color);
	draw_box(x1,y1-1,x2,y2-1,color2);
}
else if (data == 100){ // if "d" key is pressed then object starts moving to the right
printf("\n move right ");
printf("\n entered ISR, timeout = %d", timeout);
//clear_screen();
 color1=224;
x1++;
x2++;
draw_box(x1,y1,x2,y2,color1);
draw_box(barrierx1,barriery1,barrierx2,barriery2,color1);
draw_box(barrier2x1,barrier2y1,barrier2x2,barrier2y2,color1);
draw_box(barrier3x1,barrier3y1,barrier3x2,barrier3y2,color1);
draw_box(endwallx1,endwally1,endwallx2,endwally2,color1); 
while (1)
	{
		// Wait for line position update cycle
		while(!timeout)
		{};
	
		// Only gets here when the intervalTimer_ISR 
		// has set global variable timeout = 1 ( "update line position now")
		
		//Erase old objects
		color = 0;	// black
		//draw_hline (x_0, y, x_1, y, color);
		draw_box(x1-1,y1,x2-1,y2,color);
		draw_box(x1,y1,x2,y2,color);
		
		// Change line position. advance each object by 1 pixel in the desired direction.
		x1 = x1 + xdir;
		x2 = x2 + xdir;
		
		if (x2 >= 79) 
		{
			win=win+1;
		break; // stop at the end wall
		}
		
		// Draw new objects
		color1 = 0x3;	// blue
		//draw_hline (x_0, y, x_1, y, color);
		draw_box(x1,y1,x2,y2,color1);
		draw_box(x1,y1,x2,y2,color1);
		draw_box(barrierx1,barriery1,barrierx2,barriery2,color1);
		draw_box(barrier2x1,barrier2y1,barrier2x2,barrier2y2,color1);
		draw_box(barrier3x1,barrier3y1,barrier3x2,barrier3y2,color1);
		draw_box(endwallx1,endwally1,endwallx2,endwally2,color1);
		
		
		 if ( x2 == 59 && y1 >= 10 && y1 <= 20) { /* the top corner of the box touched the first barrier */
        printf ("You touched the first barrier!\n" ); 
		
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		
		/*need to use these coordinates to draw the box in the initial position*/
		draw_box (xc,yc,xd,yd,color2);
		draw_box (x1,y1,x2,y2,color);
		
		/*need to create 2 barriers in front of the first 3 barriers */
		int barrier22_x1 = 50;
		int barrier22_x2 = 51;
		int barrier22_y1 = 10;
		int barrier22_y2 = 20;
		
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color1); /*draws top barrier of the SECOND round*/
		barrier2=1;
		
		int barrier22_x3 = 50;
		int barrier22_x4 = 51;
		int barrier22_y3 = 40;
		int barrier22_y4 = 50;
		
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color1); /*draws bottom barrier of the SECOND round*/
		break;
		
    }
    else if ( x2 == 59 && y2 >= 10 && y2 <= 20 ) { /* the bottom corner of the box touched the first barrier */ 
        printf( "You touched the first barrier of the first round of barriers\n" ); 
		
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		draw_box (xc,yc,xd,yd,color2);
		/*need to use these coordinates to draw the box in the initial position*/
		
		draw_box (x1,y1,x2,y2,color);
		
		/*need to create 2 barriers in front of the first 3 barriers */
		barrier22_x1 = 50;
		barrier22_x2 = 51;
		barrier22_y1 = 10;
		barrier22_y2 = 20;
		
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color1); /*draws top barrier of the SECOND round*/
		barrier2=1;
		barrier22_x3 = 50;
		barrier22_x4 = 51;
		barrier22_y3 = 40;
		barrier22_y4 = 50;
		
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color1); /*draws bottom barrier of the SECOND round*/
		break;
    }
	
	else if ( x2 == 59 && y1 >= 25 && y1 <= 35){ /* the top corner of the box touched the second barrier */
		printf( "You touched the second barrier of the first round of barriers\n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		
		/*need to use these coordinates to draw the box in the initial position*/
		
		draw_box (x1,y1,x2,y2,color);
		draw_box (xc,yc,xd,yd,color2);
		/*need to create 2 barriers in front of the first 3 barriers */
		barrier22_x1 = 50;
		barrier22_x2 = 51;
		barrier22_y1 = 10;
		barrier22_y2 = 20;
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color1); /*draws top barrier of the SECOND round*/
		barrier2=1;

		barrier22_x3 = 50;
		barrier22_x4 = 51;
		barrier22_y3 = 40;
		barrier22_y4 = 50;
		
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color1); /*draws bottom barrier of the SECOND round*/
		break;
	}	
	else if ( x2 == 59 && y2 >= 25 && y2 <= 35){ /* the bottom corner of the box touched the second barrier */
		printf( "You touched the second barrier of the first round of barriers \n" );  
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		draw_box (xc,yc,xd,yd,color2);
		/*need to use these coordinates to draw the box in the initial position*/
		
		draw_box (x1,y1,x2,y2,color);
		
		/*need to create 2 barriers in front of the first 3 barriers */
		barrier22_x1 = 50;
		barrier22_x2 = 51;
		barrier22_y1 = 10;
		barrier22_y2 = 20;
		
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color1); /*draws top barrier of the SECOND round*/
		barrier2=1;
		barrier22_x3 = 50;
		barrier22_x4 = 51;
		barrier22_y3 = 40;
		barrier22_y4 = 50;
		
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color1); /*draws bottom barrier of the SECOND round*/
		break;
	}	
	else if ( x2 == 59 && y1 >= 40 && y1 <= 50){ /* the top corner of the box touched the third barrier */
		printf( "You touched the third barrier of the first round of barriers \n" );  
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		draw_box (xc,yc,xd,yd,color2);
		/*need to use these coordinates to draw the box in the initial position*/
		
		draw_box (x1,y1,x2,y2,color);
		
		/*need to create 2 barriers in front of the first 3 barriers */
		barrier22_x1 = 50;
		barrier22_x2 = 51;
		barrier22_y1 = 10;
		barrier22_y2 = 20;
		
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color1); /*draws top barrier of the SECOND round*/
		barrier2=1;
		barrier22_x3 = 50;
		barrier22_x4 = 51;
		barrier22_y3 = 40;
		barrier22_y4 = 50;
		
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color1); /*draws bottom barrier of the SECOND round*/
		break;
	}	
	else if ( x2 == 59 && y2 >= 40 && y2 <= 50){ /* the bottom corner of the box touched the third barrier */
		printf( "You touched the third barrier of the first round of barriers \n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		draw_box (xc,yc,xd,yd,color2);
		/*need to use these coordinates to draw the box in the initial position*/
		
		draw_box (x1,y1,x2,y2,color);
		
		/*need to create 2 barriers in front of the first 3 barriers */
		barrier22_x1 = 50;
		barrier22_x2 = 51;
		barrier22_y1 = 10;
		barrier22_y2 = 20;
		
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color1); /*draws top barrier of the SECOND round*/
		barrier2=1;
		barrier22_x3 = 50;
		barrier22_x4 = 51;
		barrier22_y3 = 40;
		barrier22_y4 = 50;
		
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color1); /*draws bottom barrier of the SECOND round*/	
		break;
	}
	else if (barrier2==1) // if the second set of barriers is up move second barriers 
  {
		
		//draw_hline (x_0, y, x_1, y, color);
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color); /*draws top barrier of the SECOND round*/
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color); /*draws bottom barrier of the SECOND round*/	
		
		
		// Change line position. advance each object by 1 pixel in the desired direction.
		barrier22_y1 = barrier22_y1 + ydir;
		barrier22_y2 = barrier22_y2 + ydir;
		barrier22_y3 = barrier22_y3 + ydir1;
		barrier22_y4 = barrier22_y4 + ydir1; 
		if ((barrier22_y1 <= 0) || (barrier22_y2 >= 60)) ydir = -ydir;
		if ((barrier22_y3 <= 0) || (barrier22_y4 >= 60)) ydir1 = -ydir1;
		// Draw new objects
		color1 = 0x3;	// blue
		//draw_hline (x_0, y, x_1, y, color);
		draw_box (barrier22_x1, barrier22_y1, barrier22_x2, barrier22_y2, color1); /*draws top barrier of the SECOND round*/
		draw_box (barrier22_x3, barrier22_y3, barrier22_x4, barrier22_y4, color1); /*draws bottom barrier of the SECOND round*/	
	
	
		  if ( barrier2==1 && x2 == barrier22_x1 && y2 >= barrier22_y1 && y2 <= barrier22_y2) // when the bottom of the object hits the first barrier in the second set create third set of barriers
{
		printf( "You touched the first barrier of the second round of barriers \n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		xe = 45;
		xf = 50;
		ye = 0; 
		yf = 60;
		barrier33_x1 = 40;
		barrier33_x2 = 41;
		barrier33_y1 = 25;
		barrier33_y2 = 35;
		
		draw_box (barrier33_x1, barrier33_y1, barrier33_x2, barrier33_y2, color1); /*draws middle barrier of the THIRD round*/
		
		draw_box (x1,y1,x2,y2,color);
		draw_box (xe,ye,xf,yf,color2);
		barrier3=1;
		break;
}
 if ( barrier2==1 && x2 == barrier22_x1 && y1 >= barrier22_y1 && y1 <= barrier22_y2) // when the top of the object hits the first barrier in the second set create third set of barriers
{
		printf( "You touched the first barrier of the second round of barriers \n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		xe = 45;
		xf = 50;
		ye = 0; 
		yf = 60;
		barrier33_x1 = 40;
		barrier33_x2 = 41;
		barrier33_y1 = 25;
		barrier33_y2 = 35;
		
		draw_box (barrier33_x1, barrier33_y1, barrier33_x2, barrier33_y2, color1); /*draws middle barrier of the THIRD round*/
		
		draw_box (x1,y1,x2,y2,color);
		draw_box (xe,ye,xf,yf,color2);
		barrier3=1;
		break;
}

	 if (barrier2==1 && x2 == barrier22_x1 && y2 >= barrier22_y3 && y2 <= barrier22_y4) // when the bottom of the object hits the second barrier in the second set create third set of barriers
	{
		printf( "You touched the second barrier of the second round of barriers \n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		xe = 45;
		xf = 50;
		ye = 0; 
		yf = 60;
		barrier33_x1 = 40;
		barrier33_x2 = 41;
		barrier33_y1 = 25;
		barrier33_y2 = 35;
		
		draw_box (barrier33_x1, barrier33_y1, barrier33_x2, barrier33_y2, color1); /*draws middle barrier of the THIRD round*/
		
		draw_box (x1,y1,x2,y2,color);
		draw_box (xe,ye,xf,yf,color2);
		barrier3=1;
		/*need to create 1 barrier in front of the second 2 barriers */
		
			
		break;
	}	
if (barrier2==1 && x2 == barrier22_x1 && y1 >= barrier22_y3 && y1 <= barrier22_y4) // when the top of the object hits the second barrier in the second set create third set of barriers
	{
		printf( "You touched the second barrier of the second round of barriers \n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		xe = 45;
		xf = 50;
		ye = 0; 
		yf = 60;
		barrier33_x1 = 40;
		barrier33_x2 = 41;
		barrier33_y1 = 25;
		barrier33_y2 = 35;
		
		draw_box (barrier33_x1, barrier33_y1, barrier33_x2, barrier33_y2, color1); /*draws middle barrier of the THIRD round*/
		draw_box (x1,y1,x2,y2,color);
		draw_box (xe,ye,xf,yf,color2);
		
		/*need to create 1 barrier in front of the second 2 barriers */
		
		
		barrier3=1;	
		break;
	}
	else if (barrier3==1) // if the third set of barriers is up then move third set of barriers
  {
		
		//draw_hline (x_0, y, x_1, y, color);
		draw_box (barrier33_x1, barrier33_y1, barrier33_x2, barrier33_y2, color); /*draws top barrier of the SECOND round*/
		
		
		// Change line position. advance each object by 1 pixel in the desired direction.
		barrier33_y1 = barrier33_y1 + ydir2;
		barrier33_y2 = barrier33_y2 + ydir2;
		if ((barrier33_y1 <= 0) || (barrier33_y2 >= 60)) ydir2 = -ydir2;
		// Draw new objects
		color1 = 0x3;	// blue
		//draw_hline (x_0, y, x_1, y, color);
		draw_box (barrier33_x1, barrier33_y1, barrier33_x2, barrier33_y2, color1); /*draws top barrier of the SECOND round*/
		
	
	
	 if (barrier3==1 && x2 == barrier33_x1 && y1 >= barrier33_y1 && y1 <= barrier33_y2) // when the top of the object hits the barrier in the third set end game
	{
		printf( "You touched the second barrier of the second round of barriers \n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		xe = 45;
		xf = 50;
		ye = 0; 
		yf = 60;
		
		draw_box (x1,y1,x2,y2,color);
		draw_box (xe,ye,xf,yf,color2);
	
		 char buffer[20], buffer2[20];
               strcpy(buffer, "Score = "); //string to buffer
               sprintf(buffer2, "%d", win); // conversion integer 
               strcat(buffer, buffer2);  //append buffer2 to buffer
               char* text_bottom_VGA = buffer;
               VGA_text (ALT_x1+10, ALT_y, text_bottom_VGA);
	screen_x = 79; screen_y = 59;
	color4 = 0;		// black
    VGA_box (0, 0, screen_x, screen_y, color4);	 /* clear the screen */
	
		/*need to create 1 barrier in front of the second 2 barriers */
			// draw a blue box around the above text
	blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;
	
	// character coords are the same as pixel buffer coords
	color4 = 0x3;		// blue 
	VGA_box (blue_x1, blue_y1, blue_x2, blue_y2, color4);
	
	/* output text message in the middle of the VGA monitor */
	VGA_text (blue_x1 + 5, blue_y1 + 4, text_bottom_VGA);



		
		break;
	}
	 if (barrier3==1 && x2 == barrier33_x1 && y2 >= barrier33_y1 && y2 <= barrier33_y2) // when the bottom of the object hits the  barrier in the third set end game
	{
		printf( "You touched the second barrier of the second round of barriers \n" ); 
		x1 =0;
		x2 =3;
		y1 =30;
		y2 =33;
		xe = 45;
		xf = 50;
		ye = 0; 
		yf = 60;
		
		draw_box (x1,y1,x2,y2,color);
		draw_box (xe,ye,xf,yf,color2);
	
 char buffer[20], buffer2[20];
               strcpy(buffer, "Score = "); //string to buffer
               sprintf(buffer2, "%d", win); // conversion integer 
               strcat(buffer, buffer2);  //append buffer2 to buffer
               char* text_bottom_VGA = buffer;
               VGA_text (ALT_x1+10, ALT_y, text_bottom_VGA);
	screen_x = 79; screen_y = 59;
	color4 = 0;		// black
    VGA_box (0, 0, screen_x, screen_y, color4);	 /* clear the screen */
	
		/*need to create 1 barrier in front of the second 2 barriers */
			// draw a blue box around the above text
	blue_x1 = 28; blue_x2 = 52; blue_y1 = 26; blue_y2 = 34;
	
	// character coords are the same as pixel buffer coords
	color4 = 0x3;		// blue 
	VGA_box (blue_x1, blue_y1, blue_x2, blue_y2, color4);
	
	/* output text message in the middle of the VGA monitor */
	VGA_text (blue_x1 + 5, blue_y1 + 4, text_bottom_VGA);



		
		break;
	}
	
  }
	
  }

	
  
		
		
		timeout = 0; //Clear the "update line position now" flag 

	}

}
if (x2 >= 79) // if the object hits the goal you win the game
	{
		 char buffer[20], buffer2[20];
               strcpy(buffer, "Score = "); //string to buffer
               sprintf(buffer2, "%d", win); // conversion integer 
               strcat(buffer, buffer2);  //append buffer2 to buffer
               char* text_bottom_VGA = buffer;
               VGA_text (ALT_x1+10, ALT_y, text_bottom_VGA);

		color1=224;
		x1 = 25;
		x2 = 28;
		y1 = 40;
		y2 = 43; 
		draw_box(x1,y1,x2,y2,color1);
		draw_box(barrierx1,barriery1,barrierx2,barriery2,color1);
		draw_box(barrier2x1,barrier2y1,barrier2x2,barrier2y2,color1);
		draw_box(barrier3x1,barrier3y1,barrier3x2,barrier3y2,color1);
		draw_box(endwallx1,endwally1,endwallx2,endwally2,color1);
	screen_x = 79; screen_y = 59;
	color4 = 0;		// black
    VGA_box (0, 0, screen_x, screen_y, color4);	 /* clear the screen */
	clear_screen();

	}	

/* echo the character */
put_jtag (JTAG_UART_ptr, (char) data & 0xFF );
}
}

}







/* Function to blank the VGA screen */
void clear_screen( )
{
	int y, x;
	int pixel_ptr;

	for (y = 0; y < resolution_y; y++)
	{
		for (x = 0; x < resolution_x; x++)
		{
			volatile char * pixel_ptr = (volatile char *) (pixel_buffer_start + (y << 7)+ x);
			*(pixel_ptr) = 0;	// clear pixel- background color is set to black
			pixel_ptr += 1;		// Next
//			pixel_ptr += 2;	 For DE0-CV increment by 2 because each pixel takes two byte addresses			
		}
	}
}




/* Line drawing or Box shading algorithm.  This function draws a line between points (x0, y0)
 * and (x1, y1). One ROW at a time. The function assumes that the coordinates are valid
 * 
 */
 
void draw_box(int x0, int y0, int x1, int y1, int color)
{
	int y;
	int x;
	
	for(y=y0;y<y1;y++)
	{
		for(x=x0;x<x1;x++)
		{
			plot_pixel(x,y,color);
		}
	}
}

/* Draw Horizontal Line */
void draw_hline(int x0, int y0, int x1, int y1, int color)
{
	int y=y0;
	int x;
	
	for(x=x0;x<x1;x++)
	{
		plot_pixel(x,y,color);
	}
}


void plot_pixel(int x, int y, short int line_color)
{
	*(volatile char *)(pixel_buffer_start + (y << 7)+x) = line_color;
	// *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color; For DE0-CV
}

void put_jtag( volatile int * JTAG_UART_ptr, char c )
{
int control;
control = *(JTAG_UART_ptr + 1); // read the JTAG_UART control register
if (control & 0xFFFF0000) // if space, then echo character, else ignore 
*(JTAG_UART_ptr) = c;
}


/****************************************************************************************
 * Draw a filled rectangle on the VGA monitor 
****************************************************************************************/
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)
{
	int offset, row, col;
  	volatile char * pixel_buffer = (char *) 0x08000000;	// VGA pixel buffer

	/* assume that the box coordinates are valid */
	for (row = y1; row <= y2; row++)
	{
		col = x1;
		while (col <= x2)
		{
			offset = (row << 7) + col;
			*(pixel_buffer + offset) = pixel_color;	
			++col;
		}
	}
}






/****************************************************************************************
 * Subroutine to send a string of text to the VGA monitor 
****************************************************************************************/
void VGA_text(int x, int y, char * text_ptr)
{
	int offset;
  	volatile char * character_buffer = (char *) 0x09000000;	// VGA character buffer

	/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while ( *(text_ptr) )
	{
		*(character_buffer + offset) = *(text_ptr);	// write to the character buffer
		++text_ptr;
		++offset;
	}
}
