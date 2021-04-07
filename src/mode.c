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
#include "track.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int mode = 0;
Color clear_texture = WHITE;
Color clear_screen = BLACK;

static int pause_select = 0;
//-------------------------------------

//Function prototypes
static void mode_title_update();
static void mode_credits_update();
static void mode_game_start_update();
static void mode_game_update();
static void mode_pause_update();
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
      mode_game_start_update();
      break;
   case 12:
      mode_game_update();
      break;
   case 13:
      mode_pause_update();
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

static void mode_game_start_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   cars_update(0);

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

   if(!IsSoundPlaying(sound_countdown_0))
   {
      audio_set_track(track.music);
      PlaySound(sound_countdown_1);
      mode = 12;
   }
}

static void mode_game_update()
{
   UpdateMusicStream(sound_drive);
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   player_update();
   cars_update(1);

   if(IsKeyPressed(KEY_ENTER))
   {
      if(music_current!=NULL)
         PauseMusicStream(*music_current);
      mode = 13;
      pause_select = 0;
      enable_parallax = 0;
   }

   BeginDrawing();
      ClearBackground(clear_screen);
      BeginTextureMode(texture_viewport);
         ClearBackground(clear_texture);
         draw(player.px,player.pz,player.steer);
         DrawTextEx(font,TextFormat("Lap\n %02d",player.lap),(Vector2){4,4},font.baseSize,0.0f,WHITE);
         DrawTextEx(font,TextFormat("Speed\n %03d",(int)(((float)player.vz/(float)MAX_SPEED)*200.0f)),(Vector2){147,4},font.baseSize,1.0f,WHITE);
         DrawTextEx(font,TextFormat("Pos\n %01d",player_pos()),(Vector2){301,4},font.baseSize,1.0f,WHITE);
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

static void mode_pause_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   if(IsKeyPressed(KEY_UP)&&pause_select>0)
      pause_select--;
   if(IsKeyPressed(KEY_DOWN)&&pause_select<2)
      pause_select++;
   if(IsKeyPressed(KEY_ENTER))
   {
      switch(pause_select)
      {
      case 0: mode = 12; enable_parallax = 1; if(music_current!=NULL) ResumeMusicStream(*music_current); break;
      case 1: mode = 11; track_build(); if(music_current!=NULL) StopMusicStream(*music_current); StopSound(sound_countdown_1); PlaySound(sound_countdown_0); break;
      case 2: mode = 0; break;
      }
   }

   BeginDrawing();
      ClearBackground(clear_screen);
      BeginTextureMode(texture_viewport);
         ClearBackground(clear_texture);
         draw(player.px,player.pz,player.steer);
         DrawTextEx(font,TextFormat("Lap\n %02d",player.lap),(Vector2){4,4},font.baseSize,0.0f,WHITE);
         DrawTextEx(font,TextFormat("Speed\n %03d",(int)(((float)player.vz/(float)MAX_SPEED)*200.0f)),(Vector2){147,4},font.baseSize,1.0f,WHITE);
         DrawTextEx(font,TextFormat("Pos\n %01d",player_pos()),(Vector2){301,4},font.baseSize,1.0f,WHITE);
         DrawTextEx(font,"Resume",(Vector2){145,96},font.baseSize,0.0f,WHITE);
         DrawTextEx(font,"Restart",(Vector2){142,128},font.baseSize,0.0f,WHITE);
         DrawTextEx(font,"Quit",(Vector2){150,160},font.baseSize,0.0f,WHITE);
         switch(pause_select)
         {
         case 0: DrawTextEx(font,">      <",(Vector2){140,96},font.baseSize,0.0f,WHITE); break;
         case 1: DrawTextEx(font,">       <",(Vector2){137,128},font.baseSize,0.0f,WHITE); break;
         case 2: DrawTextEx(font,">    <",(Vector2){145,160},font.baseSize,0.0f,WHITE); break;
         }
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
