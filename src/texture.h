/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _TEXTURE_H_

#define _TEXTURE_H_

struct Texture_Rects
{
   Rectangle car_player[2][5];
   Rectangle backdrop[5];
   Rectangle sprites[64];
   float sprites_col_scale[64];
   Rectangle car_sprites[6][3];
};
struct Parallax_data
{
   Rectangle layers[5][2];
   float speed[5];
};

extern struct Texture_Rects texture_rects;
extern struct Parallax_data parallax_data;
extern Texture2D texture;
extern Texture2D texture_menu;
extern RenderTexture2D texture_viewport;
extern Font font;

void textures_load();
void texture_set(const char *path);

#endif
