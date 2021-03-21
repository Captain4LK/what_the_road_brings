/*
SoftLK-demos - a collection of bigger examples for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <string.h>
#include <SLK/SLK.h>
//-------------------------------------

//Internal includes
#include "../external/ULK_fixed.h"
#include "config.h"
#include "segment.h"
#include "draw.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static SLK_Pal_sprite *backdrop = NULL;
static SLK_Pal_sprite *car_sprites[7];
//-------------------------------------

//Function prototypes
static void project_point(Point *p, ULK_fixed cam_x, ULK_fixed cam_y, ULK_fixed cam_z, ULK_fixed cam_depth, int width, int height, int road_width);
static void draw_segment(Segment *s);
//-------------------------------------

//Function implementations

void load_assets()
{
   backdrop = SLK_pal_sprite_load("assets/night.slk");
   SLK_Pal_sprite *car = SLK_pal_sprite_load("assets/car.slk");
   for(int i = 0;i<7;i++)
   {
      car_sprites[i] = SLK_pal_sprite_create(150,107);
      SLK_pal_sprite_copy_partial(car_sprites[i],car,0,0,i*150,0,150,107);
   }
   SLK_pal_sprite_destroy(car);
}

void draw(ULK_fixed x, ULK_fixed z, int steer)
{
   //SLK_Pal_sprite *target = SLK_draw_pal_get_target();
   //memcpy(target->data,backdrop->data,sizeof(*target->data)*640*480);

   int i = 0;
   int is = 0;
   int max_y = YRES;
   Segment *base_player = segment_list_get_pos(&segments,z+ULK_fixed_from_int(64),&i);
   Segment *base = segment_list_get_pos(&segments,z,&i);
   int max = i+RENDER_DISTANCE;
   is = i;
   
   ULK_fixed_32 pos = ULK_fixed_32_div((z%SEGLEN)<<8,SEGLEN<<8);
   ULK_fixed_32 ppos = ULK_fixed_32_div(((z+ULK_fixed_from_int(64))%SEGLEN)<<8,SEGLEN<<8);
   ULK_fixed_32 cdx = -(ULK_fixed_32_mul(base->curve,pos));
   ULK_fixed_32 cx = -cdx;
   ULK_fixed_32 py = base_player->p0.y+ULK_fixed_32_mul(base_player->p1.y-base_player->p0.y,ppos);

   Point cache;
   project_point(&base->p0,(x)-cx-cdx,py+CAM_HEIGHT,z,CAM_DEPTH,XRES,YRES,ROAD_WIDTH);
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
      if(s->p0.camera_z<<8<=CAM_DEPTH||ULK_fixed_32_to_int(s->p1.screen_y)>=max_y||s->p1.screen_y>=s->p0.screen_y)
         continue;

      draw_segment(s);

      max_y = ULK_fixed_32_to_int(s->p1.screen_y);
   }

   SLK_draw_pal_sprite(car_sprites[steer+3],245,370);
}

static void project_point(Point *p, ULK_fixed_32 cam_x, ULK_fixed cam_y, ULK_fixed cam_z, ULK_fixed_32 cam_depth, int width, int height, int road_width)
{
p->camera_x = p->x-cam_x;
   p->camera_y = p->y-cam_y;
   p->camera_z = p->z-cam_z;
   if(p->camera_z==0)
      p->camera_z = INT16_MAX<<8;
   p->screen_x = ULK_fixed_32_from_int(width/2)+ULK_fixed_32_mul(ULK_fixed_32_from_int(width/2),ULK_fixed_32_div(p->camera_x,p->camera_z<<8));
   p->screen_y = ULK_fixed_32_round(((ULK_fixed_32_from_int(height/2)-ULK_fixed_32_mul(ULK_fixed_32_from_int(height/2),ULK_fixed_32_div(p->camera_y,p->camera_z<<8)))));
   //p->screen_w = ((ULK_fixed_32_mul(p->screen_scale,ULK_fixed_32_mul(ULK_fixed_32_from_int(road_width),ULK_fixed_32_from_int(width/2))))); //Doesn't fucking work
   p->screen_w = ULK_fixed_32_div(ULK_fixed_32_from_int(width),p->camera_z<<8)*48;
}

static void draw_segment(Segment *s)
{
   ULK_fixed_32 height = s->p0.screen_y-s->p1.screen_y; 
   if(height==0)
      return;
   ULK_fixed_32 dx = ULK_fixed_32_div((s->p0.screen_x-s->p1.screen_x),(height));
   ULK_fixed_32 dw = ULK_fixed_32_div((s->p0.screen_w-s->p1.screen_w),(height));
   ULK_fixed_32 x = (s->p1.screen_x);
   ULK_fixed_32 w = (s->p1.screen_w);
   x+=ULK_fixed_32_mul(dx,ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);
   w+=ULK_fixed_32_mul(dw,ULK_fixed_32_ceil(s->p1.screen_y)-s->p1.screen_y);
   ULK_fixed_32 y = ULK_fixed_32_to_int((s->p1.screen_y));

   /*SDL_SetRenderDrawColor(renderer,s->color.r,s->color.g,s->color.b,s->color.a);
   SDL_Rect rect;
   rect.x = 0;
   rect.y = y;
   rect.w = window_width;
   rect.h = ULK_fixed_32_to_int(ULK_fixed_32_round(height));
   SDL_RenderFillRect(renderer,&rect);*/
   if(1||s->line)
      while(y<ULK_fixed_32_to_int(ULK_fixed_32_round(s->p0.screen_y)))
      {
         /*ULK_fixed_32 w16 = w/16;
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(x-w)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(x-w+w16)),y);
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(x-w+w16)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(x+w-w16)),y);
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(x+w-w16)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(x+w)),y);
         x+=dx;
         w+=dw;
         y++;*/
         int w16 = w/16;
         SLK_draw_pal_horizontal_line(0,ULK_fixed_32_to_int(ULK_fixed_32_round(x-w)),y,s->color);
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(x-w),ULK_fixed_32_to_int(x-w+w16),y,s->color_border);
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(x-w+w16),ULK_fixed_32_to_int(x+w-w16),y,s->color_road);
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(x+w-w16),ULK_fixed_32_to_int(x+w),y,s->color_border);
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(x+w),XRES,y,s->color);
         x+=dx;
         w+=dw;
         y++;
      }
   else
      while(y<ULK_fixed_32_to_int(s->p0.screen_y))
      {
        /* ULK_fixed_32 w16 = w/16;
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(x-w)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(x-w+w16)),y);

         ULK_fixed_32 start = x-w+w16;
         ULK_fixed_32 end = x-w+10*w16;
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(start)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(end)),y);
         start = end;
         end+=w16;
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(start)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(end)),y);
         start = end;
         end+=10*w16;
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(start)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(end)),y);
         start = end;
         end+=w16;
         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(start)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(end)),y);
         start = end;
         end+=10*w16;
         SDL_SetRenderDrawColor(renderer,s->color_road.r,s->color_road.g,s->color_road.b,s->color_road.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(start)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(end)),y);

         SDL_SetRenderDrawColor(renderer,s->color_border.r,s->color_border.g,s->color_border.b,s->color_border.a);
         SDL_RenderDrawLine(renderer,ULK_fixed_32_to_int(ULK_fixed_32_round(x+w-w16)),y,ULK_fixed_32_to_int(ULK_fixed_32_round(x+w)),y);

         x+=dx;
         w+=dw;
         y++;*/
         int w16 = w/16;
         SLK_draw_pal_horizontal_line(0,ULK_fixed_32_to_int(x-w),y,s->color);
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(x-w),ULK_fixed_32_to_int(x-w+w16),y,s->color_border);

         ULK_fixed_32 start = x-w+w16;
         ULK_fixed_32 end = x-w+10*w16;
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(start),ULK_fixed_32_to_int(end),y,s->color_road);
         start = end;
         end+=w16;
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(start),ULK_fixed_32_to_int(end),y,s->color_border);
         start = end;
         end+=10*w16;
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(start),ULK_fixed_32_to_int(end),y,s->color_road);
         start = end;
         end+=w16;
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(start),ULK_fixed_32_to_int(end),y,s->color_border);
         start = end;
         end+=10*w16;
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(start),ULK_fixed_32_to_int(end),y,s->color_road);

         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(x+w-w16),ULK_fixed_32_to_int(x+w),y,s->color_border);
         SLK_draw_pal_horizontal_line(ULK_fixed_32_to_int(x+w),XRES,y,s->color);
         x+=dx;
         w+=dw;
         y++;
   }
}
//-------------------------------------
