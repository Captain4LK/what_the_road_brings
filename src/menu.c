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
#include "draw.h"
#include "menu.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables

static struct
{
   Rectangle layers[3][2];
   float speed[3];
}title_parallax = 
{
   .layers = 
   {
      { {.x = 0, .y = 0, .width = 384, .height = 240}, {.x = 384, .y = 0, .width = 384, .height = 240 } },
      { {.x = 0, .y = 0, .width = 320, .height = 240}, {.x = 320, .y = 0, .width = 320, .height = 240 } },
      { {.x = 0, .y = 0, .width = 512, .height = 240}, {.x = 512, .y = 0, .width = 512, .height = 240 } },
   },
   { 
      8.0f, 16.0f, 32.0f,
   },
};
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void title_draw()
{
   DrawTextureRec(texture_menu,(Rectangle){0,0,320,240},(Vector2){0,0},WHITE);
   DrawTextureRec(texture_menu,(Rectangle){320,0,320,240},(Vector2){0,0},WHITE);

   /*DrawTextureRec(texture_menu,(Rectangle){320,0,384,240},(Vector2){title_parallax.layers[0][0].x,title_parallax.layers[0][0].y},WHITE);
   DrawTextureRec(texture_menu,(Rectangle){320,0,384,240},(Vector2){title_parallax.layers[0][1].x,title_parallax.layers[0][1].y},WHITE);

   DrawTextureRec(texture_menu,(Rectangle){704,0,320,240},(Vector2){title_parallax.layers[1][0].x,title_parallax.layers[1][0].y},WHITE);
   DrawTextureRec(texture_menu,(Rectangle){704,0,320,240},(Vector2){title_parallax.layers[1][1].x,title_parallax.layers[1][1].y},WHITE);

   DrawTextureRec(texture_menu,(Rectangle){1024,0,512,240},(Vector2){title_parallax.layers[2][0].x,title_parallax.layers[2][0].y},WHITE);
   DrawTextureRec(texture_menu,(Rectangle){1024,0,512,240},(Vector2){title_parallax.layers[2][1].x,title_parallax.layers[2][1].y},WHITE);*/
}

void title_update()
{
   if(IsKeyPressed(KEY_N))
      mode = 11;

   float dt = GetFrameTime();

   for(int i = 0;i<3;i++)
   {
      float speed = -title_parallax.speed[i]*dt;

      title_parallax.layers[i][0].x+=speed;
      title_parallax.layers[i][1].x+=speed;

      //Clip layer
      Rectangle tmp;
      if(title_parallax.layers[i][0].x<0.0f)
      {
         tmp = title_parallax.layers[i][0];
         title_parallax.layers[i][0] = title_parallax.layers[i][1];
         title_parallax.layers[i][1] = tmp;
         title_parallax.layers[i][0].x = title_parallax.layers[i][1].x+title_parallax.layers[i][1].width;
      }
      else if(title_parallax.layers[i][0].x>0.0f)
      {
         tmp = title_parallax.layers[i][0];
         title_parallax.layers[i][0] = title_parallax.layers[i][1];
         title_parallax.layers[i][1] = tmp;
         title_parallax.layers[i][0].x = title_parallax.layers[i][1].x-title_parallax.layers[i][1].width;
      }
   }
}
//-------------------------------------
