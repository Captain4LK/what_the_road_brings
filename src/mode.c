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
#include "fixed.h"
#include "config.h"
#include "util.h"
#include "audio.h"
#include "mode.h"
#include "player.h"
#include "car.h"
#include "game_draw.h"
#include "menu.h"
#include "track.h"
#include "texture.h"
#include "input.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int mode = 0;
Color clear_texture = WHITE;

static int pause_select = 0;
static float results_alpha = 0;
static int results_pos = 0;
static int results_select = 0;
//-------------------------------------

//Function prototypes
static void mode_title_update();
static void mode_credits_update();
static void mode_game_start_update();
static void mode_game_update();
static void mode_game_draw();
static void mode_pause_update();
static void mode_pause_draw();
static void mode_track_sel_update();
static void mode_results_update();
static void mode_results_draw();
//-------------------------------------

//Function implementations

void mode_update()
{
   //Game logic
   int old_mode = mode;
   switch(mode)
   {
   case 0: mode_title_update(); break;
   case 1: mode_credits_update(); break;
   case 2: mode_track_sel_update(); break;
   case 11: mode_game_start_update(); break;
   case 12: mode_game_update(); break;
   case 13: mode_pause_update(); break;
   case 14: mode_results_update(); break;
   }

   //Drawing
   BeginDrawing();
      ClearBackground(BLACK);
      BeginTextureMode(texture_viewport);
         ClearBackground(clear_texture);
         switch(old_mode)
         {
         case 0: title_draw(); break;
         case 1: credits_draw(); break;
         case 2: track_sel_draw(); break;
         case 11: game_draw(player.px,player.pz,player.steer); break;
         case 12: mode_game_draw(); break;
         case 13: mode_pause_draw(); break;
         case 14: mode_results_draw(); break;
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

static void mode_title_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);
   title_update();
}

static void mode_credits_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);
   credits_update();
}

static void mode_track_sel_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);
   track_sel_update();
}

static void mode_game_start_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   cars_update(0);

   if(!IsSoundPlaying(sound_countdown_0))
   {
      audio_set_track(1);
      PlaySound(sound_countdown_1);
      mode = 12;
   }
}

static void mode_game_update()
{
   UpdateMusicStream(sound_drive);
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   player_update(1);
   cars_update(1);

   if(input_pressed_pause())
   {
      if(music_current!=NULL)
         PauseMusicStream(*music_current);
      mode = 13;
      pause_select = 0;
      enable_parallax = 0;
   }

   if(player.lap==track.laps+1)
   {
      results_alpha = 0;
      results_pos = player_pos();
      results_select = 0;
      results_select = 0;
      mode = 14;
   }
}

static void mode_game_draw()
{
   game_draw(player.px,player.pz,player.steer);
   DrawTextEx(font,TextFormat("Lap\n%01d/%01d",player.lap,track.laps),(Vector2){4,4},font.baseSize,0.0f,WHITE);
   DrawTextEx(font,TextFormat("Speed\n %03d",(int)(((float)player.vz/(float)MAX_SPEED)*200.0f)),(Vector2){147,4},font.baseSize,1.0f,WHITE);
   DrawTextEx(font,TextFormat("Pos\n %01d",player_pos()),(Vector2){301,4},font.baseSize,1.0f,WHITE);
}

static void mode_pause_update()
{
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   if(input_pressed_up()&&pause_select>0)
      pause_select--;
   if(input_pressed_down()&&pause_select<2)
      pause_select++;
   if(input_pressed_confirm())
   {
      switch(pause_select)
      {
      case 0: mode = 12; enable_parallax = 1; if(music_current!=NULL) ResumeMusicStream(*music_current); break;
      case 1: mode = 11; enable_parallax = 1; track_build(); if(music_current!=NULL) StopMusicStream(*music_current); StopSound(sound_countdown_1); PlaySound(sound_countdown_0); break;
      case 2: mode = 0; enable_parallax = 1; audio_set_track(0); break;
      }
   }
}

static void mode_pause_draw()
{
   game_draw(player.px,player.pz,player.steer);
   DrawTextEx(font,TextFormat("Lap\n%01d/%01d",player.lap,track.laps),(Vector2){4,4},font.baseSize,0.0f,WHITE);
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
}

static void mode_results_update()
{
   UpdateMusicStream(sound_drive);
   if(music_current!=NULL)
      UpdateMusicStream(*music_current);

   float dt = GetFrameTime();
   player_update(0);
   cars_update(1);
   if(results_alpha<255.0f)
      results_alpha+=128.0f*dt;

   if(input_pressed_up()&&results_select>0)
      results_select--;
   if(input_pressed_down()&&results_select<1)
      results_select++;
   if(input_pressed_confirm())
   {
      switch(results_select)
      {
      case 0: mode = 11; track_build(); if(music_current!=NULL) StopMusicStream(*music_current); StopSound(sound_countdown_1); PlaySound(sound_countdown_0); break;
      case 1: mode = 0; audio_set_track(0); break;
      }
   }
}

static void mode_results_draw()
{
   Color fade = (Color){255,255,255,MIN(255,(int)results_alpha)};
   Color reverse_fade = (Color){255,255,255,255-fade.a};

   game_draw(player.px,player.pz,player.steer);
   DrawTextEx(font,TextFormat("Lap\n%01d/%01d",player.lap,track.laps),(Vector2){4,4},font.baseSize,0.0f,reverse_fade);
   DrawTextEx(font,TextFormat("Speed\n %03d",(int)(((float)player.vz/(float)MAX_SPEED)*200.0f)),(Vector2){147,4},font.baseSize,1.0f,reverse_fade);
   DrawTextEx(font,TextFormat("Pos\n %01d",results_pos),(Vector2){301,4},font.baseSize,1.0f,reverse_fade);
   DrawTextureRec(texture_menu,(Rectangle){8,240,256,200},(Vector2){32,20},fade);
   DrawTextEx(font,"Time",(Vector2){40,36},font.baseSize,1.0f,fade);
   for(int i = 0;i<track.laps;i++)
      DrawTextEx(font,TextFormat("%02d:%02d:%03d",(Fixed1616_to_int(lap_times[i])/60),Fixed1616_to_int(lap_times[i])%60,Fixed1616_to_int(1000*(lap_times[i]%Fixed1616_from_int(1)))),(Vector2){40,52+i*8},font.baseSize,1.0f,fade);
   if(results_pos==1)
      DrawTextEx(font,"1st",(Vector2){145,44},font.baseSize*2,1.0f,fade);
   else if(results_pos==2)
      DrawTextEx(font,"2nd",(Vector2){145,44},font.baseSize*2,1.0f,fade);
   else
      DrawTextEx(font,TextFormat("%dth",results_pos),(Vector2){145,44},font.baseSize*2,1.0f,fade);
   DrawTextEx(font,TextFormat("Collisions\n   %04d",player.collisions),(Vector2){222,36},font.baseSize,1.0f,fade);
   DrawTextEx(font,"Restart",(Vector2){142,160},font.baseSize,0.0f,fade);
   DrawTextEx(font,"Quit",(Vector2){150,176},font.baseSize,0.0f,fade);
   switch(results_select)
   {
   case 0: DrawTextEx(font,">       <",(Vector2){137,160},font.baseSize,0.0f,fade); break;
   case 1: DrawTextEx(font,">    <",(Vector2){145,176},font.baseSize,0.0f,fade); break;
   }
}
//-------------------------------------
