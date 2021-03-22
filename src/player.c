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
   if(sdl_key_down(KEY_UP))
   {
      if(player.vz<(MAX_SPEED-ACCEL))
         player.vz+=ACCEL;
   }
   else if(sdl_key_down(KEY_DOWN))
      player.vz+=2*DECEL;
   else
      player.vz+=DECEL;
   if(player.vz<0)
      player.vz = 0;
   player.pz+=player.vz;

   int speed_x = ULK_fixed_32_from_int(4);
   if(sdl_key_down(KEY_LEFT))
   {
      if(player.steer==0)
         frame = 0;
      player.px-=speed_x;
      if(player.steer<3&&frame%2==0)
         player.steer++;
   }
   else if(sdl_key_down(KEY_RIGHT))
   {
      if(player.steer==0)
         frame = 0;
      player.px+=speed_x;
      if(player.steer>-3&&frame%2==0)
         player.steer--;
   } 
   else if(player.steer!=0&&frame%2==0)
   {
      if(player.steer<0)
         player.steer++;
      else
         player.steer--;
   }

   player.pz = player.pz%(segments.used*SEGLEN);
}
//-------------------------------------
//player.vz-=ACCEL;
