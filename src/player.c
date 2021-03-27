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
#include "util.h"
#include "sdl.h"
#include "player.h"
#include "segment.h"
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

void player_update()
{
   frame++;
   if(sdl_key_down(KEY_UP)||sdl_gamepad_down(0,PAD_B))
   {
      if(player.vz<(MAX_SPEED-ACCEL))
         player.vz+=ACCEL;
   }
   else if(sdl_key_down(KEY_DOWN)||sdl_gamepad_down(0,PAD_A))
      player.vz+=2*DECEL;
   else
      player.vz+=DECEL;
   if(player.vz<0)
      player.vz = 0;
   player.pz+=player.vz;

   ULK_fixed_32 speed_x = 2*ULK_fixed_32_div(player.vz,MAX_SPEED);
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
