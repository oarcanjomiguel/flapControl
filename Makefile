CC=gcc
CFLAGS= -O3 -Wall -g
LIBS= -lm -lwiringPi -lwiringPiDev -lwiringPiPca9685

flapControl: example.c
	$(CC) -o example ${LIBS}

# gcc -Wall example.c -o example -lwiringPi -lwiringPiDev -lwiringPiPca9685 -lm 