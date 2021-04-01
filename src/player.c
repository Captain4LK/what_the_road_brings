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
#include "car.h"
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
   segment_player = segment_list_get_pos(&segments,player.pz+ULK_fixed_from_int(64),&i);
   for(int j = 0;j<segment_player->sprites.used;j++)
   {
      Sprite *sp = &dyn_array_element(Sprite,&segment_player->sprites,j);
      float width_0 = (float)texture_rects.car_player[0][0].w*SPRITE_SCALE;
      float width_1 = (float)texture_rects.sprites[sp->index].w*SPRITE_SCALE;

      if(overlap((float)player.px/65536.0f,width_0,(float)(sp->pos/65536.0f)+(sp->pos>0?1:-1)*(width_1/2.0f),width_1,0.5))
         player.stopped = 1;
   }
   Car_list *cl = segment_player->cars;
   while(cl)
   {
      if(player.vz>cl->car.speed&&overlap((float)player.px/65536.0f,(float)texture_rects.car_player[0][0].w*SPRITE_SCALE,(float)cl->car.pos_x/65536.0f,(float)texture_rects.car_sprites[cl->car.index][0].w*SPRITE_SCALE,0.3f))
      {
         player.vz = ULK_fixed_mul(cl->car.speed,ULK_fixed_div(cl->car.speed,player.vz));
      }

      cl = cl->next;
   }

   if(!player.stopped)
      player.px-=ULK_fixed_32_mul(ULK_fixed_32_div(player.vz,MAX_SPEED),ULK_fixed_32_mul(segment_player->curve,ULK_fixed_32_from_int(2)))*dt;

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
   segment_player = segment_list_get_pos(&segments,player.pz+ULK_fixed_from_int(64),&i);
}
//-------------------------------------
