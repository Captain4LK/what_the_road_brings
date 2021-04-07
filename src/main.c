/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "audio.h"
#include "mode.h"
#include "car.h"
#include "segment.h"
#include "draw.h"
#include "input.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static Color grass1 = {11,31,60,255};
static int fullscreen = 0;
//-------------------------------------

//Function prototypes
static void main_loop();
//-------------------------------------

//Function implementations

int main(int argc, char **arg)
{
   /*srand(time(NULL));
   for(int i = 0;i<50;i++)
   {
      printf("[add_sprite %d %d %d 0]\n",3+i*5,11+rand()%3,(ULK_fixed_32_from_int(1)+(ULK_fixed_32_from_int(1)/3)*(rand()%9)));
   }*/
   //exit(-1);

   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   //SetConfigFlags(FLAG_FULLSCREEN_MODE);
   InitWindow(XRES*2,YRES*2,"What the road brings");
   InitAudioDevice();
   SetTargetFPS(FPS);

   load_assets();
   audio_load();
   clear_texture = grass1;

   dyn_array_init(Segment,&segments,128);

   audio_set_track(0);

#ifndef __EMSCRIPTEN__
   while(!WindowShouldClose())
      main_loop();
#else
   emscripten_set_main_loop(main_loop,0,1);
#endif

   audio_unload();
   CloseWindow();

   return 0;
}

static void main_loop()
{
   if(input_pressed_fullscreen())
   {
      fullscreen = !fullscreen;

      static int main_loop_owidth,main_loop_oheight;
      static int main_loop_ox,main_loop_oy;
      if(fullscreen)
      {
         main_loop_owidth = GetScreenWidth();
         main_loop_oheight = GetScreenHeight();
         main_loop_ox = GetWindowPosition().x;
         main_loop_oy = GetWindowPosition().y;
         SetWindowState(FLAG_WINDOW_UNDECORATED|FLAG_WINDOW_RESIZABLE);
         SetWindowSize(GetMonitorWidth(0),GetMonitorHeight(0));
         SetWindowPosition(0,0);
      }
      else
      {
         SetWindowSize(main_loop_owidth,main_loop_oheight);
         SetWindowPosition(main_loop_ox,main_loop_oy);
         ClearWindowState(FLAG_WINDOW_UNDECORATED);
      }
   }
   
   modes_update();
}
//-------------------------------------
