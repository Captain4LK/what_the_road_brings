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
#include "audio.h"
#include "mode.h"
#include "player.h"
#include "car.h"
#include "draw.h"
#include "menu.h"
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
static void mode_title_update();
static void mode_credits_update();
static void mode_game_update();
//-------------------------------------

//Function implementations

void modes_update()
{
   switch(mode)
   {
   case 0: //Title screen
      mode_title_update();
      break;
   case 1: //Credits
      mode_credits_update();
      break;
   case 11:
      mode_game_update();
      break;
   }
}

static void mode_title_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);
   title_update();

   BeginDrawing();
      ClearBackground(clear_screen);
      BeginTextureMode(texture_viewport);
         ClearBackground(clear_texture);
         title_draw();
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

static void mode_credits_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);
   credits_update();

   BeginDrawing();
      ClearBackground(clear_screen);
      BeginTextureMode(texture_viewport);
         ClearBackground(clear_texture);
         credits_draw();
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

static void mode_game_update()
{
   UpdateMusicStream(sound_drive);
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   player_update();
   cars_update();

   BeginDrawing();
      ClearBackground(clear_screen);
      BeginTextureMode(texture_viewport);
         ClearBackground(clear_texture);
         draw(player.px,player.pz,player.steer);
         DrawText(TextFormat("Lap\n%02d",player.lap),4,4,1,WHITE);
         //DrawText(TextFormat("      Time\n%02d:%02d:%03d",(ULK_fixed_32_to_int(player.time)/60),ULK_fixed_32_to_int(player.time)%60,ULK_fixed_32_to_int(1000*(player.time%ULK_fixed_32_from_int(1)))),270,4,1,WHITE);
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
