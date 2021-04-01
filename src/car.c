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
static uint8_t car_update_counter = 0;
static uint16_t car_id_counter = 0;
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

Car car_list_remove(Car_list **l, uint16_t id)
{
   if(l==NULL)
      return (Car){0};

   Car_list *in = *l;
   Car_list **last = l;
   Car out = (Car){0};
   while(in)
   {
      if(in->car.id==id)
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

void cars_update()
{
   float dt = sdl_get_delta();
   car_update_counter++;
   for(int i = 0;i<segments.used;i++)
   {
      Segment *s = segment_list_get(&segments,i);
      Car_list *l = s->cars;
      while(l)
      {
         Car_list *next = l->next;
         if(l->car.counter!=car_update_counter)
         {
            l->car.counter++;

            //Steering 
            if(1)
            {
               int j;
               for(j = 1;j<16;j++)
               {
                  Segment *sl = segment_list_get(&segments,j+i);
                  //Steer arround player
                  //Only when faster than player
                  if(sl==segment_player&&l->car.speed>player.vz&&overlap(player.px/65536.0f,texture_rects.car_player[0][0].w*SPRITE_SCALE,l->car.pos_x/65536.0f,texture_rects.car_sprites[l->car.index][0].w*SPRITE_SCALE,1.2f)) //Pointer comparison...
                  {
                     int dir = 0;
                     if(player.px>ULK_fixed_32_from_int(1)/2) dir = -1;
                     else if(player.px<-ULK_fixed_32_from_int(1)/2) dir = 1;
                     else dir = (l->car.pos_x>player.px)?1:-1;
                     //l->car.pos_x+=ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(dir),ULK_fixed_32_from_int(j)),ULK_fixed_32_div((l->car.speed-player.vz)<<8,MAX_SPEED<<8));//ULK_fixed_32_div(l->car.speed<<8,CAR_MAX_SPEED<<8)*dir*2*dt;
                     l->car.pos_x+=ULK_fixed_32_div(l->car.speed<<8,CAR_MAX_SPEED<<8)*dir*dt*2;

                     break;
                  }
                  
                  Car_list *cl = sl->cars;
                  int end = 0;
                  while(cl)
                  {
                     if(l->car.speed>=cl->car.speed&&overlap(cl->car.pos_x/65536.0f,texture_rects.car_sprites[cl->car.index][0].w*SPRITE_SCALE,l->car.pos_x/65536.0f,texture_rects.car_sprites[l->car.index][0].w*SPRITE_SCALE,0.8f))
                     {
                        int dir = 0;
                        if(cl->car.pos_x>ULK_fixed_32_from_int(1)/2) dir = -1;
                        else if(cl->car.pos_x<-ULK_fixed_32_from_int(1)/2) dir = 1;
                        else dir = (l->car.pos_x>cl->car.pos_x)?1:-1;
                        //l->car.pos_x+=ULK_fixed_32_mul(ULK_fixed_32_div(ULK_fixed_32_from_int(dir),ULK_fixed_32_from_int(j)),((-l->car.speed+cl->car.speed)<<8))*dt;
                        l->car.pos_x+=ULK_fixed_32_div(l->car.speed<<8,CAR_MAX_SPEED<<8)*dir*dt;
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
                  if(l->car.pos_x>(ULK_fixed_32_from_int(1)-ULK_fixed_32_from_int(4)/10))
                     l->car.pos_x-=2*ULK_fixed_32_div(l->car.speed,CAR_MAX_SPEED)*dt;
                  else if(l->car.pos_x<-(ULK_fixed_32_from_int(1)-ULK_fixed_32_from_int(4)/10))
                     l->car.pos_x+=2*ULK_fixed_32_div(l->car.speed,CAR_MAX_SPEED)*dt;
               }
            }

            //Update z position/segment
            l->car.z+=l->car.speed*dt;
            if(l->car.z>=SEGLEN)
            {
               l->car.z = l->car.z%SEGLEN;
               Car car = car_list_remove(&s->cars,l->car.id);                
               Segment *n = segment_list_get(&segments,i+1);
               Car_list *cn = car_list_new();
               cn->car = car;
               cn->next = n->cars;
               n->cars = cn;
            }
         }
         l = next;
      }
   }
}

uint16_t cars_id_counter()
{
   return car_id_counter++;
}
//-------------------------------------
