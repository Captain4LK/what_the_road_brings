/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "fixed.h"
#include "config.h"
#include "util.h"
#include "track.h"
#include "car.h"
#include "segment.h"
#include "player.h"
#include "mode.h"
#include "audio.h"
#include "texture.h"
#include "lil.h"
//-------------------------------------

//#defines
#define COLOR_ROAD() \
   { if((segments.used%10)<5) {s.color = colors[0];s.color_road = colors[4];s.color_border = colors[2];s.line = segments.used%5>1;}else{s.color = colors[1];s.color_road = colors[5];s.color_border = colors[3];s.line = 0;}}
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
static Color colors[7];

Track track;
Fixed1616 *lap_times = NULL;
//-------------------------------------

//Function prototypes
static LILCALLBACK lil_value_t fnc_add_road(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_add_sprite(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_add_stripe(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_add_car(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_rand(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_fixed_mul(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_get_segments_used(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_music(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_laps(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_texture(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_player_sprite(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_backdrop_sprite(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_sprite(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_car_sprite(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_color(lil_t lil, size_t argc, lil_value_t* argv);
static LILCALLBACK lil_value_t fnc_set_backdrop(lil_t lil, size_t argc, lil_value_t* argv);

static void add_road(int start, int end, int length, Fixed1616 curve, Fixed1616 hill);
static void add_sprite(int seg, int index, Fixed1616 pos, uint8_t type);
static void add_car(int seg, int index, Fixed1616 pos, uint8_t type, Fixed2408 speed);
static void add_stripe(int start, int end);
//-------------------------------------

//Function implementations

void track_build()
{
   srand(time(NULL));
   track.laps = 3;

   //Setup road
   //Free previous road
   for(int i = 0;i<segments.used;i++)
   {
      Segment *s = &dyn_array_element(Segment,&segments,i);
      dyn_array_free(Sprite,&s->sprites);
      Car_list *l = s->cars;
      while(l)
      {
         Car_list *next = l->next;
         car_free(l->car);
         car_list_free(l);

         l = next;
      }
   }
   Car_list *l = cars_opponents;
   while(l)
   {
      Car_list *next = l->next;
      car_list_free(l);

      l = next;
   }
   cars_opponents = NULL;
   if(lap_times)
      free(lap_times);
   lap_times = NULL;
   dyn_array_clear(Segment,&segments);

   lil_t lil = lil_new();

   const char* filename = "data/tracks/track_00.lil";
   const char* err_msg;
   size_t pos;

   //lil functions
   lil_register(lil,"ULK_fixed_mul",fnc_fixed_mul);
   lil_register(lil,"add_road",fnc_add_road);
   lil_register(lil,"add_sprite",fnc_add_sprite);
   lil_register(lil,"add_stripe",fnc_add_stripe);
   lil_register(lil,"add_car",fnc_add_car);
   lil_register(lil,"get_segments_used",fnc_get_segments_used);
   lil_register(lil,"rand",fnc_rand);
   lil_register(lil,"set_music",fnc_set_music);
   lil_register(lil,"set_laps",fnc_set_laps);
   lil_register(lil,"set_texture",fnc_set_texture);
   lil_register(lil,"set_player_sprite",fnc_set_player_sprite);
   lil_register(lil,"set_backdrop_sprite",fnc_set_backdrop_sprite);
   lil_register(lil,"set_sprite",fnc_set_sprite);
   lil_register(lil,"set_car_sprite",fnc_set_car_sprite);
   lil_register(lil,"set_color",fnc_set_color);
   lil_register(lil,"set_backdrop",fnc_set_backdrop);

   //lil constants
   lil_set_var(lil,"CAR_MAX_SPEED",lil_alloc_integer(CAR_MAX_SPEED),LIL_SETVAR_GLOBAL);

   char *tmpcode = malloc(strlen(filename) + 256);
   sprintf(tmpcode, "set __lilmain:code__ [read {%s}]\nif [streq $__lilmain:code__ ''] {print There is no code in the file or the file does not exist} {eval $__lilmain:code__}\n", filename);
   lil_value_t result = lil_parse(lil,tmpcode,0,1);
   free(tmpcode);
   if(lil_error(lil,&err_msg,&pos))
        printf("lil: error at %i: %s\n",(int)pos,err_msg);
   lil_free_value(result);
   lil_free(lil);

   segment_list_get_pos(&segments,0,NULL)->color_road = WHITE;

   player_reset();
   lap_times = malloc(sizeof(*lap_times)*track.laps);

   clear_texture = colors[6];
}

static LILCALLBACK lil_value_t fnc_add_road(lil_t lil, size_t argc, lil_value_t* argv)
{
   if(argc<5)
   {
      lil_set_error(lil,"Not enough arguments to function add_road");
      return NULL;
   }
   add_road(lil_to_integer(argv[0]),lil_to_integer(argv[1]),lil_to_integer(argv[2]),lil_to_integer(argv[3]),lil_to_integer(argv[4]));

   return NULL;
}

static LILCALLBACK lil_value_t fnc_add_sprite(lil_t lil, size_t argc, lil_value_t* argv)
{
   if(argc<4)
   {
      lil_set_error(lil,"Not enough arguments to function add_sprite");
      return NULL;
   }
   add_sprite(lil_to_integer(argv[0]),lil_to_integer(argv[1]),lil_to_integer(argv[2]),lil_to_integer(argv[3]));

   return NULL;
}

static LILCALLBACK lil_value_t fnc_add_stripe(lil_t lil, size_t argc, lil_value_t* argv)
{
   if(argc<2)
   {
      lil_set_error(lil,"Not enough arguments to function add_stripe");
      return NULL;
   }
   add_stripe(lil_to_integer(argv[0]),lil_to_integer(argv[1]));

   return NULL;
}

static LILCALLBACK lil_value_t fnc_add_car(lil_t lil, size_t argc, lil_value_t* argv)
{
   if(argc<5)
   {
      lil_set_error(lil,"Not enough arguments to function add_dar");
      return NULL;
   }
   add_car(lil_to_integer(argv[0]),lil_to_integer(argv[1]),lil_to_integer(argv[2]),lil_to_integer(argv[3]),lil_to_integer(argv[4]));

   return NULL;
}

static LILCALLBACK lil_value_t fnc_rand(lil_t lil, size_t argc, lil_value_t* argv)
{
   return lil_alloc_integer(rand());
}

static LILCALLBACK lil_value_t fnc_get_segments_used(lil_t lil, size_t argc, lil_value_t* argv)
{
   return lil_alloc_integer(segments.used);
}

static LILCALLBACK lil_value_t fnc_fixed_mul(lil_t lil, size_t argc, lil_value_t* argv)
{
   return lil_alloc_integer(Fixed2408_mul(lil_to_integer(argv[0]),lil_to_integer(argv[1])));
}

static LILCALLBACK lil_value_t fnc_set_music(lil_t lil, size_t argc, lil_value_t* argv)
{
   audio_set_path(1,lil_to_string(argv[0]));
   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_laps(lil_t lil, size_t argc, lil_value_t* argv)
{
   track.laps = lil_to_integer(argv[0]);
   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_texture(lil_t lil, size_t argc, lil_value_t* argv)
{
   texture_set(lil_to_string(argv[0]));
   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_player_sprite(lil_t lil, size_t argc, lil_value_t* argv)
{
   Rectangle *r = &texture_rects.car_player[lil_to_integer(argv[0])][lil_to_integer(argv[1])];
   r->x = lil_to_integer(argv[2]);
   r->y = lil_to_integer(argv[3]);
   r->width = lil_to_integer(argv[4]);
   r->height = lil_to_integer(argv[5]);

   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_backdrop_sprite(lil_t lil, size_t argc, lil_value_t* argv)
{
   Rectangle *r = &texture_rects.backdrop[lil_to_integer(argv[0])];
   r->x = lil_to_integer(argv[1]);
   r->y = lil_to_integer(argv[2]);
   r->width = lil_to_integer(argv[3]);
   r->height = lil_to_integer(argv[4]);

   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_sprite(lil_t lil, size_t argc, lil_value_t* argv)
{
   Rectangle *r = &texture_rects.sprites[lil_to_integer(argv[0])];
   r->x = lil_to_integer(argv[1]);
   r->y = lil_to_integer(argv[2]);
   r->width = lil_to_integer(argv[3]);
   r->height = lil_to_integer(argv[4]);
   texture_rects.sprites_col_scale[lil_to_integer(argv[0])] = lil_to_double(argv[5]);

   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_car_sprite(lil_t lil, size_t argc, lil_value_t* argv)
{
   Rectangle *r = &texture_rects.car_sprites[lil_to_integer(argv[0])][lil_to_integer(argv[1])];
   r->x = lil_to_integer(argv[2]);
   r->y = lil_to_integer(argv[3]);
   r->width = lil_to_integer(argv[4]);
   r->height = lil_to_integer(argv[5]);

   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_color(lil_t lil, size_t argc, lil_value_t* argv)
{
   Color *c = &colors[lil_to_integer(argv[0])];
   c->r = lil_to_integer(argv[1]);
   c->g = lil_to_integer(argv[2]);
   c->b = lil_to_integer(argv[3]);
   c->a = lil_to_integer(argv[4]);

   return NULL;
}

static LILCALLBACK lil_value_t fnc_set_backdrop(lil_t lil, size_t argc, lil_value_t* argv)
{
   int index = lil_to_integer(argv[0]);
   parallax_data.layers[index][0].x = lil_to_integer(argv[1]);
   parallax_data.layers[index][0].y = lil_to_integer(argv[2]);
   parallax_data.layers[index][0].width = lil_to_integer(argv[3]);
   parallax_data.layers[index][0].height = lil_to_integer(argv[4]);
   parallax_data.speed[index] = lil_to_double(argv[5]);
   parallax_data.layers[index][1].x = parallax_data.layers[index][0].width;
   parallax_data.layers[index][1].y = parallax_data.layers[index][0].y;
   parallax_data.layers[index][1].width = parallax_data.layers[index][0].width;
   parallax_data.layers[index][1].height = parallax_data.layers[index][0].height;
   return NULL;
}

static void add_stripe(int start, int end)
{
   for(int i = start;i<=end;i++)
   {
      segment_list_get(&segments,i)->color = colors[4];
      segment_list_get(&segments,i)->color_border = colors[4];
   }
}

static void add_road(int start, int end, int length, Fixed1616 curve, Fixed1616 hill)
{

   Fixed1616 start_y = segments.used?dyn_array_element(Segment,&segments,segments.used-1).p1.y:0;
   Fixed1616 end_y = start_y+hill;
   int total = start+end+length;

   for(int i = 0;i<start;i++)
   {
      Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      Fixed1616 ly = segments.used?dyn_array_element(Segment,&segments,segments.used-1).p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+Fixed1616_mul(end_y-start_y,((-cos(((float)i/(float)total)*M_PI)*Fixed1616_from_int(1))/2)+Fixed1616_from_int(1)/2);
      COLOR_ROAD()
      s.curve = Fixed1616_mul(curve,Fixed1616_mul(Fixed1616_div(Fixed1616_from_int(i),Fixed1616_from_int(start)),Fixed1616_div(Fixed1616_from_int(i),Fixed1616_from_int(start))));
      dyn_array_init(Sprite,&s.sprites,2);
      dyn_array_add(Segment,&segments,128,s);
   }

   for(int i = 0;i<length;i++)
   {
      Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      Fixed1616 ly = segments.used?dyn_array_element(Segment,&segments,segments.used-1).p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+Fixed1616_mul(end_y-start_y,((-cos(((float)(start+i)/(float)total)*M_PI)*Fixed1616_from_int(1))/2)+Fixed1616_from_int(1)/2);
      COLOR_ROAD()
      s.curve = curve;
      dyn_array_init(Sprite,&s.sprites,2);
      dyn_array_add(Segment,&segments,128,s);
   }

   for(int i = 0;i<end;i++)
   {
      Segment s;
      memset(&s,0,sizeof(s));
      s.p0.z = segments.used*SEGLEN;
      Fixed1616 ly = segments.used?dyn_array_element(Segment,&segments,segments.used).p1.y:0;
      s.p0.y = ly;
      s.p1.z = (segments.used+1)*SEGLEN;
      s.p1.y = start_y+Fixed1616_mul(end_y-start_y,((-cos(((float)(start+length+i)/(float)total)*M_PI)*Fixed1616_from_int(1))/2)+Fixed1616_from_int(1)/2);
      COLOR_ROAD()
      s.curve = curve+Fixed1616_mul(-curve,((-cos(((float)i/(float)end)*M_PI)*Fixed1616_from_int(1))/2)+Fixed1616_from_int(1)/2);
      dyn_array_init(Sprite,&s.sprites,2);
      dyn_array_add(Segment,&segments,128,s);
   }
}

static void add_sprite(int seg, int index, Fixed1616 pos, uint8_t type)
{
   Sprite sp;
   sp.index = index;
   sp.pos = pos;
   sp.type = type;
   dyn_array_add(Sprite,&dyn_array_element(Segment,&segments,seg).sprites,2,sp);
}

static void add_car(int seg, int index, Fixed1616 pos, uint8_t type, Fixed2408 speed)
{
   Segment *segment = &dyn_array_element(Segment,&segments,seg);
   Car_list *l = car_list_new();
   Car *c = car_new();
   l->car = c;
   l->car->pos_x = pos;
   l->car->index = index;
   l->car->z = Fixed2408_from_int(1)+1;
   l->car->counter = 0;
   l->car->id = cars_id_counter();
   l->car->speed = speed;
   l->car->type = type;
   l->car->segment = seg;
   l->car->lap = 0;
   if(type==1)
   {
      Car_list *lo = car_list_new();
      lo->car = l->car;
      lo->next = cars_opponents;
      cars_opponents = lo;
   }

   l->next = segment->cars;
   segment->cars = l;
}
//-------------------------------------
