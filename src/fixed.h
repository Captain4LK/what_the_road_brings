/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _FIXED_H_

#define _FIXED_H_

#define FIXED1616_PI 205887
#define FIXED1616_E 178145

//16:16 fixed point
typedef int32_t Fixed1616;

//Normal mathematics
Fixed1616 Fixed1616_from_int(int in);
int          Fixed1616_to_int(Fixed1616 in);
Fixed1616 Fixed1616_div(Fixed1616 a, Fixed1616 b);
Fixed1616 Fixed1616_mul(Fixed1616 a, Fixed1616 b);
Fixed1616 Fixed1616_round(Fixed1616 in);
Fixed1616 Fixed1616_floor(Fixed1616 in);
Fixed1616 Fixed1616_ceil(Fixed1616 in);
Fixed1616 Fixed1616_sqrt(Fixed1616 in);
Fixed1616 Fixed1616_deg_to_angle(Fixed1616 in);
Fixed1616 Fixed1616_rad_to_angle(Fixed1616 in);
Fixed1616 Fixed1616_angle_to_deg(Fixed1616 in);
Fixed1616 Fixed1616_angle_to_rad(Fixed1616 in);

//24:8 fixed point
typedef int32_t Fixed2408;

//Normal mathematics
Fixed2408 Fixed2408_from_int(int in);
int          Fixed2408_to_int(Fixed2408 in);
Fixed2408 Fixed2408_div(Fixed2408 a, Fixed2408 b);
Fixed2408 Fixed2408_mul(Fixed2408 a, Fixed2408 b);
Fixed2408 Fixed2408_round(Fixed2408 in);
Fixed2408 Fixed2408_floor(Fixed2408 in);
Fixed2408 Fixed2408_ceil(Fixed2408 in);
Fixed2408 Fixed2408_sqrt(Fixed2408 in);
Fixed2408 Fixed2408_deg_to_angle(Fixed2408 in);
Fixed2408 Fixed2408_rad_to_angle(Fixed2408 in);
Fixed2408 Fixed2408_angle_to_deg(Fixed2408 in);
Fixed2408 Fixed2408_angle_to_rad(Fixed2408 in);

#endif
