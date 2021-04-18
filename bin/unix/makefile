CC = gcc
CFLAGS+=-Wall -lraylib -lGL -lm -lpthread -lrt -ldl -lX11

pseudo_3d_racer: main.o lil.o track.o audio.o mode.o segment.o texture.o fixed.o car.o game_draw.o player.o menu.o input.o
	gcc -o pseudo_3d_racer $^  $(CFLAGS)
	chmod +x pseudo_3d_racer
	mv pseudo_3d_racer ../

main.o: ../../src/main.c ../../src/audio.h ../../src/mode.h ../../src/segment.h ../../src/car.h ../../src/texture.h ../../src/input.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

lil.o: ../../src/lil.c ../../src/lil.h
	$(CC) -O3 -c $< $(CFLAGS) 

input.o: ../../src/input.c ../../src/input.h ../../src/fixed.h ../../src/config.h ../../src/util.h
	$(CC) -O3 -c $< $(CFLAGS) 

texture.o: ../../src/texture.c ../../src/texture.h ../../src/fixed.h ../../src/config.h ../../src/util.h
	$(CC) -O3 -c $< $(CFLAGS) 

track.o: ../../src/track.c ../../src/track.h ../../src/mode.h ../../src/game_draw.h ../../src/player.h ../../src/texture.h ../../src/lil.h ../../src/segment.h ../../src/car.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

mode.o: ../../src/mode.c ../../src/track.h ../../src/menu.h ../../src/audio.h ../../src/mode.h ../../src/car.h ../../src/texture.h ../../src/segment.h ../../src/game_draw.h ../../src/input.h ../../src/player.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

segment.o: ../../src/segment.c ../../src/segment.h ../../src/car.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

fixed.o: ../../src/fixed.c ../../src/fixed.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

game_draw.o: ../../src/game_draw.c ../../src/game_draw.h ../../src/car.h ../../src/segment.h ../../src/player.h ../../src/texture.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

car.o: ../../src/car.c ../../src/car.h ../../src/audio.h ../../src/texture.h ../../src/game_draw.h ../../src/segment.h ../../src/player.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

player.o: ../../src/player.c ../../src/player.h ../../src/track.h ../../src/texture.h ../../src/input.h ../../src/audio.h ../../src/segment.h ../../src/car.h ../../src/fixed.h ../../src/util.h ../../src/game_draw.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

audio.o: ../../src/audio.c ../../src/audio.h ../../src/mode.h ../../src/segment.h ../../src/car.h ../../src/game_draw.h ../../src/player.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

menu.o: ../../src/menu.c ../../src/menu.h ../../src/track.h ../../src/audio.h ../../src/texture.h ../../src/player.h ../../src/mode.h ../../src/game_draw.h ../../src/input.h ../../src/fixed.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

clean:
	rm *.o ../pseudo_3d_racer
