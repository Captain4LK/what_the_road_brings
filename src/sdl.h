/*
A racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _SDL_H_

#define _SDL_H_

typedef enum 
{
   KEY_NONE,
   KEY_A,KEY_B,KEY_C,KEY_D,KEY_E,KEY_F,KEY_G,
   KEY_H,KEY_I,KEY_J,KEY_K,KEY_L,KEY_M,KEY_N,
   KEY_O,KEY_P,KEY_Q,KEY_R,KEY_S,KEY_T,KEY_U,
   KEY_V,KEY_W,KEY_X,KEY_Y,KEY_Z,
   KEY_0,KEY_1,KEY_2,KEY_3,KEY_4,KEY_5,
   KEY_6,KEY_7,KEY_8,KEY_9,
   KEY_F1,KEY_F2,KEY_F3,KEY_F4,KEY_F5,KEY_F6,
   KEY_F7,KEY_F8,KEY_F9,KEY_F10,KEY_F11,KEY_F12,
   KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,
   KEY_SPACE,KEY_TAB,KEY_SHIFT,KEY_CTRL,KEY_INS,
   KEY_DEL,KEY_HOME,KEY_END,KEY_PGUP,KEY_PGDN,
   KEY_BACK,KEY_ESCAPE,KEY_RETURN,KEY_ENTER,KEY_PAUSE,KEY_SCROLL,
   KEY_NP0,KEY_NP1,KEY_NP2,KEY_NP3,KEY_NP4,KEY_NP5,KEY_NP6,KEY_NP7,KEY_NP8,KEY_NP9,
   KEY_NP_MUL,KEY_NP_DIV,KEY_NP_ADD,KEY_NP_SUB,KEY_NP_DECIMAL,
   KEY_MAX,
}Key;

typedef enum 
{
   PAD_A, PAD_B, PAD_X, PAD_Y, PAD_BACK, PAD_GUIDE,
   PAD_START, PAD_LEFTSTICK, PAD_RIGHTSTICK, PAD_LEFTSHOULDER,
   PAD_RIGHTSHOULDER, PAD_UP, PAD_DOWN, PAD_LEFT, PAD_RIGHT,
   PAD_MAX,
}Ramepad_button;

typedef enum 
{
   BUTTON_LEFT,BUTTON_RIGHT,BUTTON_MIDDLE,BUTTON_X1,BUTTON_X2,
   BUTTON_MAX,
}Mouse_button;

typedef struct
{
   uint8_t r;
   uint8_t g;
   uint8_t b;
   uint8_t a;
}Color;

extern SDL_Window *sdl_window;
extern SDL_Renderer *renderer;
extern SDL_Texture *target;

void sdl_init();
void sdl_update();
int sdl_running();
int sdl_key_down(int key);
int sdl_key_pressed(int key);
int sdl_key_released(int key);
int sdl_mouse_down(int key);
int sdl_mouse_pressed(int key);
int sdl_mouse_released(int key);
int sdl_mouse_wheel_get_scroll();
int sdl_gamepad_down(int index, int key);
int sdl_gamepad_pressed(int index, int key);
int sdl_gamepad_released(int index, int key);
SDL_Texture *sdl_load_image(const char *path);

#endif
