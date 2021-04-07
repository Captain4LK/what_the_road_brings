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
#include "player.h"
#include "mode.h"
#include "draw.h"
#include "menu.h"
#include "track.h"
#include "input.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables

static int title_select = 0;
static int credits_loaded = 0;
static char *credits_text = NULL;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void title_draw()
{
   DrawTextureRec(texture_menu,(Rectangle){0,0,320,240},(Vector2){0,0},WHITE);
   DrawTextureRec(texture_menu,(Rectangle){320,0,320,240},(Vector2){0,0},WHITE);

   DrawTextureRec(texture_menu,(Rectangle){0,240,8,9},(Vector2){4,163+23*title_select},WHITE);
}

void title_update()
{
   //Select
   if(input_pressed_down()&&title_select<2)
      title_select++;
   if(input_pressed_up()&&title_select>0)
      title_select--;

   if(input_pressed_confirm())
   {
      switch(title_select)
      {
      case 0: mode = 11; track_build(); player_reset(); StopMusicStream(*music_current); PlaySound(sound_countdown_0); break;
      case 1: mode = 1; break;
      case 2: CloseWindow(); break;
      }
   }
}

void credits_draw()
{
   DrawTextureRec(texture_menu,(Rectangle){0,0,320,240},(Vector2){0,0},WHITE);
   DrawTextEx(font,credits_text,(Vector2){0,4},font.baseSize,0.0f,WHITE);
}

void credits_update()
{
   if(!credits_loaded)
   {
      credits_loaded = 1;

      FILE *f = fopen("data/credits.txt","r");
      int size = 0;
      fseek(f,0,SEEK_END);
      size = ftell(f);
      fseek(f,0,SEEK_SET);
      credits_text = malloc(size+1);
      fread(credits_text,size,1,f);
      credits_text[size] = 0;
      fclose(f);
   }

   if(input_pressed_confirm())
      mode = 0;
}
//-------------------------------------
