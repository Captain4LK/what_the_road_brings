/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _SEGMENT_H_

#define _SEGMENT_H_

typedef struct
{
   ULK_fixed_32 y;
   ULK_fixed z;
   ULK_fixed camera_z;
   ULK_fixed_32 screen_x;
   ULK_fixed_32 screen_y;
   ULK_fixed_32 screen_w;
}Point;

typedef struct
{
   ULK_fixed_32 pos;
   uint32_t index;
}Sprite;

typedef struct
{
   Point p0;
   Point p1;
   Color color;
   Color color_road;
   Color color_border;
   uint8_t line;
   ULK_fixed_32 curve;
   ULK_fixed_32 clip_y;
   dyn_array sprites;
   Car_list *cars;
}Segment;

extern dyn_array segments;
extern Segment *segment_player;

Segment *segment_list_get_pos(dyn_array *list, ULK_fixed pos, int *index);
Segment *segment_list_get(dyn_array *list, int index);

#endif
