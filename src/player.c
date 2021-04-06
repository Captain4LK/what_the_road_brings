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
#include <raylib.h>
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "audio.h"
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

void player_reset()
{
   player.pz = segments.used*SEGLEN-GOAL_POS;
   player.px = 0;
   player.vz = 0;
   player.steer = 0;
   player.stopped = 0;
   player.lap = 0;
}

void player_update()
{
   float dt = GetFrameTime();
   ULK_fixed_32 vz_acc = ACCEL*dt;
   ULK_fixed_32 vz_dec = DECEL*dt;

   //Collision
   int i;
   segment_player = segment_list_get_pos(&segments,player.pz+PLAYER_OFFSET,&i);
   int stop = player.stopped;
   player.stopped = 0;
   for(int j = 0;j<segment_player->sprites.used;j++)
   {
      Sprite *sp = &dyn_array_element(Sprite,&segment_player->sprites,j);
      float width_0 = (float)texture_rects.car_player[0][0].width*SPRITE_SCALE;
      float width_1 = (float)texture_rects.sprites[sp->index].width*SPRITE_SCALE;

      if(overlap((float)player.px/65536.0f,width_0,(float)(sp->pos/65536.0f)+(sp->pos>0?1.5f:-1.5f)*(width_1/2.0f),width_1,0.5))
      {
         if(!stop)
            PlaySound(sound_hit);

         player.stopped = 1;
         if(sp->type==1)
            player.stopped = 2+(player.px>0);
      }
   }
   Car_list *cl = segment_player->cars;
   while(cl)
   {
      if(player.vz>cl->car->speed&&overlap((float)player.px/65536.0f,(float)texture_rects.car_player[0][0].width*SPRITE_SCALE,(float)cl->car->pos_x/65536.0f,(float)texture_rects.car_sprites[cl->car->index][0].width*SPRITE_SCALE,0.3f))
      {
         player.vz = ULK_fixed_mul(cl->car->speed,ULK_fixed_div(cl->car->speed,player.vz));
         PlaySound(sound_hit);
      }

      cl = cl->next;
   }
   //-------------------------------------

   if(player.stopped!=1)
   {
      if(segment_player->curve<0&&player.stopped!=3)
         player.px-=ULK_fixed_32_mul(ULK_fixed_32_div(player.vz,MAX_SPEED),ULK_fixed_32_mul(segment_player->curve,ULK_fixed_32_from_int(2)))*dt;
      else if(segment_player->curve>0&&player.stopped!=2)
         player.px-=ULK_fixed_32_mul(ULK_fixed_32_div(player.vz,MAX_SPEED),ULK_fixed_32_mul(segment_player->curve,ULK_fixed_32_from_int(2)))*dt;
   }

   frame++;
   if(IsKeyDown(KEY_UP))
   {
      if(player.vz<(MAX_SPEED))
         player.vz+=MIN(MAX_SPEED-player.vz,vz_acc);
   }
   else if(IsKeyDown(KEY_DOWN))
      player.vz+=2*vz_dec;
   else
      player.vz+=vz_dec;
   if(player.vz>MAX_SPEED/4&&abs(player.px)>=ULK_fixed_32_from_int(1)-ULK_fixed_32_from_int(1)/10)
      player.vz+=4*vz_dec;
   if(player.vz<0)
      player.vz = 0;

   ULK_fixed old_z = player.pz;
   if(player.stopped!=1)
      player.pz+=player.vz*dt;
   player.time+=ULK_fixed_32_from_int(1)*dt;
   player.pz = player.pz%(segments.used*SEGLEN);
   if(old_z>player.pz)
   {
      player.time = 0;
      player.lap++;
   }

   if(player.vz==0&&IsMusicPlaying(sound_drive))
      StopMusicStream(sound_drive);
   else if(player.vz>0&&!IsMusicPlaying(sound_drive))
      PlayMusicStream(sound_drive);
   SetMusicPitch(sound_drive,0.5f+((float)player.vz/(float)MAX_SPEED));

   ULK_fixed_32 speed_x = 3*MIN(ULK_fixed_32_from_int(1),ULK_fixed_32_div(player.vz,MAX_SPEED/3))*dt;
   if(IsKeyDown(KEY_LEFT)&&player.stopped!=2)
   {
      if(player.steer==0)
         frame = 0;
      player.px-=speed_x;
      if(player.steer<2&&frame%6==0)
         player.steer++;
   }
   else if(IsKeyDown(KEY_RIGHT)&&player.stopped!=3)
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

   segment_player = segment_list_get_pos(&segments,player.pz+ULK_fixed_from_int(64),&i);
}

int player_pos()
{
   int pos = 8;
   Car_list *l = cars_opponents;
   while(l)
   {
      if(player.lap>l->car->lap)
      {
         pos--;
      }
      else if(player.lap==l->car->lap)
      {
         if(player.pz+PLAYER_OFFSET>l->car->segment*SEGLEN+l->car->z)
            pos--;
      }
      l = l->next;
   }

   return pos;
}
//-------------------------------------
