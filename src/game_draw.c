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
#include "config.h"
#include "util.h"
#include "fixed.h"
#include "car.h"
#include "segment.h"
#include "game_draw.h"
#include "texture.h"
#include "player.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int enable_parallax = 1;
static Camera2D camera = {.offset = {.x = XRES/2, .y = 200}, .target = {.x = XRES/2, .y = 200}, .rotation = 0.0f, .zoom = 1.0f};
//-------------------------------------

//Function prototypes
static void project_point(Point *p, Fixed1616 cam_x, Fixed1616 cam_y, Fixed2408 cam_z);
static void draw_segment(Segment *s);
static void parallax_scroll(Fixed1616 curve, float dt);
//-------------------------------------

//Function implementations

void game_draw(Fixed2408 x, Fixed2408 z, int steer, float dt)
{
   //Draw background, with parallax effect
   for(int i = 0;i<5;i++)
   {
      DrawTextureRec(texture,texture_rects.backdrop[i],(Vector2){parallax_data.layers[i][0].x,parallax_data.layers[i][0].y},WHITE);
      DrawTextureRec(texture,texture_rects.backdrop[i],(Vector2){parallax_data.layers[i][1].x,parallax_data.layers[i][1].y},WHITE);
   }
   BeginMode2D(camera);

   //max_y: clipping bounds
   //pos: how far the player has traversed the current segment
   //ppos: how far the player sprite has traversed the current segment
   //cdx: curve delta starting value, multiplied by pos, to account for partial segment traversal
   //cx: road x shift value, gets incremented by cdx every segment
   //py: starting segment height, used to move the camera to the road
   Fixed1616 max_y = Fixed1616_from_int(YRES+YRES/8);
   int index;
   Segment *base = segment_list_get_pos(&segments,z,&index);
   int max = index+RENDER_DISTANCE;
   int index_start = index;
   Fixed1616 pos = Fixed1616_div((z%SEGLEN)<<8,SEGLEN<<8);
   Fixed1616 ppos = Fixed1616_div(((z+PLAYER_OFFSET)%SEGLEN)<<8,SEGLEN<<8);
   Fixed1616 cdx = -(Fixed1616_mul(base->curve,pos));
   Fixed1616 cx = -cdx;
   segment_player = segment_list_get_pos(&segments,z+PLAYER_OFFSET,NULL);
   Fixed1616 py = segment_player->p0.y+Fixed1616_mul(segment_player->p1.y-segment_player->p0.y,ppos);
   //float target_tilt = -5.625f*(base->curve/65536.0f);
   //if(camera.rotation>target_tilt)
      //camera.rotation-=MIN(camera.rotation-target_tilt,22.5f*dt);
   //if(camera.rotation<target_tilt)
      //camera.rotation+=MIN(-camera.rotation+target_tilt,22.5f*dt);

   //Draw road segments
   //Only project every second point, the other
   //one can be cached
   project_point(&base->p0,(x*ROAD_WIDTH)-cx-cdx,py+CAM_HEIGHT,z-(((index_start%segments.used)<index_start)?segments.used*SEGLEN:0));
   Point cache = base->p0;
   for(;index<max;index++)
   {
      Segment *s = segment_list_get(&segments,index);
      s->clip_y = max_y;
      s->p0 = cache;
      project_point(&s->p1,(x*ROAD_WIDTH)-cx-cdx,py+CAM_HEIGHT,z-((index%segments.used)<index_start?segments.used*SEGLEN:0));
      cache = s->p1;

      cx+=cdx;
      cdx+=s->curve;
      if(s->p0.camera_z<<8<=CAM_DEPTH //Fixes some flickering issues
         ||s->p1.screen_y>=max_y //Clip road
         ||s->p1.screen_y>=s->p0.screen_y)
         continue;

      max_y = s->p1.screen_y; //Set new clipping value
      draw_segment(s);
   }

   //Draw sprites
   //All cars and sprites get drawn from
   //back to front, this game is not 3d so
   //there is no z buffer 
   for(index = max-1;index>index_start;index--)
   {
      Segment *s = segment_list_get(&segments,index);
      Rectangle dst;
      float scale_0 = (float)CAM_DEPTH/(float)(s->p0.camera_z<<8);
      float scale_1 = (float)CAM_DEPTH/(float)(s->p1.camera_z<<8);

      for(int j = 0;j<s->sprites.used;j++)
      {
         Sprite *sp = &dyn_array_element(Sprite,&s->sprites,j);
         dst.width = (float)texture_rects.sprites[sp->index].width*scale_1*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE;
         dst.height = (float)texture_rects.sprites[sp->index].height*scale_1*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE;
         dst.x = (s->p1.screen_x/65536.0f)+(scale_1*(sp->pos/65536.0f)*ROAD_WIDTH*XRES/2)-(sp->pos==0?dst.width/2:(sp->pos<0?dst.width:0));
         dst.y = (s->p1.screen_y/65536.0f)-dst.height+1;

         //Its possible to specify a clip rect with raylib.
         //This, however, flushes the sprite batch, worsening
         //performance.
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
         int dir = (l->car->pos_x>player.px)*2;
         float t = (float)(l->car->z%SEGLEN)/(float)SEGLEN;
         float sc = interpolate(scale_0,scale_1,t);
         
         dst.width = (float)texture_rects.car_sprites[l->car->index][dir].width*sc*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE*0.5;
         dst.height = (float)texture_rects.car_sprites[l->car->index][dir].height*sc*(XRES/2)*ROAD_WIDTH*SPRITE_SCALE*0.5;
         dst.x = interpolate(s->p0.screen_x/65536.0f,s->p1.screen_x/65536.0f,t)+(sc*(l->car->pos_x/65536.0f)*ROAD_WIDTH*XRES/2)-dst.width/2;
         dst.y = interpolate(s->p0.screen_y/65536.0f,s->p1.screen_y/65536.0f,t)-dst.height+1;

         float clip = MAX(dst.y+dst.height-(s->clip_y/65536.0f),0.0f);
         if(clip<dst.height)
         {
            Rectangle src = texture_rects.car_sprites[l->car->index][dir];
            src.height-=src.height*clip/dst.height;
            dst.height-=clip;
            DrawTexturePro(texture,src,dst,(Vector2){0,0},0.0f,WHITE);
         }

         l = l->next;
      }

      if(s==segment_player)
      {
         if(segment_player->p1.y-segment_player->p0.y>Fixed1616_from_int(1)/2)
            DrawTextureRec(texture,texture_rects.car_player[1][steer+2],(Vector2){115,170},WHITE);
         else
            DrawTextureRec(texture,texture_rects.car_player[0][steer+2],(Vector2){115,170},WHITE);
      }
   }

   //Update parallax background scroll
   if(!player.stopped&&enable_parallax)
      parallax_scroll(segment_player->curve,dt);
   EndMode2D();
}

static void project_point(Point *p, Fixed1616 cam_x, Fixed1616 cam_y, Fixed2408 cam_z)
{
   Fixed1616 camera_x = -cam_x;
   Fixed1616 camera_y = p->y-cam_y;
   p->camera_z = p->z-cam_z;
   if(p->camera_z==0)
      p->camera_z = Fixed2408_from_int(INT16_MAX);
   p->screen_w = Fixed1616_mul(Fixed1616_div(Fixed1616_from_int(XRES),p->camera_z<<8)*48,CAM_DEPTH);
   p->screen_x = (Fixed1616_from_int(XRES/2)+Fixed1616_mul(Fixed1616_from_int(XRES/2),Fixed1616_mul(Fixed1616_div(camera_x,p->camera_z<<8),CAM_DEPTH)));
   p->screen_y = (Fixed1616_from_int(YRES/2)-Fixed1616_mul(Fixed1616_from_int(YRES/2),Fixed1616_mul(Fixed1616_div(camera_y,p->camera_z<<8),CAM_DEPTH)));
}

static void draw_segment(Segment *s)
{
   if(!s->line)
   {
      Vector2 a,b,c;

      //Grass
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f-XRES/16;
      b.x = 0.0f-XRES/16;
      c.x = XRES+XRES/16;
      DrawTriangle(a,b,c,s->color);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f-XRES/16;
      b.x = XRES+XRES/16;
      c.x = XRES+XRES/16;
      DrawTriangle(a,b,c,s->color);

      //Left border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Road
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      //Right border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
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
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f-XRES/16;
      b.x = 0.0f-XRES/16;
      c.x = XRES+XRES/16;
      DrawTriangle(a,b,c,s->color);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = 0.0f-XRES/16;
      b.x = XRES+XRES/16;
      c.x = XRES+XRES/16;
      DrawTriangle(a,b,c,s->color);

      //Left border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Road
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_road);

      //Right border
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x+s->p1.screen_w-s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x+s->p0.screen_w-s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x+s->p0.screen_w))/65536.0f;
      c.x = ((s->p1.screen_x+s->p1.screen_w))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Left marker 
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+11*s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+11*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+12*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+11*s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+12*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+12*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      //Right marker 
      a.y = ((s->p1.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p1.screen_x-s->p1.screen_w+21*s->p1.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+21*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+22*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);

      a.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      b.y = (Fixed1616_ceil(s->p0.screen_y))/65536.0f;
      c.y = ((s->p1.screen_y))/65536.0f;
      a.x = ((s->p0.screen_x-s->p0.screen_w+21*s->p0.screen_w/16))/65536.0f;
      b.x = ((s->p0.screen_x-s->p0.screen_w+22*s->p0.screen_w/16))/65536.0f;
      c.x = ((s->p1.screen_x-s->p1.screen_w+22*s->p1.screen_w/16))/65536.0f;
      DrawTriangle(a,b,c,s->color_border);
   }
}

static void parallax_scroll(Fixed1616 curve, float dt)
{
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
//-------------------------------------
