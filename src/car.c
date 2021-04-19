/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "fixed.h"
#include "config.h"
#include "util.h"
#include "car.h"
#include "player.h"
#include "segment.h"
#include "texture.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static Car_list *base_cars_list = NULL;
static Car *base_cars = NULL;
//TODO: reset on track load
static uint8_t car_update_counter = 0;
static uint16_t car_id_counter = 0;

Car_list *cars_opponents = NULL;
//-------------------------------------

//Function prototypes
static void car_npc_update(Segment *s, Car_list *l, int seg, float dt);
static void car_opp_update(Segment *s, Car_list *l, int seg, float dt);
//-------------------------------------

//Function implementations

Car_list *car_list_new()
{
   if(base_cars_list==NULL)
   {
      Car_list *l = malloc(sizeof(*l));
      l->next = NULL;
      return l;
   }

   Car_list *l = base_cars_list;
   base_cars_list = l->next;
   l->next = NULL;
   return l;
}

void car_list_free(Car_list *l)
{
   l->next = base_cars_list;
   base_cars_list = l;
}

Car *car_list_remove(Car_list **l, uint16_t id)
{
   if(l==NULL)
      return NULL;

   Car_list *in = *l;
   Car_list **last = l;
   Car *out = NULL;
   while(in)
   {
      if(in->car->id==id)
      {
         *last = in->next;
         out = in->car;
         car_list_free(in);
         break;
      }

      last = &in->next;
      in = in->next;
   }

   return out;
}

Car *car_new()
{
   if(base_cars==NULL)
   {
      Car *l = malloc(sizeof(*l));
      l->next = NULL;
      return l;
   }

   Car *l = base_cars;
   base_cars = l->next;
   l->next = NULL;
   return l;
}

void car_free(Car *l)
{
   l->next = base_cars;
   base_cars = l;
}

void cars_update(uint8_t op, float dt)
{
   //The counter is used to keep track of which cars have already been updated this frame
   //It intensionally overflows, since only the difference between two numbers matters
   car_update_counter++;

   for(int i = 0;i<segments.used;i++)
   {
      Segment *s = segment_list_get(&segments,i);
      Car_list *l = s->cars;
      while(l)
      {
         //Store the next element, since the current
         //element might be deleted during updating
         Car_list *next = l->next;

         if(l->car->type==0)
            car_npc_update(s,l,i,dt);
         else if(l->car->type==1&&op)
            car_opp_update(s,l,i,dt);

         l = next;
      }
   }
}

static void car_npc_update(Segment *s, Car_list *l, int seg, float dt)
{
   //Only update car if not updated already
   //Cars may be accessed twice since they can
   //be moved forward one segment
   if(l->car->counter!=car_update_counter)
   {
      l->car->counter++;

      //Steering 
      //Only when close to the player
      //TODO
      if(1)
      {
         int j;
         for(j = 1;j<16;j++)
         {
            //Steer around player
            //Only when faster than player
            //Steering around player has the highes priority, 
            //since a car driving through the player would
            //look very weird
            Segment *sl = segment_list_get(&segments,j+seg);
            if(sl==segment_player&&l->car->speed>player.vz //Pointer comparison...
               &&overlap(player.px/65536.0f,texture_rects.car_player[0][0].width*SPRITE_SCALE,l->car->pos_x/65536.0f,texture_rects.car_sprites[l->car->index][0].width*SPRITE_SCALE,1.0f))
            {
               //Calculate steering direction
               int dir = 0;
               if(player.px>Fixed1616_from_int(1)/3) dir = -1;
               else if(player.px<-Fixed1616_from_int(1)/3) dir = 1;
               else dir = (l->car->pos_x>player.px)?1:-1;

               l->car->pos_x+=Fixed1616_div(l->car->speed<<8,CAR_MAX_SPEED<<8)*dir*dt*4;
               break;
            }
            
            Car_list *cl = sl->cars;
            int end = 0;
            while(cl)
            {
               if(l->car->speed>=cl->car->speed&&overlap(cl->car->pos_x/65536.0f,texture_rects.car_sprites[cl->car->index][0].width*SPRITE_SCALE,l->car->pos_x/65536.0f,texture_rects.car_sprites[l->car->index][0].width*SPRITE_SCALE,0.9f))
               {
                  //Calculate steering direction
                  int dir = 0;
                  if(cl->car->pos_x>Fixed1616_from_int(1)/3) dir = -1;
                  else if(cl->car->pos_x<-Fixed1616_from_int(1)/3) dir = 1;
                  else dir = (l->car->pos_x>cl->car->pos_x)?1:-1;

                  l->car->pos_x+=Fixed1616_div(l->car->speed<<8,CAR_MAX_SPEED<<8)*dir*dt;
                  end = 1;
                  break;
               }

               cl = cl->next;
            }
            if(end)
               break;
         }
         if(j==16) //Nothing found
         {
            if(l->car->pos_x>(Fixed1616_from_int(1)-Fixed1616_from_int(3)/10))
               l->car->pos_x-=2*Fixed1616_div(l->car->speed,CAR_MAX_SPEED)*dt;
            else if(l->car->pos_x<-(Fixed1616_from_int(1)-Fixed1616_from_int(3)/10))
               l->car->pos_x+=2*Fixed1616_div(l->car->speed,CAR_MAX_SPEED)*dt;
         }
      }

      //Update z position/segment
      l->car->z+=l->car->speed*dt;
      if(l->car->z>=SEGLEN)
      {
         l->car->z = l->car->z%SEGLEN;
         Car *car = car_list_remove(&s->cars,l->car->id);               
         Segment *n = segment_list_get(&segments,seg+1);
         car->segment = seg+1;
         Car_list *cn = car_list_new();
         cn->car = car;
         cn->next = n->cars;
         n->cars = cn;
      }
   }
}

static void car_opp_update(Segment *s, Car_list *l, int seg, float dt)
{
   //Only update car if not updated already
   //Cars may be accessed twice since they can
   //be moved forward one segment
   if(l->car->counter!=car_update_counter)
   {
      l->car->counter++;

      //Steering 
      //Only when close to the player
      //TODO
      if(1)
      {
         int j;
         for(j = 1;j<16;j++)
         {
            //Steer around player
            //Only when faster than player
            //Steering around player has the highes priority, 
            //since a car driving through the player would
            //look very weird
            Segment *sl = segment_list_get(&segments,j+seg);
            if(sl==segment_player&&l->car->speed>player.vz //Pointer comparison...
               &&overlap(player.px/65536.0f,texture_rects.car_player[0][0].width*SPRITE_SCALE,l->car->pos_x/65536.0f,texture_rects.car_sprites[l->car->index][0].width*SPRITE_SCALE,1.0f))
            {
               //Calculate steering direction
               int dir = 0;
               if(player.px>Fixed1616_from_int(1)/3) dir = -1;
               else if(player.px<-Fixed1616_from_int(1)/3) dir = 1;
               else dir = (l->car->pos_x>player.px)?1:-1;

               l->car->pos_x+=Fixed1616_div(l->car->speed<<8,CAR_MAX_SPEED<<8)*dir*dt*4;
               break;
            }
            
            Car_list *cl = sl->cars;
            int end = 0;
            while(cl)
            {
               if(l->car->speed>=cl->car->speed&&overlap(cl->car->pos_x/65536.0f,texture_rects.car_sprites[cl->car->index][0].width*SPRITE_SCALE,l->car->pos_x/65536.0f,texture_rects.car_sprites[l->car->index][0].width*SPRITE_SCALE,0.9f))
               {
                  //Calculate steering direction
                  int dir = 0;
                  if(cl->car->pos_x>Fixed1616_from_int(1)/3) dir = -1;
                  else if(cl->car->pos_x<-Fixed1616_from_int(1)/3) dir = 1;
                  else dir = (l->car->pos_x>cl->car->pos_x)?1:-1;

                  l->car->pos_x+=Fixed1616_div(l->car->speed<<8,CAR_MAX_SPEED<<8)*dir*dt;
                  end = 1;
                  break;
               }

               cl = cl->next;
            }
            if(end)
               break;
         }
         if(j==16) //Nothing found
         {
            if(l->car->pos_x>(Fixed1616_from_int(1)-Fixed1616_from_int(3)/10))
               l->car->pos_x-=2*Fixed1616_div(l->car->speed,CAR_MAX_SPEED)*dt;
            else if(l->car->pos_x<-(Fixed1616_from_int(1)-Fixed1616_from_int(3)/10))
               l->car->pos_x+=2*Fixed1616_div(l->car->speed,CAR_MAX_SPEED)*dt;
         }
      }

      //Update z position/segment
      Fixed2408 old_z = l->car->z+l->car->segment*SEGLEN;
      l->car->z+=l->car->speed*dt;

      if(old_z>((l->car->z+l->car->segment*SEGLEN)%(segments.used*SEGLEN)))
         l->car->lap++;
      if(l->car->z>=SEGLEN)
      {
         l->car->z = l->car->z%SEGLEN;
         Car *car = car_list_remove(&s->cars,l->car->id);               
         Segment *n = segment_list_get(&segments,seg+1);
         car->segment = (seg+1)%segments.used;
         Car_list *cn = car_list_new();
         cn->car = car;
         cn->next = n->cars;
         n->cars = cn;
      }
   }
}

uint16_t cars_id_counter()
{
   return car_id_counter++;
}
//-------------------------------------
