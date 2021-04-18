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

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "fixed.h"
#include "config.h"
#include "util.h"
#include "audio.h"
#include "car.h"
#include "player.h"
#include "segment.h"
#include "input.h"
#include "track.h"
#include "texture.h"
//-------------------------------------

//#defines
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
   player.collisions = 0;
   player.time = 0;
}

void player_update(int controll)
{
   float dt = GetFrameTime();
   Fixed1616 vz_acc = ACCEL*dt;
   Fixed1616 vz_dec = DECEL*dt;

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

      if(overlap((float)player.px/65536.0f,width_0,(float)(sp->pos/65536.0f)+(sp->pos>0?1.0f:-1.0f)*(width_1/2.0f),width_1,texture_rects.sprites_col_scale[sp->index]))
      {
         if(!stop)
         {
            PlaySound(sound_hit);
            player.collisions++;
         }

         if(sp->type==1)
            player.stopped = 2+(player.px>0);
         else if(sp->type==2)
         {
            if(player.vz<MAX_SPEED)
               PlaySound(sound_boost);
            player.vz = MAX_SPEED_BOOST;
         }
         else
            player.stopped = 1;
      }
   }
   Car_list *cl = segment_player->cars;
   while(cl)
   {
      if(player.vz>cl->car->speed&&overlap((float)player.px/65536.0f,(float)texture_rects.car_player[0][0].width*SPRITE_SCALE,(float)cl->car->pos_x/65536.0f,(float)texture_rects.car_sprites[cl->car->index][0].width*SPRITE_SCALE,0.3f))
      {
         player.vz = Fixed2408_mul(cl->car->speed,Fixed2408_div(cl->car->speed,player.vz));
         PlaySound(sound_hit);
         player.collisions++;
      }

      cl = cl->next;
   }
   //-------------------------------------

   if(player.stopped!=1)
   {
      if(segment_player->curve<0&&player.stopped!=3)
         player.px-=Fixed1616_mul(Fixed1616_div(player.vz,MAX_SPEED),Fixed1616_mul(segment_player->curve,Fixed1616_from_int(2)))*dt;
      else if(segment_player->curve>0&&player.stopped!=2)
         player.px-=Fixed1616_mul(Fixed1616_div(player.vz,MAX_SPEED),Fixed1616_mul(segment_player->curve,Fixed1616_from_int(2)))*dt;
   }

   frame++;
   if(controll&&input_down_accel())
   {
      if(player.vz<(MAX_SPEED))
         player.vz+=MIN(MAX_SPEED-player.vz,vz_acc);
   }
   else if(controll&&input_down_decel())
      player.vz+=2*vz_dec;
   else
      player.vz+=vz_dec;
   if(player.vz>MAX_SPEED/4&&abs(player.px)>=Fixed1616_from_int(1)-Fixed1616_from_int(1)/10)
      player.vz+=4*vz_dec;
   if(player.vz<0)
      player.vz = 0;

   Fixed2408 old_z = player.pz;
   //Peak floating point...
   //I should really start explicetely casting between int and float...
   if(player.stopped!=1)
      player.pz+=(Fixed1616)(player.vz*dt);
   player.time+=Fixed1616_from_int(1)*dt;
   player.pz = player.pz%(segments.used*SEGLEN);
   if(old_z>player.pz)
   {
      if(player.lap!=0)
         lap_times[player.lap-1] = player.time;
      player.time = 0;
      player.lap++;
   }

   if(player.vz==0&&IsMusicPlaying(sound_drive))
      StopMusicStream(sound_drive);
   else if(player.vz>0&&!IsMusicPlaying(sound_drive))
      PlayMusicStream(sound_drive);
   SetMusicPitch(sound_drive,0.5f+((float)player.vz/(float)MAX_SPEED));

   Fixed1616 speed_x = 3*MIN(Fixed1616_from_int(1),Fixed1616_div(player.vz,MAX_SPEED/3))*dt;
   if(controll&&input_down_steer_left()&&player.stopped!=2)
   {
      if(player.steer==0)
         frame = 0;
      player.px-=speed_x*input_steer_left();
      if(player.steer<2&&frame%6==0)
         player.steer++;
   }
   else if(controll&&input_down_steer_right()&&player.stopped!=3)
   {
      if(player.steer==0)
         frame = 0;
      player.px+=speed_x*input_steer_right();
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

   segment_player = segment_list_get_pos(&segments,player.pz+Fixed2408_from_int(64),&i);
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
