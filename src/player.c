/*
SoftLK-demos - a collection of bigger examples for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <SLK/SLK.h>
//-------------------------------------

//Internal includes
#include "../external/ULK_fixed.h"
#include "config.h"
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

   if(SLK_key_down(SLK_KEY_UP)||SLK_gamepad_down(0,SLK_PAD_B))
   {
      if(player.vz<(MAX_SPEED-ACCEL))
         player.vz+=ACCEL;
   }
   else if(SLK_key_down(SLK_KEY_DOWN)||SLK_gamepad_down(0,SLK_PAD_A))
      player.vz+=2*DECEL;
   else
      player.vz+=DECEL;
   if(player.vz<0)
      player.vz = 0;
   player.pz+=player.vz;

   int speed_x = ULK_fixed_32_from_int(4);
   if(SLK_key_down(SLK_KEY_LEFT)||SLK_gamepad_down(0,SLK_PAD_LEFT))
   {
      if(player.steer==0)
         frame = 0;
      player.px-=speed_x;
      if(player.steer<3&&frame%2==0)
         player.steer++;
   }
   else if(SLK_key_down(SLK_KEY_RIGHT)||SLK_gamepad_down(0,SLK_PAD_RIGHT))
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
