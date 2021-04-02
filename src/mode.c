/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <math.h>
#include <time.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "mode.h"
#include "player.h"
#include "car.h"
#include "draw.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int mode = 0;
Color clear_texture = WHITE;
Color clear_screen = BLACK;
//-------------------------------------

//Function prototypes
static void game_update();
//-------------------------------------

//Function implementations

void modes_update()
{
   switch(mode)
   {
   case 0: //Title screen

      break;
   }
   game_update();
}

static void game_update()
{
   player_update();
   cars_update();

   BeginDrawing();
      ClearBackground(clear_screen);
      BeginTextureMode(texture_viewport);
         ClearBackground(clear_texture);
         draw(player.px,player.pz,player.steer);
      EndTextureMode();

      float window_width = GetScreenWidth();
      float window_height = GetScreenHeight();
      Rectangle rect;
      if((float)window_width/(4.0f/3.0f)>=window_height)
      {
         rect.width = window_height*(4.0f/3.0f);
         rect.height = window_height;
      }
      else
      {
         rect.width = window_width;
         rect.height = window_width*(3.0f/4.0f);
      }
      rect.x = (window_width-rect.width)/2;
      rect.y = (window_height-rect.height)/2;
      DrawTexturePro(texture_viewport.texture,(Rectangle){0,0,(float)texture_viewport.texture.width,(float)-texture_viewport.texture.height},rect,(Vector2){0,0},0.0f,WHITE);

   EndDrawing();
}
//-------------------------------------
