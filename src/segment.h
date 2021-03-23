/*
SoftLK-demos - a collection of bigger examples for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _SEGMENT_H_

#define _SEGMENT_H_

typedef struct
{
   ULK_fixed_32 x;
   ULK_fixed_32 y;
   ULK_fixed z;
   ULK_fixed_32 camera_x;
   ULK_fixed_32 camera_y;
   ULK_fixed camera_z;
   ULK_fixed_32 screen_scale;
   ULK_fixed_32 screen_x;
   ULK_fixed_32 screen_y;
   ULK_fixed_32 screen_w;
}Point;

typedef struct
{
   Point p0;
   Point p1;
   Color color;
   Color color_road;
   Color color_border;
   Color color_line;
   int line;
   int texture;
   ULK_fixed_32 curve;
}Segment;

typedef struct
{
   int used;
   int space;
   Segment *segments;
}Segment_list;

extern Segment_list segments;

void     segment_list_init(Segment_list *list);
void     segment_list_add(Segment_list *list, Segment *s);
Segment *segment_list_get_pos(Segment_list *list, ULK_fixed pos, int *index);
Segment *segment_list_get(Segment_list *list, int index);

#endif
