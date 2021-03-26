/*
A racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <math.h>
#include <SDL2/SDL.h>
//#include <SLK/SLK.h>
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "sdl.h"
#include "player.h"
#include "segment.h"
#include "draw.h"
//-------------------------------------

//#defines
#define COLOR_ROAD() \
{ if((segments.used%10)<5) {s.color = grass0;s.color_road = road0;s.color_border = border0;s.line = segments.used%5>1;}else{s.color = grass1;s.color_road = road1;s.color_border = border1;s.line = 0;}}
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static Color grass0 = {83,80,10,255};
static Color grass1 = {65,65,35,255};
static Color border0 = {102,102,102,255};
static Color border1 = {65,65,35,255};
//static Color road0 = {34,34,34,255};
static Color road0 = {59,45,31,255};
static Color road1 = {59,45,31,255};
//-------------------------------------

//Function prototypes
static void game_update();
static void add_road(int start, int end, int length, ULK_fixed_32 curve, ULK_fixed_32 hill);
//-------------------------------------

//Function implementations

int main(int argc, char **arg)
{
   sdl_init();
   load_assets();

   //Setup road
   segment_list_init(&segments);
   add_road(0,0,64,0,0);
   //add_road(0,0,640,-ULK_fixed_32_from_int(4),0);
   add_road(0,0,16,0,0);
   add_road(0,0,64,-ULK_fixed_32_from_int(1)/2,0);
   add_road(0,0,16,0,0);
   add_road(0,0,64,ULK_fixed_32_from_int(1)/2,0);
   for(int i = 0;i<4;i++)
   {
      add_road(4,4,32,0,ULK_fixed_32_from_int(400));
      add_road(4,4,32,0,-ULK_fixed_32_from_int(400));
   }

   int fullscreen = 0;
   while(sdl_running())
   {
      sdl_update();

      SDL_SetRenderTarget(renderer,target);
      if(sdl_key_pressed(KEY_M))
      {
         fullscreen = !fullscreen;
         SDL_SetWindowFullscreen(sdl_window,fullscreen?SDL_WINDOW_FULLSCREEN_DESKTOP:0);
      }

      SDL_SetRenderDrawColor(renderer,0,0,0,255);
      SDL_RenderClear(renderer);

      game_update();
      SDL_SetRenderTarget(renderer,NULL);

      SDL_SetRenderDrawColor(renderer,0,0,0,255);
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer,target,NULL,NULL);
      SDL_RenderPresent(renderer);
   }

   return 0;
}

static void game_update()
{
   player_update();

   draw(player.px,player.pz,player.steer);
}

static void add_road(int start, int end, int length, ULK_fixed_32 curve, ULK_fixed_32 hill)
{

   ULK_fixed_32 start_y = segments.used?segments.segments[segments.used-1].p1.y:0;
   ULK_fixed_32 end_y = start_y+hill;
   int total = start+end+length;

   for(int i = 0;i<start;i++)
   {
       Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      ULK_fixed_32 ly = segments.used?segments.segments[segments.used-1].p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+ULK_fixed_32_mul(end_y-start_y,((-cos(((float)i/(float)total)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      s.curve = curve+ULK_fixed_32_mul(-curve,((-ULK_fixed_32_cos(ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(end))*4)/2)+ULK_fixed_32_from_int(1)/2));
      COLOR_ROAD()
      s.texture = segments.used%4;
      s.curve = ULK_fixed_32_mul(curve,ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(start)),ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(start))));
      segment_list_add(&segments,&s);
   }

   for(int i = 0;i<length;i++)
   {
      Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      ULK_fixed_32 ly = segments.used?segments.segments[segments.used-1].p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+ULK_fixed_32_mul(end_y-start_y,((-cos(((float)(start+i)/(float)total)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      COLOR_ROAD()
      s.texture = segments.used%4;
      s.curve = curve;
      segment_list_add(&segments,&s);
   }

   for(int i = 0;i<end;i++)
   {
       Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      ULK_fixed_32 ly = segments.used?segments.segments[segments.used-1].p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+ULK_fixed_32_mul(end_y-start_y,((-cos(((float)(start+length+i)/(float)total)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      COLOR_ROAD()
      //s.curve = ULK_fixed_32_mul(curve,ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(end)),ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(end))));
      s.texture = segments.used%4;
      s.curve = curve+ULK_fixed_32_mul(-curve,((-cos(((float)i/(float)end)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      segment_list_add(&segments,&s);
   }
}
//-------------------------------------
