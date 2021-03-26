/*
SoftLK-demos - a collection of bigger examples for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <SDL2/SDL.h>
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "sdl.h"
#include "segment.h"
#include "draw.h"
#include "player.h"
//-------------------------------------

//#defines
#define MIN(a,b) \
   ((a)<(b)?(a):(b))

#define MAX(a,b) \
   ((a)>(b)?(a):(b))
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static int draw_mode = 0;
static SDL_Texture *texture = NULL;

static struct
{
   SDL_Rect road00;
   SDL_Rect road01;
   SDL_Rect road02;
   SDL_Rect road03;
   SDL_Rect car_player[2][9];
   SDL_Rect backdrop[5];
}texture_rects = 
{
   .road00 = {.x = 0,.y = 0,.w = 128,.h = 16},
   .road01 = {.x = 0,.y = 16,.w = 128,.h = 16},
   .road02 = {.x = 0,.y = 32,.w = 128,.h = 16},
   .road03 = {.x = 0,.y = 48,.w = 128,.h = 16},
   .car_player = 
   {{
      {.x = 0, .y = 0, .w = 90, .h = 48},
      {.x = 90, .y = 0, .w = 90, .h = 48},
      {.x = 180, .y = 0, .w = 90, .h = 48},
      {.x = 270, .y = 0, .w = 90, .h = 48},
      {.x = 360, .y = 0, .w = 90, .h = 48},
      {.x = 450, .y = 0, .w = 90, .h = 48},
      {.x = 540, .y = 0, .w = 90, .h = 48},
      {.x = 630, .y = 0, .w = 90, .h = 48},
      {.x = 720, .y = 0, .w = 90, .h = 48},
    },{
      {.x = 0, .y = 48, .w = 90, .h = 48},
      {.x = 90, .y = 48, .w = 90, .h = 48},
      {.x = 180, .y = 48, .w = 90, .h = 48},
      {.x = 270, .y = 48, .w = 90, .h = 48},
      {.x = 360, .y = 48, .w = 90, .h = 48},
      {.x = 450, .y = 48, .w = 90, .h = 48},
      {.x = 540, .y = 48, .w = 90, .h = 48},
      {.x = 630, .y = 48, .w = 90, .h = 48},
      {.x = 720, .y = 48, .w = 90, .h = 48},
   }},
   .backdrop = 
   {
      {.x = 0, .y = 144, .w = 320, .h = 160},
      {.x = 320, .y = 144, .w = 272, .h = 160},
      {.x = 592, .y = 144, .w = 544, .h = 160},
      {.x = 1136, .y = 144, .w = 544, .h = 160},
      {.x = 1680, .y = 144, .w = 544, .h = 160},
   },
};

static struct
{
   SDL_FRect layers[5][2];
   float speed[5];
}parallax_data = 
{
   .layers = 
   {
      { {.x = 0, .y = -20, .w = 320, .h = 160}, {.x = 0, .y = -20, .w = 0, .h = 0 } },
      { {.x = 0, .y = -20, .w = 272, .h = 160}, {.x = 272, .y = -20, .w = 272, .h = 160 } },
      { {.x = 0, .y = -20, .w = 544, .h = 160}, {.x = 544, .y = -20, .w = 544, .h = 160 } },
      { {.x = 0, .y = -20, .w = 544, .h = 160}, {.x = 544, .y = -20, .w = 544, .h = 160 } },
      { {.x = 0, .y = -20, .w = 544, .h = 160}, {.x = 544, .y = -20, .w = 544, .h = 160 } },
   },
   .speed = 
   {
      0, 0.5f, 1.0f, 1.5f, 2.0f,
   },
};
//-------------------------------------

//Function prototypes
static void project_point(Point *p, ULK_fixed cam_x, ULK_fixed cam_y, ULK_fixed cam_z, ULK_fixed cam_depth, int width, int height, int road_width);
static void draw_segment(Segment *s);
static void draw_segment_tex(Segment *s);
static void draw_segment_frame(Segment *s);
static void parallax_scroll(ULK_fixed_32 curve);
//-------------------------------------

//Function implementations

void load_assets()
{
   texture = sdl_load_image("assets/textures.png");
}

void draw(ULK_fixed x, ULK_fixed z, int steer)
{
   for(int i = 0;i<5;i++)
   {
      SDL_RenderCopyF(renderer,texture,&texture_rects.backdrop[i],&parallax_data.layers[i][0]);
      SDL_RenderCopyF(renderer,texture,&texture_rects.backdrop[i],&parallax_data.layers[i][1]);
   }

   if(sdl_key_pressed(KEY_Y))
   {
      draw_mode++;
      if(draw_mode==3)
         draw_mode = 0;
   }

   int i = 0;
   int is = 0;
   ULK_fixed_32 max_y = ULK_fixed_32_from_int(YRES);
   Segment *base_player = segment_list_get_pos(&segments,z+ULK_fixed_from_int(64),&i);
   Segment *base = segment_list_get_pos(&segments,z,&i);
   parallax_scroll(base_player->curve);
   player.px-=ULK_fixed_32_mul(ULK_fixed_32_div(player.vz,MAX_SPEED),ULK_fixed_32_mul(base_player->curve,ULK_fixed_32_from_int(2)));
   int max = i+RENDER_DISTANCE;
   is = i;
   
   ULK_fixed_32 pos = ULK_fixed_32_div((z%SEGLEN)<<8,SEGLEN<<8);
   ULK_fixed_32 ppos = ULK_fixed_32_div(((z+ULK_fixed_from_int(64))%SEGLEN)<<8,SEGLEN<<8);
   ULK_fixed_32 cdx = -(ULK_fixed_32_mul(base->curve,pos));
   ULK_fixed_32 cx = -cdx;
   ULK_fixed_32 py = base_player->p0.y+ULK_fixed_32_mul(base_player->p1.y-base_player->p0.y,ppos);

   Point cache;
   project_point(&base->p0,(x)-cx-cdx,py+CAM_HEIGHT,z-((is%segments.used<is)?segments.used*SEGLEN:0),CAM_DEPTH,XRES,YRES,ROAD_WIDTH);
   cache = base->p0;
   for(;i<max;i++)
   {
      Segment *s = segment_list_get(&segments,i);
      int looped = (i%segments.used)<is?segments.used*SEGLEN:0;
      s->p0 = cache;
      project_point(&s->p1,(x)-cx-cdx,py+CAM_HEIGHT,z-looped,CAM_DEPTH,XRES,YRES,ROAD_WIDTH);
      cache = s->p1;
      cx+=cdx;
      cdx+=s->curve;
      if(s->p0.camera_z<<8<=CAM_DEPTH||s->p1.screen_y>=max_y||s->p1.screen_y>=s->p0.screen_y)
         continue;
      if(ULK_fixed_32_floor(s->p0.screen_y)<ULK_fixed_32_ceil(s->p1.screen_y))
         continue;

      switch(draw_mode)
      {
      case 0: draw_segment(s); break;
      case 1: draw_segment_frame(s); break;
      case 2: draw_segment_tex(s); break;
      }

      max_y = s->p1.screen_y;
   }

   if(base_player->p1.y-base_player->p0.y>ULK_fixed_32_from_int(1))
      SDL_RenderCopy(renderer,texture,&texture_rects.car_player[1][steer+4],&((SDL_Rect){.x = 115, .y = 180, .w = 90, .h = 48}));
   else
      SDL_RenderCopy(renderer,texture,&texture_rects.car_player[0][steer+4],&((SDL_Rect){.x = 115, .y = 180, .w = 90, .h = 48}));
}

static void project_point(Point *p, ULK_fixed_32 cam_x, ULK_fixed_32 cam_y, ULK_fixed cam_z, ULK_fixed_32 cam_depth, int width, int height, int road_width)
{
p->camera_x = p->x-cam_x;
   p->camera_y = p->y-cam_y;
   p->camera_z = p->z-cam_z;
   if(p->camera_z==0)
      p->camera_z = INT16_MAX<<8;
   p->screen_x = (ULK_fixed_32_from_int(width/2)+ULK_fixed_32_mul(ULK_fixed_32_from_int(width/2),ULK_fixed_32_mul(ULK_fixed_32_div(p->camera_x,p->camera_z<<8),cam_depth)));
   p->screen_y = (ULK_fixed_32_from_int(height/2)-ULK_fixed_32_mul(ULK_fixed_32_from_int(height/2),ULK_fixed_32_mul(ULK_fixed_32_div(p->camera_y,p->camera_z<<8),cam_depth)));
   p->screen_w = ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(width),p->camera_z<<8)*48,cam_depth);
}

static void draw_segment(Segment *s)
{
   ULK_fixed_32 height = s->p0.screen_y-s->p1.screen_y; 
   ULK_fixed_32 dx = ULK_fixed_32_div((s->p0.screen_x-s->p1.screen_x),(height));
   ULK_fixed_32 dw = ULK_fixed_32_div((s->p0.screen_w-s->p1.screen_w),(height));
   ULK_fixed_32 x = (s->p1.screen_x);
   ULK_fixed_32 w = (s->p1.screen_w);
   x+=ULK_fixed_32_mul(dx,ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);
   w+=ULK_fixed_32_mul(dw,ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);
   ULK_fixed_32 y = (s->p1.screen_y);//+ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);

   SDL_SetRenderDrawColor(renderer,s->color.r,s->color.g,s->color.b,s->color.a);
   SDL_Rect rect;
   rect.x = 0;
   rect.y = ULK_fixed_32_to_int(y);
   rect.w = XRES;
   rect.h = ULK_fixed_32_to_int(ULK_fixed_32_ceil(height));
   SDL_RenderFillRect(renderer,&rect);

   if(!s->line)
   {
      int y_draw = ULK_fixed_32_to_int((y));
      while(y<(s->p0.screen_y))
      {
         ULK_fixed_32 w16 = w/16;
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((x-w)),y_draw,ULK_fixed_32_to_int((x-w+w16)),y_draw);
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((x-w+w16)),y_draw,ULK_fixed_32_to_int((x+w-w16)),y_draw);
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((x+w-w16)),y_draw,ULK_fixed_32_to_int((x+w)),y_draw);
         x+=dx;
         w+=dw;
         y+=ULK_fixed_32_from_int(1);
         y_draw++;
      }
   }
   else
   {
      int y_draw = ULK_fixed_32_to_int((y));
      while(y<(s->p0.screen_y))
      {
         ULK_fixed_32 w16 = w/16;

         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((x-w)),y_draw,ULK_fixed_32_to_int((x-w+w16)),y_draw);

         ULK_fixed_32 start = x-w+w16;
         ULK_fixed_32 end = x-w+10*w16;
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((start)),y_draw,ULK_fixed_32_to_int((end)),y_draw);
         start = end;
         end+=w16;
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((start)),y_draw,ULK_fixed_32_to_int((end)),y_draw);
         start = end;
         end+=10*w16;
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((start)),y_draw,ULK_fixed_32_to_int((end)),y_draw);
         start = end;
         end+=w16;
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((start)),y_draw,ULK_fixed_32_to_int((end)),y_draw);
         start = end;
         end+=10*w16;
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((start)),y_draw,ULK_fixed_32_to_int((end)),y_draw);

         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int((x+w-w16)),y_draw,ULK_fixed_32_to_int((x+w)),y_draw);

         x+=dx;
         w+=dw;
         y+=ULK_fixed_32_from_int(1);
         y_draw++;
      }
   }
}

static void draw_segment_frame(Segment *s)
{
   SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
   SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(s->p1.screen_x-s->p1.screen_w),ULK_fixed_32_to_int(s->p1.screen_y),ULK_fixed_32_to_int(s->p1.screen_x+s->p1.screen_w),ULK_fixed_32_to_int(s->p1.screen_y));
   SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(s->p0.screen_x-s->p1.screen_w),ULK_fixed_32_to_int(s->p0.screen_y),ULK_fixed_32_to_int(s->p0.screen_x+s->p0.screen_w),ULK_fixed_32_to_int(s->p0.screen_y));
   SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(s->p1.screen_x-s->p1.screen_w),ULK_fixed_32_to_int(s->p1.screen_y),ULK_fixed_32_to_int(s->p0.screen_x-s->p0.screen_w),ULK_fixed_32_to_int(s->p0.screen_y));
   SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(s->p0.screen_x+s->p0.screen_w),ULK_fixed_32_to_int(s->p0.screen_y),ULK_fixed_32_to_int(s->p1.screen_x+s->p1.screen_w),ULK_fixed_32_to_int(s->p1.screen_y));
}

static void draw_segment_tex(Segment *s)
{
   ULK_fixed_32 height = s->p0.screen_y-s->p1.screen_y; 
   ULK_fixed_32 dx = ULK_fixed_32_div((s->p0.screen_x-s->p1.screen_x),(height));
   ULK_fixed_32 dw = ULK_fixed_32_div((s->p0.screen_w-s->p1.screen_w),(height));
   ULK_fixed_32 x = (s->p1.screen_x);
   ULK_fixed_32 w = (s->p1.screen_w);
   x+=ULK_fixed_32_mul(dx,ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);
   w+=ULK_fixed_32_mul(dw,ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);
   ULK_fixed_32 y = (s->p1.screen_y);//+ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);

   SDL_SetRenderDrawColor(renderer,s->color.r,s->color.g,s->color.b,s->color.a);
   SDL_Rect rect;
   SDL_Rect trect;
   rect.x = 0;
   rect.y = ULK_fixed_32_to_int(y);
   rect.w = XRES;
   rect.h = ULK_fixed_32_to_int((height))+1;
   SDL_RenderFillRect(renderer,&rect);

   ULK_fixed_32 stripe_height = 0;
   ULK_fixed_32 stripe_y = 0;
   switch(s->texture)
   {
   case 0: stripe_height = ULK_fixed_32_div(ULK_fixed_32_from_int(texture_rects.road00.h),ULK_fixed_32_ceil(height)); stripe_y = ULK_fixed_32_from_int(texture_rects.road00.y); break;
   case 1: stripe_height = ULK_fixed_32_div(ULK_fixed_32_from_int(texture_rects.road01.h),ULK_fixed_32_ceil(height)); stripe_y = ULK_fixed_32_from_int(texture_rects.road01.y); break;
   case 2: stripe_height = ULK_fixed_32_div(ULK_fixed_32_from_int(texture_rects.road02.h),ULK_fixed_32_ceil(height)); stripe_y = ULK_fixed_32_from_int(texture_rects.road02.y); break;
   case 3: stripe_height = ULK_fixed_32_div(ULK_fixed_32_from_int(texture_rects.road03.h),ULK_fixed_32_ceil(height)); stripe_y = ULK_fixed_32_from_int(texture_rects.road03.y); break;
   }

   int y_draw = ULK_fixed_32_to_int((y));
   while(y<(s->p0.screen_y))
   {
      rect.w = ULK_fixed_32_to_int((2*w));
      rect.x = ULK_fixed_32_to_int((x-w));
      rect.y = y_draw;
      rect.h = 1;
      switch(s->texture)
      {
      case 0: trect.w = texture_rects.road00.w; break;
      case 1: trect.w = texture_rects.road01.w; break;
      case 2: trect.w = texture_rects.road02.w; break;
      case 3: trect.w = texture_rects.road03.w; break;
      }
      trect.y = ULK_fixed_32_to_int((stripe_y));
      trect.x = 0;
      trect.h = MAX(1,ULK_fixed_32_to_int((stripe_height)-1));
      SDL_RenderCopy(renderer,texture,&trect,&rect);
      x+=dx;
      w+=dw;
      y+=ULK_fixed_32_from_int(1);
      y_draw++;
      stripe_y+=stripe_height;
   }
}

static void parallax_scroll(ULK_fixed_32 curve)
{
   for(int i = 0;i<5;i++)
   {
      float speed = -MIN(2.0f,parallax_data.speed[i]*(curve/65536.0f)*((float)player.vz/(float)MAX_SPEED));

      parallax_data.layers[i][0].x+=speed;
      parallax_data.layers[i][1].x+=speed;

      //Clip layer
      SDL_FRect tmp;
      if(parallax_data.layers[i][0].x<0.0f)
      {
         tmp = parallax_data.layers[i][0];
         parallax_data.layers[i][0] = parallax_data.layers[i][1];
         parallax_data.layers[i][1] = tmp;
         parallax_data.layers[i][0].x = parallax_data.layers[i][1].x+parallax_data.layers[i][1].w;
      }
      else if(parallax_data.layers[i][0].x>0.0f)
      {
         tmp = parallax_data.layers[i][0];
         parallax_data.layers[i][0] = parallax_data.layers[i][1];
         parallax_data.layers[i][1] = tmp;
         parallax_data.layers[i][0].x = parallax_data.layers[i][1].x-parallax_data.layers[i][1].w;
      }
   }
}
