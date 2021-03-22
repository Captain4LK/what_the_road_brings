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
{ if((segments.used%6)<3) {s.color = grass0;s.color_road = road0;s.color_border = border0;s.line = 1;}else{s.color = grass1;s.color_road = road1;s.color_border = border1;s.line = 0;}}
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static Color grass0 = {185,217,34,255};
static Color grass1 = {156,193,27,255};
static Color border0 = {155,11,11,255};
static Color border1 = {244,240,190,255};
static Color road0 = {121,121,141,255};
static Color road1 = {139,138,154,255};
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
   add_road(0,0,32,0,0);
   for(int i = 0;i<10;i++)
   {
      add_road(4,4,32,0,ULK_fixed_32_from_int(400));
      //add_road(0,0,8,0,0);
      add_road(4,4,32,0,-ULK_fixed_32_from_int(400));
      //add_road(0,0,8,0,0);
   }
   /*for(int i = 0;i<10;i++)
   {
      add_road(8,8,32,ULK_fixed_32_from_int(1)/2,0);
      add_road(8,8,32,-ULK_fixed_32_from_int(1)/2,0);
   }*/
   /*add_road(8,8,32,ULK_fixed_32_from_int(1)/2,ULK_fixed_32_from_int(500));
   add_road(8,8,64,-ULK_fixed_32_from_int(1)/2,0);
   add_road(8,8,32,0,ULK_fixed_32_from_int(-10));
   add_road(8,8,32,0,ULK_fixed_32_from_int(10));
   add_road(8,8,32,0,ULK_fixed_32_from_int(-5));*/

   while(sdl_running())
   {
      sdl_update();

      SDL_SetRenderDrawColor(renderer,0,0,0,255);
      SDL_RenderClear(renderer);

      game_update();

      //SDL_SetRenderDrawColor(renderer,0xff,0,0xff,255);
      //SDL_RenderFillRect(renderer,&((SDL_Rect){.x = 0,.y = 0,.w = 255,.h = 255}));

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
      s.curve = curve+ULK_fixed_32_mul(-curve,((-cos(((float)i/(float)end)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      segment_list_add(&segments,&s);
   }
}
//-------------------------------------
