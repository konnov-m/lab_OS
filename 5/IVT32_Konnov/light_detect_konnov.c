/*******************************************************************************
 * Copyright (c) 2022 Sergey Balabaev (sergei.a.balabaev@gmail.com)                     *
 *                                                                             *
 * The MIT License (MIT):                                                      *
 * Permission is hereby granted, free of charge, to any person obtaining a     *
 * copy of this software and associated documentation files (the "Software"),  *
 * to deal in the Software without restriction, including without limitation   *
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,    *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell   *
 * copies of the Software, and to permit persons to whom the Software is       *
 * furnished to do so, subject to the following conditions:                    *
 * The above copyright notice and this permission notice shall be included     *
 * in all copies or substantial portions of the Software.                      *
 *                                                                             *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR  *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,    *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER      *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,             *
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR       *
 * OTHER DEALINGS IN THE SOFTWARE.                                             *
 ******************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <signal.h>

#include "./Adafruit_ADS1X15_RPi/Adafruit_ADS1015.h"

#define err(mess)                                    \
	{ \ fprintf(stderr, "Error: %s.", mess); \ exit(1); \
	}

//***************************//
#define ADC_PIN 0 // GPIO PIN TRIG
//***************************//
#define THRESHOLD 5000
#define TIME_SIZE 10

Adafruit_ADS1115 ads;

int fd_chan;

void help()
{
	printf("    Use this application for reading from light sensor\n");
	printf("    execute format: ./light_detect [-h][-q] TIME \n");
	printf("    return: ADC value, where 0 - minimum light, 65535 - maximum light\n");
	printf("    TIME - pause between writing in ms\n");
	printf("    -h - help\n");
	printf("    -q - quiet\n");
}

char *time_t_2_char(const time_t time)
{
    size_t i;
    char *str =(char*)  malloc(TIME_SIZE);
    sprintf(str, "%ld\n", time);
    return str;
}

char *uint16_t_2_char(const uint16_t num){

	char *str = (char*)malloc(16);
	sprintf(str, "%d\n", num);
	return str;
}

void Exiting_sig(int sig)
{
	write(fd_chan, "Exit\n", 7);
	exit(0);
}

int main(int argc, char *argv[])
{	
	signal(SIGINT,&Exiting_sig);
	int quiet = 0;
    struct timespec start;
    const char *name_channel = "./light_data";
	uint16_t min_value = 0;
	if (argc <= 1) {
		help();
		return -1;
	} else {
		if ((strcmp(argv[1], "-h") == 0)) {
			help();
			return 0;
		} else {
			if ((strcmp(argv[1], "-q") == 0)) {
				quiet = 1;
			}
            if (argc >= 4) {
               fd_chan = open(name_channel, O_WRONLY);
            }
            if(argc >= 5) {
                min_value = atoi(argv[4]);
            }
        }   
	if (!quiet)
		printf("\nThe lightsensor application was started\n\n");
	ads.setGain(GAIN_TWO);
	ads.begin();
	uint16_t adc0;
	int argument = 1;
	if (quiet)
		argument++;
	uint16_t pause_time = atoi(argv[argument]);
	while (true) {
		adc0 = ads.readADC_SingleEnded(ADC_PIN);
        clock_gettime( CLOCK_REALTIME, &start);
	if (quiet) {

        if (adc0 > min_value){
//            write(fd_chan, uint16_t_2_char(adc0), 5);
//	    	write(fd_chan, "\n", 2);
            write(fd_chan, time_t_2_char(start.tv_sec), TIME_SIZE+1);
//	   	write(fd_chan, "\n", 2); 
//printf("%d\n", adc0);
	    //fflush(stdout);
            }
        } else {
			// printf("ADC: %d\n", adc0);
			// fflush(stdout);
		}
		usleep(pause_time * 1000);
	}
}
}

