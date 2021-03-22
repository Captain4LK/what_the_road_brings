CC = gcc
CFLAGS+=-lm -Wall

pseudo_3d_racer: main.o segment.o ULK_fixed.o draw.o player.o
	gcc -o pseudo_3d_racer $^ -lSLK -lm -lSDL2 -Wall
	chmod +x pseudo_3d_racer
	mv pseudo_3d_racer ../

main.o: ../../src/main.c ../../src/segment.h ../../src/draw.h ../../src/player.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

segment.o: ../../src/segment.c ../../src/segment.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

ULK_fixed.o: ../../external/ULK_fixed.c ../../external/ULK_fixed.h
	$(CC) -O3 -c $< $(CFLAGS) 

draw.o: ../../src/draw.c ../../src/draw.h ../../src/segment.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

player.o: ../../src/player.c ../../src/player.h ../../src/segment.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

clean:
	rm *.o ../pseudo_3d_racer
