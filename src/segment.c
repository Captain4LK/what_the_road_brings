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
//-------------------------------------

//Internal includes
#include "fixed.h"
#include "config.h"
#include "util.h"
#include "car.h"
#include "segment.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
dyn_array segments;
Segment *segment_player = NULL;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations
Segment *segment_list_get_pos(dyn_array *list, Fixed2408 pos, int *index)
{
   int in = Fixed2408_to_int(Fixed2408_div(pos,SEGLEN))%list->used;
   if(index)
      *index = in;
   return &dyn_array_element(Segment,list,in);
}

Segment *segment_list_get(dyn_array *list, int index)
{
   return &dyn_array_element(Segment,list,index%list->used);
}
//-------------------------------------
