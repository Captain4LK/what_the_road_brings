/*
SoftLK-demos - a collection of bigger examples for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <string.h>
#include <math.h>
#include <SLK/SLK.h>
//-------------------------------------

//Internal includes
#include "../external/ULK_fixed.h"
#include "config.h"
#include "player.h"
#include "segment.h"
#include "draw.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static uint8_t grass0 = 87;
static uint8_t grass1 = 88;
static uint8_t border0 = 50;
static uint8_t border1 = 51;
static uint8_t road0 = 1;
static uint8_t road1 = 2;
//-------------------------------------

//Function prototypes
static void game_update();
static void add_road(int start, int end, int length, ULK_fixed_32 curve, ULK_fixed_32 hill);
//-------------------------------------

//Function implementations

int main(int argc, char **arg)
{
   SLK_setup(XRES,YRES,1,"SLK_img2pixel",0,SLK_WINDOW_MAX,0);
   SLK_timer_set_fps(FPS);

   //Layer for game
   SLK_layer_create(0,SLK_LAYER_PAL);
   SLK_layer_activate(0,1);
   SLK_layer_set_dynamic(0,0);
   SLK_layer_set_current(0);
   SLK_layer_set_palette(0,SLK_palette_load("assets/aurora.pal"));
   SLK_draw_pal_set_clear_index(0);
   SLK_draw_pal_clear();

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

   while(SLK_core_running())
   {
      SLK_update();

      game_update();

      SLK_render_update();
   }

   return 0;
}

static void game_update()
{
   SLK_layer_set_current(0);
   SLK_draw_pal_clear();

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
      s.color = segments.used&1?grass0:grass1;
      s.color_road = segments.used&1?road0:road1;
      s.color_border = segments.used&1?border0:border1;
      s.line = segments.used&1;
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
      s.color = segments.used&1?grass0:grass1;
      s.color_road = segments.used&1?road0:road1;
      s.color_border = segments.used&1?border0:border1;
      s.line = segments.used&1;
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
      s.color = segments.used&1?grass0:grass1;
      s.color_road = segments.used&1?road0:road1;
      s.color_border = segments.used&1?border0:border1;
      s.line = segments.used&1;
      //s.curve = ULK_fixed_32_mul(curve,ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(end)),ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(end))));
      s.curve = curve+ULK_fixed_32_mul(-curve,((-cos(((float)i/(float)end)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      segment_list_add(&segments,&s);
   }
}
//-------------------------------------
