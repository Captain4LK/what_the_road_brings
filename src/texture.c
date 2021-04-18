/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdlib.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "texture.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
Texture2D texture;
Texture2D texture_menu;
RenderTexture2D texture_viewport;
Font font;
struct Texture_Rects texture_rects;
struct Parallax_data parallax_data;
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void textures_load()
{
   texture_menu = LoadTexture("data/assets/sheet_menu.png");
   texture_viewport = LoadRenderTexture(XRES,YRES);
   font = LoadFont("data/assets/font.png");
}

void texture_set(const char *path)
{
   UnloadTexture(texture);
   texture = LoadTexture(path);
}
//-------------------------------------
