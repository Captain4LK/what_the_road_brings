CC = emcc 
CFLAGS+= -Wall -flto

pseudo_3d_racer: main.o mode.o segment.o ULK_fixed.o car.o draw.o player.o sdl.o
	emcc -O3 $^ -lm -flto -Wall -s USE_SDL=2 --preload-file ../assets/ -o racer.html --shell-file shell_minimal.html

main.o: ../../src/main.c ../../src/mode.h ../../src/segment.h ../../src/car.h ../../src/draw.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

mode.o: ../../src/mode.c ../../src/mode.h ../../src/car.h ../../src/segment.h ../../src/draw.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

segment.o: ../../src/segment.c ../../src/segment.h ../../src/car.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

ULK_fixed.o: ../../src/ULK_fixed.c ../../src/ULK_fixed.h
	$(CC) -O3 -c $< $(CFLAGS) 

draw.o: ../../src/draw.c ../../src/draw.h ../../src/car.h ../../src/segment.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

car.o: ../../src/car.c ../../src/car.h ../../src/draw.h ../../src/segment.h ../../src/player.h ../../src/util.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

player.o: ../../src/player.c ../../src/player.h ../../src/segment.h ../../src/car.h ../../src/util.h ../../src/draw.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

sdl.o: ../../src/sdl.c ../../src/sdl.h ../../src/config.h
	$(CC) -O3 -c $< $(CFLAGS) 

clean:
	rm *.o 
