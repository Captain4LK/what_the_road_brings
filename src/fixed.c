/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdint.h>
//-------------------------------------

//Internal includes
#include "fixed.h"
//-------------------------------------

//#defines
#define SIGN(in) \
   (1|(in>>(sizeof(in)*8-1)))

#define FROM_INT(type,power) \
   type type##_from_int(int in) { return (type)in<<power; }

#define TO_INT(type,power) \
   int type##_to_int(type in) { return (int)(in>>power); }

#define DIV(type,con,power) \
   type type##_div(type a, type b) { return ((con)a*((con)1<<power))/b; }

#define MUL(type,con,power) \
   type type##_mul(type a, type b) { return ((con)a*(con)b)/((con)1<<power); }

#define ROUND(type,power,and) \
   type type##_round(type in) { int sign = SIGN(in); in*=sign; in = ((in+((type)1<<(power-1))))&(and); in*=sign; return in; }

#define FLOOR(type,and) \
   type type##_floor(type in) { int sign = SIGN(in); in*=sign; in&=(and); in*=sign; return in; }

#define CEIL(type,power,and) \
   type type##_ceil(type in) { int sign = SIGN(in); in*=sign; in = (in+((type)1<<power)-1)&(and); in*=sign; return in; }

#define SQRT(type,con,shift) \
   type type##_sqrt(type in) { int sign = SIGN(in); in*=sign; con result = 0, a = (con)in<<(shift/2), b = (con)1L<<(shift*2-2); while(b>a) b>>=2; \
   while(b) { if(a>=result+b) { a-=result+b; result = result+2*b; } b>>=2; result>>=1; } return result*sign; }

#define DEG2ANGLE(type) \
   type type##_deg_to_angle(type in) { return type##_div(type##_mul(type##_from_int(8),in),type##_from_int(360)); }

#define ANGLE2DEG(type) \
   type type##_angle_to_deg(type in) { return type##_mul(type##_from_int(45),in); } 

#define RAD2ANGLE(type,pi) \
   type type##_rad_to_angle(type in) { return type##_div(type##_mul(type##_from_int(8),in),2*pi); }

#define ANGLE2RAD(type,pi) \
   type type##_angle_to_rad(type in) { return type##_div(type##_mul(pi,in),type##_from_int(4)); }
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations
FROM_INT(Fixed1616,16)
TO_INT(Fixed1616,16)
DIV(Fixed1616,int64_t,16)
MUL(Fixed1616,int64_t,16)
ROUND(Fixed1616,16,(INT32_MAX-UINT16_MAX))
FLOOR(Fixed1616,(INT32_MAX-UINT16_MAX))
CEIL(Fixed1616,16,(INT32_MAX-UINT16_MAX))
SQRT(Fixed1616,int64_t,32)
DEG2ANGLE(Fixed1616)
ANGLE2DEG(Fixed1616)
RAD2ANGLE(Fixed1616,205887);
ANGLE2RAD(Fixed1616,205887);

FROM_INT(Fixed2408,8)
TO_INT(Fixed2408,8)
DIV(Fixed2408,int64_t,8)
MUL(Fixed2408,int64_t,8)
ROUND(Fixed2408,8,(INT32_MAX-UINT8_MAX))
FLOOR(Fixed2408,(INT32_MAX-UINT8_MAX))
CEIL(Fixed2408,8,(INT32_MAX-UINT8_MAX))
DEG2ANGLE(Fixed2408)
ANGLE2DEG(Fixed2408)
RAD2ANGLE(Fixed2408,205887);
ANGLE2RAD(Fixed2408,205887);
SQRT(Fixed2408,int64_t,16)
//-------------------------------------
