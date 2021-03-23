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
#include "segment.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
Segment_list segments;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void segment_list_init(Segment_list *list)
{
   list->space = 200;
   list->used = 0;
   list->segments = malloc(sizeof(*list->segments)*list->space);
}

void segment_list_add(Segment_list *list, Segment *s)
{
   list->segments[list->used].p0 = s->p0;
   list->segments[list->used].p1 = s->p1;
   list->segments[list->used].color = s->color;
   list->segments[list->used].color_road = s->color_road;
   list->segments[list->used].color_border = s->color_border;
   list->segments[list->used].line = s->line;
   list->segments[list->used].texture = s->texture;
   list->segments[list->used].curve = s->curve;
   list->used++;
   if(list->used==list->space)
   {
      list->space+=200;
      list->segments = realloc(list->segments,sizeof(*list->segments)*list->space);
   }
}

Segment *segment_list_get_pos(Segment_list *list, ULK_fixed pos, int *index)
{
   *index = ULK_fixed_to_int(ULK_fixed_div(pos,SEGLEN))%list->used;
   return &list->segments[*index];
}

Segment *segment_list_get(Segment_list *list, int index)
{
   return &list->segments[index%list->used];
}
//-------------------------------------
