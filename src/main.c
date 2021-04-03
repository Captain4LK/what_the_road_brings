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
#include <string.h>
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
static Color grass0 = {88,125,62,255};
static Color grass1 = {59,115,73,255};
static Color border0 = {102,102,102,255};
static Color border1 = {59,115,73,255};
static Color road0 = {59,45,31,255};
static Color road1 = {59,45,31,255};
static int fullscreen = 0;
//-------------------------------------

//Function prototypes
static void add_road(int start, int end, int length, ULK_fixed_32 curve, ULK_fixed_32 hill);
static void add_sprite(int seg, int index, ULK_fixed_32 pos);
static void add_car(int seg, int index, ULK_fixed_32 pos);
static void main_loop();
//-------------------------------------

//Function implementations

int main(int argc, char **arg)
{
   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   //SetConfigFlags(FLAG_FULLSCREEN_MODE);
   InitWindow(XRES*2,YRES*2,"What the road brings");
   InitAudioDevice();
   SetTargetFPS(FPS);

   load_assets();
   audio_load();
   clear_texture = grass1;

   //Setup road
   dyn_array_init(Segment,&segments,128);

   add_road(0,0,64,0,0);
   add_road(0,0,16,0,0);
   add_road(4,4,56,-ULK_fixed_32_from_int(2),0);
   add_road(0,0,16,0,0);
   add_road(4,4,56,ULK_fixed_32_from_int(1),0);
   for(int i = 0;i<2;i++)
   {
      add_road(4,4,32,0,ULK_fixed_32_from_int(400));
      add_road(4,4,56,ULK_fixed_32_from_int(1)/2,0);
      add_road(4,4,32,0,-ULK_fixed_32_from_int(400));
   }
   add_road(4,4,56,-ULK_fixed_32_from_int(1),0);
   add_road(0,0,16,0,0);
   add_road(4,4,56,ULK_fixed_32_from_int(1),0);
   add_road(4,4,160,ULK_fixed_32_from_int(1)/2,ULK_fixed_32_from_int(2000));
   add_road(0,0,8,0,0);
   add_road(4,4,160,0,-ULK_fixed_32_from_int(2000));
   add_road(0,0,16,0,0);

   segment_list_get_pos(&segments,PLAYER_OFFSET,NULL)->color_road = WHITE;

   printf("Segment memory: %ld bytes\n",sizeof(Segment)*segments.used);

   srand(time(NULL));
   for(int i = 0;i<segments.used;i++)
   {
      if(rand()%10>6)
         add_sprite(i,0,ULK_fixed_32_from_int(1)+(ULK_fixed_32_from_int(1))*(rand()%6+1));
      else
         add_sprite(i,1+rand()%3,ULK_fixed_32_from_int(1)+(ULK_fixed_32_from_int(1))*(rand()%6+1));
      if(rand()%10>6)
         add_sprite(i,0,-(ULK_fixed_32_from_int(1)+((ULK_fixed_32_from_int(1))*(rand()%6+1))));
      else
         add_sprite(i,1+rand()%3,-(ULK_fixed_32_from_int(1)+((ULK_fixed_32_from_int(1))*(rand()%6+1))));
      if(rand()%20==0)
         add_car(i,0,(1-rand()%3)*ULK_fixed_32_from_int(1)/2);
   }

   audio_set_track(0);
#ifndef __EMSCRIPTEN__
   while(!WindowShouldClose())
      main_loop();
#else
   emscripten_set_main_loop(main_loop,0,1);
#endif

   return 0;
}

static void add_road(int start, int end, int length, ULK_fixed_32 curve, ULK_fixed_32 hill)
{

   ULK_fixed_32 start_y = segments.used?dyn_array_element(Segment,&segments,segments.used-1).p1.y:0;
   ULK_fixed_32 end_y = start_y+hill;
   int total = start+end+length;

   for(int i = 0;i<start;i++)
   {
      Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      ULK_fixed_32 ly = segments.used?dyn_array_element(Segment,&segments,segments.used-1).p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+ULK_fixed_32_mul(end_y-start_y,((-cos(((float)i/(float)total)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      s.curve = curve+ULK_fixed_32_mul(-curve,((-ULK_fixed_32_cos(ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(end))*4)/2)+ULK_fixed_32_from_int(1)/2));
      COLOR_ROAD()
      s.curve = ULK_fixed_32_mul(curve,ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(start)),ULK_fixed_32_div(ULK_fixed_32_from_int(i),ULK_fixed_32_from_int(start))));
      dyn_array_init(Sprite,&s.sprites,2);
      dyn_array_add(Segment,&segments,128,s);
   }

   for(int i = 0;i<length;i++)
   {
      Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      ULK_fixed_32 ly = segments.used?dyn_array_element(Segment,&segments,segments.used-1).p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+ULK_fixed_32_mul(end_y-start_y,((-cos(((float)(start+i)/(float)total)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      COLOR_ROAD()
      s.curve = curve;
      dyn_array_init(Sprite,&s.sprites,2);
      dyn_array_add(Segment,&segments,128,s);
   }

   for(int i = 0;i<end;i++)
   {
      Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      ULK_fixed_32 ly = segments.used?dyn_array_element(Segment,&segments,segments.used).p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+ULK_fixed_32_mul(end_y-start_y,((-cos(((float)(start+length+i)/(float)total)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      COLOR_ROAD()
      s.curve = curve+ULK_fixed_32_mul(-curve,((-cos(((float)i/(float)end)*M_PI)*ULK_fixed_32_from_int(1))/2)+ULK_fixed_32_from_int(1)/2);
      dyn_array_init(Sprite,&s.sprites,2);
      dyn_array_add(Segment,&segments,128,s);
   }
}

static void add_sprite(int seg, int index, ULK_fixed_32 pos)
{
   Sprite sp;
   sp.index = index;
   sp.pos = pos;
   dyn_array_add(Sprite,&dyn_array_element(Segment,&segments,seg).sprites,2,sp);
}

static void add_car(int seg, int index, ULK_fixed_32 pos)
{
   Segment *segment = &dyn_array_element(Segment,&segments,seg);
   Car_list *l = car_list_new();
   l->car.pos_x = pos;
   l->car.index = index;
   l->car.z = ULK_fixed_from_int(1)+1;
   l->next = segment->cars;
   l->car.counter = 0;
   l->car.id = cars_id_counter();
   l->car.speed = ULK_fixed_mul(128+rand()%129,CAR_MAX_SPEED);
   segment->cars = l;
}

static void main_loop()
{
   if(IsKeyPressed(KEY_M))
   {
      fullscreen = !fullscreen;

      static int main_loop_owidth,main_loop_oheight;
      static int main_loop_ox,main_loop_oy;
      if(fullscreen)
      {
         main_loop_owidth = GetScreenWidth();
         main_loop_oheight = GetScreenHeight();
         main_loop_ox = GetWindowPosition().x;
         main_loop_oy = GetWindowPosition().y;
         SetWindowState(FLAG_WINDOW_UNDECORATED|FLAG_WINDOW_RESIZABLE);
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
   
   modes_update();
}
//-------------------------------------
