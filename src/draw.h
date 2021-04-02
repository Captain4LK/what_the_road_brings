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
   Rectangle road00;
   Rectangle road01;
   Rectangle road02;
   Rectangle road03;
   Rectangle car_player[2][9];
   Rectangle backdrop[5];
   Rectangle sprites[32];
   Rectangle car_sprites[4][3];
};
extern struct Texture_Rects texture_rects;

extern Texture2D texture;
extern RenderTexture2D texture_viewport;

void load_assets();
void draw(ULK_fixed x, ULK_fixed z, int steer);

#endif
