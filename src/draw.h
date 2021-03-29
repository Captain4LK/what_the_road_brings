/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _DRAW_H_

#define _DRAW_H_

struct Texture_Rects
{
   SDL_Rect road00;
   SDL_Rect road01;
   SDL_Rect road02;
   SDL_Rect road03;
   SDL_Rect car_player[2][9];
   SDL_Rect backdrop[5];
   SDL_Rect sprites[32];
};
extern struct Texture_Rects texture_rects;

void load_assets();
void draw(ULK_fixed x, ULK_fixed z, int steer);

#endif
