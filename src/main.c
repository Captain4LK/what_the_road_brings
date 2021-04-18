/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdlib.h>
#include <stdint.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "config.h"
#include "util.h"
#include "fixed.h"
#include "audio.h"
#include "mode.h"
#include "car.h"
#include "segment.h"
#include "input.h"
#include "texture.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
static void main_loop();
//-------------------------------------

//Function implementations

int main(int argc, const char **argv)
{
   //Initialize raylib
   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   InitWindow(XRES*2,YRES*2,TITLE);

   //Load ressources
   dyn_array_init(Segment,&segments,128);
   textures_load();
   audio_load();
   audio_set_track(0);

#ifndef __EMSCRIPTEN__
   SetTargetFPS(FPS);
   while(!WindowShouldClose())
      main_loop();
#else
   emscripten_set_main_loop(main_loop,FPS,1);
#endif

   audio_unload();
   CloseWindow();

   return 0;
}

static void main_loop()
{
   if(input_pressed_fullscreen())
   {
      static int main_loop_fullscreen = 0;
      static int main_loop_owidth,main_loop_oheight;
      static int main_loop_ox,main_loop_oy;

      main_loop_fullscreen = !main_loop_fullscreen;
      if(main_loop_fullscreen)
      {
         main_loop_owidth = GetScreenWidth();
         main_loop_oheight = GetScreenHeight();
         main_loop_ox = GetWindowPosition().x;
         main_loop_oy = GetWindowPosition().y;
         SetWindowState(FLAG_WINDOW_UNDECORATED);
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
   
   mode_update();
}
//-------------------------------------
