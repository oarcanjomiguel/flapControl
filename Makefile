CC=gcc
CFLAGS= -O3 -Wall -g
LIBS= -lm -lwiringPi -lwiringPiDev -lwiringPiPca9685

example: example.o
	$(CC) -o example example.o ${LIBS}

# gcc -Wall example.c -o example -lwiringPi -lwiringPiDev -lwiringPiPca9685 -lm 