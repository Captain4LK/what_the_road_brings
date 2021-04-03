/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <raylib.h>
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "car.h"
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
Texture2D texture;
RenderTexture2D texture_viewport;

struct Texture_Rects texture_rects = 
{
   .road00 = {.x = 0,.y = 0,.width = 128,.height = 16},
   .road01 = {.x = 0,.y = 16,.width = 128,.height = 16},
   .road02 = {.x = 0,.y = 32,.width = 128,.height = 16},
   .road03 = {.x = 0,.y = 48,.width = 128,.height = 16},
   .car_player = 
   {{
      {.x = 0, .y = 0, .width = 90, .height = 60},
      {.x = 90, .y = 0, .width = 90, .height = 60},
      {.x = 180, .y = 0, .width = 90, .height = 60},
      {.x = 270, .y = 0, .width = 90, .height = 60},
      {.x = 360, .y = 0, .width = 90, .height = 60},
    },{
      {.x = 0, .y = 60, .width = 90, .height = 60},
      {.x = 90, .y = 60, .width = 90, .height = 60},
      {.x = 180, .y = 60, .width = 90, .height = 60},
      {.x = 270, .y = 60, .width = 90, .height = 60},
      {.x = 360, .y = 60, .width = 90, .height = 60},
   }},
   .backdrop = 
   {
      {.x = 0, .y = 120, .width = 320, .height = 160},
      {.x = 320, .y = 120, .width = 512, .height = 160},
      {.x = 832, .y = 120, .width = 512, .height = 160},
   },
   .sprites = 
   {
      {.x = 0, .y = 280, .width = 51, .height = 100},
      {.x = 51, .y = 280, .width = 32, .height = 16},
      {.x = 51, .y = 296, .width = 32, .height = 16},
      {.x = 51, .y = 312, .width = 32, .height = 32},
   },
   .car_sprites = 
   {
      {
         {.x = 90, .y = 0, .width = 90, .height = 60},
         {.x = 180, .y = 0, .width = 90, .height = 60},
         {.x = 270, .y = 0, .width = 90, .height = 60},
      },
   },
};

static struct
{
   Rectangle layers[5][2];
   float speed[5];
}parallax_data = 
{
   .layers = 
   {
      { {.x = 0, .y = 0, .width = 320, .height = 160}, {.x = 0, .y = 0, .width = 0, .height = 0 } },
      { {.x = 0, .y = 0, .width = 512, .height = 160}, {.x = 512, .y = 0, .width = 512, .height = 160 } },
      { {.x = 0, .y = 0, .width = 512, .height = 160}, {.x = 512, .y = 0, .width = 512, .height = 160 } },
   },
   .speed = 
   {
      0, 128.0f, 256.0f,
   },
};
//-------------------------------------

//Function prototypes
static void project_point(Point *p, ULK_fixed cam_x, ULK_fixed cam_y, ULK_fixed cam_z, ULK_fixed cam_depth, int width, int height, int road_width);
static void draw_segment(Segment *s);
static void parallax_scroll(ULK_fixed_32 curve);
//-------------------------------------

//Function implementations

void load_assets()
{
   texture = LoadTexture("assets/textures.png");
   texture_viewport = LoadRenderTexture(XRES,YRES);
}

void draw(ULK_fixed x, ULK_fixed z, int steer)
{
   for(int i = 0;i<5;i++)
   {
      DrawTextureRec(texture,texture_rects.backdrop[i],(Vector2){parallax_data.layers[i][0].x,parallax_data.layers[i][0].y},WHITE);
      DrawTextureRec(texture,texture_rects.backdrop[i],(Vector2){parallax_data.layers[i][1].x,parallax_data.layers[i][1].y},WHITE);
   }

   int i = 0;
   int is = 0;
   ULK_fixed_32 max_y = ULK_fixed_32_from_int(YRES);
   segment_player = segment_list_get_pos(&segments,player.pz+PLAYER_OFFSET,&i);
   Segment *base = segment_list_get_pos(&segments,z,&i);
   if(!player.stopped)
      parallax_scroll(segment_player->curve);
   int max = i+RENDER_DISTANCE;
   is = i;
   
   ULK_fixed_32 pos = ULK_fixed_32_div((z%SEGLEN)<<8,SEGLEN<<8);
   ULK_fixed_32 ppos = ULK_fixed_32_div(((z+PLAYER_OFFSET)%SEGLEN)<<8,SEGLEN<<8);
   ULK_fixed_32 cdx = -(ULK_fixed_32_mul(base->curve,pos));
   ULK_fixed_32 cx = -cdx;
   ULK_fixed_32 py = segment_player->p0.y+ULK_fixed_32_mul(segment_player->p1.y-segment_player->p0.y,ppos);

   //Draw road segments
   Point cache;
   project_point(&base->p0,(x*ROAD_WIDTH)-cx-cdx,py+CAM_HEIGHT,z-(((is%segments.used)<is)?segments.used*SEGLEN:0),CAM_DEPTH,XRES,YRES,ROAD_WIDTH);
   cache = base->p0;
   for(;i<max;i++)
   {
      Segment *s = segment_list_get(&segments,i);
      s->clip_y = max_y;
      int looped = (i%segments.used)<is?segments.used*SEGLEN:0;
      s->p0 = cache;
      project_point(&s->p1,(x*ROAD_WIDTH)-cx-cdx,py+CAM_HEIGHT,z-looped,CAM_DEPTH,XRES,YRES,ROAD_WIDTH);
      cache = s->p1;
      cx+=cdx;
      cdx+=s->curve;
      if(s->p0.camera_z<<8<=CAM_DEPTH||s->p1.screen_y>=max_y||s->p1.screen_y>=s->p0.screen_y)
         continue;
      if(ULK_fixed_32_floor(s->p0.screen_y)<ULK_fixed_32_ceil(s->p1.screen_y))
         continue;
      draw_segment(s);

      max_y = s->p1.screen_y;
   }

   //Draw sprites
   for(i = max-1;i>is;i--)
   {
      Segment *s = segment_list_get(&segments,i);
      Rectangle dst;
      float scale_0 = (float)CAM_DEPTH/(float)(s->p0.camera_z<<8);
      float scale_1 = (float)CAM_DEPTH/(float)(s->p1.camera_z<<8);

      for(int j = 0;j<s->sprites.used;j++)
      {
         Sprite *sp = &dyn_array_element(Sprite,&s->sprites,j);
         dst.width = (float)texture_rects.sprites[sp->index].width*scale_1*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE*1.5f;
         dst.height = (float)texture_rects.sprites[sp->index].height*scale_1*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE*1.5f;
         dst.x = (s->p1.screen_x/65536.0f)+(scale_1*(sp->pos/65536.0f)*ROAD_WIDTH*XRES/2)-(sp->pos<0?dst.width:0);
         dst.y = (s->p1.screen_y/65536.0f)-dst.height+1;

         float clip = MAX(dst.y+dst.height-(s->clip_y/65536.0f),0.0f);
         if(clip<dst.height)
         {
            Rectangle src = texture_rects.sprites[sp->index];
            src.height-=src.height*clip/dst.height;
            dst.height-=clip;
            DrawTexturePro(texture,src,dst,(Vector2){0,0},0.0f,WHITE);
         }
      }

      Car_list *l = s->cars;
      while(l)
      {
         int dir = (l->car.pos_x>player.px)*2;
         float t = (float)(l->car.z%SEGLEN)/(float)SEGLEN;
         float sc = interpolate(scale_0,scale_1,t);
         
         dst.width = (float)texture_rects.car_sprites[l->car.index][dir].width*sc*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE*0.5;
         dst.height = (float)texture_rects.car_sprites[l->car.index][dir].height*sc*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE*0.5;
         dst.x = interpolate(s->p0.screen_x/65536.0f,s->p1.screen_x/65536.0f,t)+(sc*(l->car.pos_x/65536.0f)*ROAD_WIDTH*XRES/2)-dst.width/2;
         dst.y = interpolate(s->p0.screen_y/65536.0f,s->p1.screen_y/65536.0f,t)-dst.height+1;

         float clip = MAX(dst.y+dst.height-(s->clip_y/65536.0f),0.0f);
         if(clip<dst.height)
         {
            Rectangle src = texture_rects.car_sprites[l->car.index][dir];
            src.height-=src.height*clip/dst.height;
            dst.height-=clip;
            DrawTexturePro(texture,src,dst,(Vector2){0,0},0.0f,WHITE);
         }

         l = l->next;
      }

      if(s==segment_player)
      {
         if(segment_player->p1.y-segment_player->p0.y>ULK_fixed_32_from_int(1))
            DrawTextureRec(texture,texture_rects.car_player[1][steer+2],(Vector2){115,170},WHITE);
         else
            DrawTextureRec(texture,texture_rects.car_player[0][steer+2],(Vector2){115,170},WHITE);
      }
   }
}

static void project_point(Point *p, ULK_fixed_32 cam_x, ULK_fixed_32 cam_y, ULK_fixed cam_z, ULK_fixed_32 cam_depth, int width, int height, int road_width)
{
   ULK_fixed_32 camera_x = -cam_x;
   ULK_fixed_32 camera_y = p->y-cam_y;
   p->camera_z = p->z-cam_z;
   if(p->camera_z==0)
      p->camera_z = INT16_MAX<<8;
   p->screen_x = (ULK_fixed_32_from_int(width/2)+ULK_fixed_32_mul(ULK_fixed_32_from_int(width/2),ULK_fixed_32_mul(ULK_fixed_32_div(camera_x,p->camera_z<<8),cam_depth)));
   p->screen_y = (ULK_fixed_32_from_int(height/2)-ULK_fixed_32_mul(ULK_fixed_32_from_int(height/2),ULK_fixed_32_mul(ULK_fixed_32_div(camera_y,p->camera_z<<8),cam_depth)));
   p->screen_w = ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(width),p->camera_z<<8)*48,cam_depth);
}

static void draw_segment(Segment *s)
{
   if(!s->line)
   {
      Vector2 a,b,c;

      //Grass
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f;
      b.x = 0.0f;
      c.x = XRES;
      DrawTriangle(a,b,c,s->color);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f;
      b.x = XRES;
      c.x = XRES;
      DrawTriangle(a,b,c,s->color);

      //Left border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Road
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      //Right border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);
   }
   else
   {
      Vector2 a,b,c;

      //Grass
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f;
      b.x = 0.0f;
      c.x = XRES;
      DrawTriangle(a,b,c,s->color);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f;
      b.x = XRES;
      c.x = XRES;
      DrawTriangle(a,b,c,s->color);

      //Left border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Road
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      //Right border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Left marker 
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+11*s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+11*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+12*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+11*s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+12*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+12*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Right marker 
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+21*s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+21*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+22*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      b.y = (ULK_fixed_32_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+21*s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+22*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+22*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);
   }
}

static void parallax_scroll(ULK_fixed_32 curve)
{
   float dt = GetFrameTime();

   for(int i = 0;i<5;i++)
   {
      float speed = -parallax_data.speed[i]*(curve/65536.0f)*((float)player.vz/(float)MAX_SPEED)*dt;

      parallax_data.layers[i][0].x+=speed;
      parallax_data.layers[i][1].x+=speed;

      //Clip layer
      Rectangle tmp;
      if(parallax_data.layers[i][0].x<0.0f)
      {
         tmp = parallax_data.layers[i][0];
         parallax_data.layers[i][0] = parallax_data.layers[i][1];
         parallax_data.layers[i][1] = tmp;
         parallax_data.layers[i][0].x = parallax_data.layers[i][1].x+parallax_data.layers[i][1].width;
      }
      else if(parallax_data.layers[i][0].x>0.0f)
      {
         tmp = parallax_data.layers[i][0];
         parallax_data.layers[i][0] = parallax_data.layers[i][1];
         parallax_data.layers[i][1] = tmp;
         parallax_data.layers[i][0].x = parallax_data.layers[i][1].x-parallax_data.layers[i][1].width;
      }
   }
}
