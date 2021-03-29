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
#include <SDL2/SDL.h>
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "sdl.h"
#include "player.h"
#include "draw.h"
#include "segment.h"
//-------------------------------------

//#defines
#define MIN(a,b) \
   ((a)<(b)?(a):(b))
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
struct Player player;
static int frame = 0;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void player_update()
{
   float dt = sdl_get_delta();
   ULK_fixed_32 vz_acc = ACCEL*dt;
   ULK_fixed_32 vz_dec = DECEL*dt;
   player.stopped = 0;

   //Collision
   SDL_SetRenderDrawColor(renderer,0,0,0,255);
   int i;
   Segment *bp = segment_list_get_pos(&segments,player.pz+ULK_fixed_from_int(64),&i);
   for(int j = 0;j<bp->sprites.used;j++)
   {
      Sprite *sp = &dyn_array_element(Sprite,&bp->sprites,j);
      SDL_Rect dst;
      dst.w = texture_rects.sprites[sp->index].w*((float)CAM_DEPTH/(float)bp->p1.camera_z);
      dst.x = ((float)bp->p1.screen_x/65536.0f)+((float)CAM_DEPTH/(float)bp->p1.camera_z)*((float)sp->pos/(float)65536.0f)*(XRES/4)-dst.w/2.0f;
      if((145+texture_rects.car_player[0][0].w-60)>dst.x&&145<dst.x+dst.w)
         player.stopped = 1;
   }
   if(!player.stopped)
      player.px-=ULK_fixed_32_mul(ULK_fixed_32_div(player.vz,MAX_SPEED),ULK_fixed_32_mul(bp->curve,ULK_fixed_32_from_int(2)))*dt;

   frame++;
   if(sdl_key_down(KEY_UP)||sdl_gamepad_down(0,PAD_B))
   {
      if(player.vz<(MAX_SPEED))
         player.vz+=MIN(MAX_SPEED-player.vz,vz_acc);
   }
   else if(sdl_key_down(KEY_DOWN)||sdl_gamepad_down(0,PAD_A))
      player.vz+=2*vz_dec;
   else
      player.vz+=vz_dec;
   if(player.vz>MAX_SPEED/4&&abs(player.px)>ULK_fixed_32_from_int(1))
      player.vz+=2*vz_dec;
   if(player.vz<0)
      player.vz = 0;

   if(!player.stopped)
      player.pz+=player.vz*dt;

   ULK_fixed_32 speed_x = 2*ULK_fixed_32_div(player.vz,MAX_SPEED)*dt;
   if(sdl_key_down(KEY_LEFT)||sdl_gamepad_down(0,PAD_LEFT))
   {
      if(player.steer==0)
         frame = 0;
      player.px-=speed_x;
      if(player.steer<2&&frame%6==0)
         player.steer++;
   }
   else if(sdl_key_down(KEY_RIGHT)||sdl_gamepad_down(0,PAD_RIGHT))
   {
      if(player.steer==0)
         frame = 0;
      player.px+=speed_x;
      if(player.steer>-2&&frame%6==0)
         player.steer--;
   } 
   else if(player.steer!=0&&frame%6==0)
   {
      if(player.steer<0)
         player.steer++;
      else
         player.steer--;
   }

   player.pz = player.pz%(segments.used*SEGLEN);
}
//-------------------------------------
