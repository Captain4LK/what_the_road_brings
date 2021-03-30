/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _CAR_H_

#define _CAR_H_

typedef struct
{
   ULK_fixed_32 pos_x;
   ULK_fixed_32 z;
   uint32_t index;
}Car;

typedef struct Car_list
{
   Car car;
   struct Car_list *next;
}Car_list;

Car_list *car_list_new();
void car_list_free(Car_list *l);

#endif
