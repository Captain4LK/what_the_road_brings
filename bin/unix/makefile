CC = gcc
CFLAGS+=-Wall -lraylib -lGL -lm -lpthread -lrt -ldl -lX11

pseudo_3d_racer: main.o lil.o track.o audio.o mode.o segment.o ULK_fixed.o car.o draw.o player.o menu.o input.o
	gcc -o pseudo_3d_racer $^  $(CFLAGS)
	chmod +x pseudo_3d_racer
	mv pseudo_3d_racer ../

main.o: ../../src/main.c ../../src/audio.h ../../src/mode.h ../../src/segment.h ../../src/car.h ../../src/draw.h ../../src/util.h ../../src/input.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

lil.o: ../../src/lil.c ../../src/lil.h
	$(CC) -O3 -c $< $(CFLAGS) 

input.o: ../../src/input.c ../../src/input.h ../../src/config.h ../../src/util.h
	$(CC) -O3 -c $< $(CFLAGS) 

track.o: ../../src/track.c ../../src/track.h ../../src/player.h ../../src/lil.h ../../src/segment.h ../../src/car.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

mode.o: ../../src/mode.c ../../src/track.h ../../src/menu.h ../../src/audio.h ../../src/mode.h ../../src/car.h ../../src/segment.h ../../src/draw.h ../../src/input.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

segment.o: ../../src/segment.c ../../src/segment.h ../../src/car.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

ULK_fixed.o: ../../src/ULK_fixed.c ../../src/ULK_fixed.h
	$(CC) -O3 -c $< $(CFLAGS) 

draw.o: ../../src/draw.c ../../src/draw.h ../../src/car.h ../../src/segment.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

car.o: ../../src/car.c ../../src/car.h ../../src/audio.h ../../src/draw.h ../../src/segment.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

player.o: ../../src/player.c ../../src/player.h ../../src/input.h ../../src/audio.h ../../src/segment.h ../../src/car.h ../../src/util.h ../../src/draw.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

audio.o: ../../src/audio.c ../../src/audio.h ../../src/mode.h ../../src/segment.h ../../src/car.h ../../src/draw.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

menu.o: ../../src/menu.c ../../src/menu.h ../../src/track.h ../../src/audio.h ../../src/player.h ../../src/mode.h ../../src/draw.h ../../src/input.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

clean:
	rm *.o ../pseudo_3d_racer
