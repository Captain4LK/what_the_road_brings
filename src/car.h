/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _CAR_H_

#define _CAR_H_

typedef struct Car
{
   //Universall for all cars
   Fixed1616 pos_x;
   Fixed2408 z;
   Fixed2408 speed;
   uint16_t id;
   uint8_t index;
   uint8_t counter;
   uint8_t type;

   //Mostly used for opponent cars
   int segment;
   int lap;

   //Internal use, do not touch
   struct Car *next;
}Car;

typedef struct Car_list
{
   Car *car;
   struct Car_list *next;
}Car_list;

extern Car_list *cars_opponents;

Car_list *car_list_new();
void car_list_free(Car_list *l);
Car *car_list_remove(Car_list **l, uint16_t id);

Car *car_new();
void car_free(Car *l);

void cars_update(uint8_t op);
uint16_t cars_id_counter();

#endif
