OBJS = test.o waveread_write.o
CC = gcc
CFLAGS = -Wall -O -g -D HAVE_CONFIG_H
LD = /usr/local/lib/libspeexdsp.a
INCLUDE_PATH = /usr/local/include/speex

test : $(OBJS) waveread_write.h
	$(CC) $(OBJS) $(LD) -lm -I$(INCLUDE_PATH) -o test

test.o : test.c waveread_write.h
	$(CC) $(CFLAGS) -I$(INCLUDE_PATH) -c test.c
	
waveread_write.o : waveread_write.c waveread_write.h
	gcc -c waveread_write.c
clean:
	rm -rf *.o test