/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "mode.h"
#include "sdl.h"
#include "player.h"
#include "car.h"
#include "draw.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
int mode = 0;
//-------------------------------------

//Function prototypes
static void game_update();
//-------------------------------------

//Function implementations

void modes_update()
{
   switch(mode)
   {
   case 0: //Title screen

      break;
   }
   game_update();
}

static void game_update()
{
   player_update();
   cars_update();

   draw(player.px,player.pz,player.steer);
}
//-------------------------------------
