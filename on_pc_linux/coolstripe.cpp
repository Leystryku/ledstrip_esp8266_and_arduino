
#include <stdio.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <termios.h>
#include <stdint.h>
#include "leynet.h"


#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <cstdio>

#include <errno.h>
 

int main (int argc, char *argv[])
{

/*
	if(!argv[1])
	{
		printf("enter the numbers of leds!\n");
	}
	
	if(!argv[2])
	{
		printf("enter the ip!\n");
		return 0;
	}*/

	int fd = open("/dev/ttyACM0", O_RDWR);
    if(!fd)
    {
    	printf("Arduino not found!\n");

		exit( EXIT_FAILURE );
    }



	Display *display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);

	XWindowAttributes gwa;

	XGetWindowAttributes(display, root, &gwa);
	int width = gwa.width;
	int height = gwa.height;


uint8_t lastupdate_r = 0;
uint8_t lastupdate_g = 0;
uint8_t lastupdate_b = 0;
		char data[3] = {0};
		write(fd, data, 3);



        usleep(1000*500);
    while(1)
    {



		XImage *image = XGetImage(display,root, 0,0 , width,height,AllPlanes, ZPixmap);

		if(!image)
			continue;


		unsigned long red_mask = image->red_mask;
		unsigned long green_mask = image->green_mask;
		unsigned long blue_mask = image->blue_mask;


		uint64_t overall_r = 0;
		uint64_t overall_g = 0;
		uint64_t overall_b = 0;

		uint64_t pixelcount = 0;

	   for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height ; y++)
			{
				pixelcount = pixelcount + 1;
				unsigned long pixel = XGetPixel(image,x,y);

				unsigned char blue = pixel & blue_mask;
				unsigned char green = (pixel & green_mask) >> 8;
				unsigned char red = (pixel & red_mask) >> 16;


				unsigned long disttowhite = 0xFFFFFF - (pixel&0xFFFFFF);

				if(!disttowhite)
					continue;

				unsigned long disttoblack = (pixel&0xFFFFFF);

				if(!disttoblack)
					continue;
				
				if(!pixel)
					continue;

				overall_r += red;
				overall_g += green;
				overall_b += blue;
			}

		}

		XDestroyImage(image);

    	uint8_t avg_r = (uint8_t)(overall_r / pixelcount);
    	uint8_t avg_g = (uint8_t)(overall_g / pixelcount); 
    	uint8_t avg_b = (uint8_t)(overall_b / pixelcount);


		if(avg_r == '\n')
			avg_r = avg_r - 1;

		if(avg_g == '\n')
			avg_g = avg_g - 1;

		if(avg_b == '\n')
			avg_b = avg_b - 1;

		uint8_t tolerance = 7;

		if(abs(lastupdate_r - avg_r) < tolerance && abs(lastupdate_g - avg_g) < tolerance && abs(lastupdate_b - avg_b) < tolerance)
		{
			continue;
		}

		char data[3] = {0};
		data[0] = avg_r;
		data[1] = avg_g;
		data[2] = avg_b;

		lastupdate_r = avg_r;
		lastupdate_g = avg_g;
		lastupdate_b = avg_b;

		write(fd, data, 3);

    	//fprintf(file,"%c",avg_r);
    	//fprintf(file,"%c",avg_g);
    	//fprintf(file,"%c",avg_b);

    	//fprintf(file,"%c",'\n');

    	printf("%d __ %d __ %d\n", avg_r, avg_g, avg_b);


        usleep(1000*6);
    }

   	XCloseDisplay(display);


	return 0;
}
