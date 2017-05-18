text_wave : text_wave.o waveread_write.o
	gcc -o -g text_wave  text_wave.o waveread_write.o
text_wave.o : text_wave.c waveread_write.h
	gcc -c text_wave.c
waveread_write.o : waveread_write.c waveread_write.h
	gcc -c waveread_write.c

clean :
	rm text_wav  *.o  
