
all: clean
	gcc sound_detect_konnov.c  -o sound_detect
	g++ light_detect_konnov.c Adafruit_ADS1X15_RPi/Adafruit_ADS1015.cpp -o light_detect_konnov -lwiringPi
clean:
	rm -f *.o
