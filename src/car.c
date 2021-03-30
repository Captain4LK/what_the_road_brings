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
#include "sdl.h"
#include "car.h"
#include "player.h"
#include "segment.h"
#include "draw.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static Car_list *base_list_cars = NULL;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

Car_list *car_list_new()
{
   if(base_list_cars==NULL)
   {
      Car_list *l = malloc(sizeof(*l));
      l->next = NULL;
      return l;
   }

   Car_list *l = base_list_cars;
   base_list_cars = l->next;
   l->next = NULL;
   return l;
}

void car_list_free(Car_list *l)
{
   l->next = base_list_cars;
   base_list_cars = l;
}
//-------------------------------------
