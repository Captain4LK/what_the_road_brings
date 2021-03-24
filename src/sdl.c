/*
A racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <SDL2/SDL.h>
//-------------------------------------

//Internal includes
#include "config.h"
#define CUTE_PNG_IMPLEMENTATION
#include "../external/cute_png.h"
#include "sdl.h"
//-------------------------------------

//#defines
#define MAX_CONTROLLERS 4 
//-------------------------------------

//Typedefs
typedef struct
{
   SDL_GameController *gamepad;
   SDL_Haptic *haptic;
   SDL_JoystickID id;
   int connected;
   uint8_t new_button_state[PAD_MAX];
   uint8_t old_button_state[PAD_MAX];
}Gamepad;
//-------------------------------------

//Variables
SDL_Window *sdl_window;
SDL_Renderer *renderer;
int window_width;
int window_height;
SDL_Texture *target;

static uint8_t key_map[SDL_NUM_SCANCODES];
static uint8_t mouse_map[6];
static uint8_t gamepad_map[SDL_CONTROLLER_BUTTON_MAX];
static uint8_t new_key_state[KEY_MAX];
static uint8_t old_key_state[KEY_MAX];
static uint8_t new_mouse_state[BUTTON_MAX];
static uint8_t old_mouse_state[BUTTON_MAX];
static Gamepad gamepads[MAX_CONTROLLERS];
static float delta;
static int frametime;
static int framedelay = 1000/FPS;
static int framestart;
static int mouse_wheel;
static int mouse_x_rel;
static int mouse_y_rel;
static int mouse_x;
static int mouse_y;
static int running = 1;
//-------------------------------------

//Function prototypes
static void update_viewport();
static int get_gamepad_index(int which); 
//-------------------------------------

//Function implementations

void sdl_init()
{
   if(SDL_Init(SDL_INIT_EVERYTHING)<0)
   {
      printf("FATAL ERROR: failed to init sdl\n");
      exit(-1);
   }

   sdl_window = SDL_CreateWindow("Racer",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,XRES,YRES,SDL_WINDOW_RESIZABLE);

   if(!sdl_window)
   {
      printf("FATAL ERROR: failed to create window\n");
      exit(-1);
   }

   renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED|(FPS?0:SDL_RENDERER_PRESENTVSYNC));
   if(!renderer)
   {
      printf("FATAL ERROR: failed to create renderer\n");
      exit(-1);
   }
   SDL_SetRenderDrawColor(renderer,0,0,0,0);
   SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
   SDL_RenderSetLogicalSize(renderer,320,240);
   SDL_RenderSetIntegerScale(renderer,1);
   target = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,XRES,YRES);
   SDL_RendererInfo info;
   SDL_GetRendererInfo(renderer,&info);
   printf("%d\n",info.flags&SDL_RENDERER_TARGETTEXTURE);
   SDL_SetHint("SDL_HINT_RENDER_SCALE_QUALITY","0");

   //update_viewport();

    key_map[0x00] = KEY_NONE;
   key_map[SDL_SCANCODE_A] = KEY_A;
   key_map[SDL_SCANCODE_B] = KEY_B;
   key_map[SDL_SCANCODE_C] = KEY_C;
   key_map[SDL_SCANCODE_D] = KEY_D;
   key_map[SDL_SCANCODE_E] = KEY_E;
   key_map[SDL_SCANCODE_F] = KEY_F;
   key_map[SDL_SCANCODE_G] = KEY_G;
   key_map[SDL_SCANCODE_H] = KEY_H;
   key_map[SDL_SCANCODE_I] = KEY_I;
   key_map[SDL_SCANCODE_J] = KEY_J;
   key_map[SDL_SCANCODE_K] = KEY_K;
   key_map[SDL_SCANCODE_L] = KEY_L;
   key_map[SDL_SCANCODE_M] = KEY_M;
   key_map[SDL_SCANCODE_N] = KEY_N;
   key_map[SDL_SCANCODE_O] = KEY_O;
   key_map[SDL_SCANCODE_P] = KEY_P;
   key_map[SDL_SCANCODE_Q] = KEY_Q;
   key_map[SDL_SCANCODE_R] = KEY_R;
   key_map[SDL_SCANCODE_S] = KEY_S;
   key_map[SDL_SCANCODE_T] = KEY_T;
   key_map[SDL_SCANCODE_U] = KEY_U;
   key_map[SDL_SCANCODE_V] = KEY_V;
   key_map[SDL_SCANCODE_W] = KEY_W;
   key_map[SDL_SCANCODE_X] = KEY_X;
   key_map[SDL_SCANCODE_Y] = KEY_Y;
   key_map[SDL_SCANCODE_Z] = KEY_Z;

   key_map[SDL_SCANCODE_F1] = KEY_F1;
   key_map[SDL_SCANCODE_F2] = KEY_F2;
   key_map[SDL_SCANCODE_F3] = KEY_F3;
   key_map[SDL_SCANCODE_F4] = KEY_F4;
   key_map[SDL_SCANCODE_F5] = KEY_F5;
   key_map[SDL_SCANCODE_F6] = KEY_F6;
   key_map[SDL_SCANCODE_F7] = KEY_F7;
   key_map[SDL_SCANCODE_F8] = KEY_F8;
   key_map[SDL_SCANCODE_F9] = KEY_F9;
   key_map[SDL_SCANCODE_F10] = KEY_F10;
   key_map[SDL_SCANCODE_F11] = KEY_F11;
   key_map[SDL_SCANCODE_F12] = KEY_F12;

   key_map[SDL_SCANCODE_DOWN] = KEY_DOWN;
   key_map[SDL_SCANCODE_LEFT] = KEY_LEFT;
   key_map[SDL_SCANCODE_RIGHT] = KEY_RIGHT;
   key_map[SDL_SCANCODE_UP] = KEY_UP;
   key_map[SDL_SCANCODE_RETURN] = KEY_ENTER;

   key_map[SDL_SCANCODE_BACKSPACE] = KEY_BACK;
   key_map[SDL_SCANCODE_ESCAPE] = KEY_ESCAPE;
   key_map[SDL_SCANCODE_TAB] = KEY_TAB;
   key_map[SDL_SCANCODE_LGUI] = KEY_HOME;
   key_map[SDL_SCANCODE_END] = KEY_END;
   key_map[SDL_SCANCODE_PAGEUP] = KEY_PGUP;
   key_map[SDL_SCANCODE_PAGEDOWN] = KEY_PGDN;
   key_map[SDL_SCANCODE_INSERT] = KEY_INS;
   key_map[SDL_SCANCODE_LSHIFT] = KEY_SHIFT;
   key_map[SDL_SCANCODE_RSHIFT] = KEY_SHIFT;
   key_map[SDL_SCANCODE_LCTRL] = KEY_CTRL;
   key_map[SDL_SCANCODE_RCTRL] = KEY_CTRL;
   key_map[SDL_SCANCODE_SPACE] = KEY_SPACE;

   key_map[SDL_SCANCODE_0] = KEY_0;
   key_map[SDL_SCANCODE_1] = KEY_1;
   key_map[SDL_SCANCODE_2] = KEY_2;
   key_map[SDL_SCANCODE_3] = KEY_3;
   key_map[SDL_SCANCODE_4] = KEY_4;
   key_map[SDL_SCANCODE_5] = KEY_5;
   key_map[SDL_SCANCODE_6] = KEY_6;
   key_map[SDL_SCANCODE_7] = KEY_7;
   key_map[SDL_SCANCODE_8] = KEY_8;
   key_map[SDL_SCANCODE_9] = KEY_9;

   key_map[SDL_SCANCODE_KP_0] = KEY_NP0;
   key_map[SDL_SCANCODE_KP_1] = KEY_NP1;
   key_map[SDL_SCANCODE_KP_2] = KEY_NP2;
   key_map[SDL_SCANCODE_KP_3] = KEY_NP3;
   key_map[SDL_SCANCODE_KP_4] = KEY_NP4;
   key_map[SDL_SCANCODE_KP_5] = KEY_NP5;
   key_map[SDL_SCANCODE_KP_6] = KEY_NP6;
   key_map[SDL_SCANCODE_KP_7] = KEY_NP7;
   key_map[SDL_SCANCODE_KP_8] = KEY_NP8;
   key_map[SDL_SCANCODE_KP_9] = KEY_NP9;
   key_map[SDL_SCANCODE_KP_MULTIPLY] = KEY_NP_MUL;
   key_map[SDL_SCANCODE_KP_PLUS] = KEY_NP_ADD;
   key_map[SDL_SCANCODE_KP_DIVIDE] = KEY_NP_DIV;
   key_map[SDL_SCANCODE_KP_MINUS] = KEY_NP_SUB;
   key_map[SDL_SCANCODE_KP_PERIOD] = KEY_NP_DECIMAL;

   mouse_map[SDL_BUTTON_LEFT] = BUTTON_LEFT;
   mouse_map[SDL_BUTTON_RIGHT] = BUTTON_RIGHT;
   mouse_map[SDL_BUTTON_MIDDLE] = BUTTON_MIDDLE;
   mouse_map[SDL_BUTTON_X1] = BUTTON_X1;
   mouse_map[SDL_BUTTON_X2] = BUTTON_X2;

   gamepad_map[SDL_CONTROLLER_BUTTON_A] = PAD_A;
   gamepad_map[SDL_CONTROLLER_BUTTON_B] = PAD_B;
   gamepad_map[SDL_CONTROLLER_BUTTON_X] = PAD_X;
   gamepad_map[SDL_CONTROLLER_BUTTON_Y] = PAD_Y;
   gamepad_map[SDL_CONTROLLER_BUTTON_BACK] = PAD_BACK;
   gamepad_map[SDL_CONTROLLER_BUTTON_GUIDE] = PAD_GUIDE;
   gamepad_map[SDL_CONTROLLER_BUTTON_START] = PAD_START;
   gamepad_map[SDL_CONTROLLER_BUTTON_LEFTSTICK] = PAD_LEFTSTICK;
   gamepad_map[SDL_CONTROLLER_BUTTON_RIGHTSTICK] = PAD_RIGHTSTICK;
   gamepad_map[SDL_CONTROLLER_BUTTON_LEFTSHOULDER] = PAD_LEFTSHOULDER;
   gamepad_map[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER] = PAD_RIGHTSHOULDER;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_UP] = PAD_UP;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_DOWN] = PAD_DOWN;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_LEFT] = PAD_LEFT;
   gamepad_map[SDL_CONTROLLER_BUTTON_DPAD_RIGHT] = PAD_RIGHT;

   //Clear key states, just in case,
   //should already be empty since known at compile time
   memset(new_key_state,0,sizeof(new_key_state));
   memset(old_key_state,0,sizeof(old_key_state));
   memset(new_mouse_state,0,sizeof(new_mouse_state));
   memset(old_mouse_state,0,sizeof(old_mouse_state));
   for(int i = 0;i<MAX_CONTROLLERS;i++)
   {
      memset(gamepads[i].new_button_state,0,sizeof(gamepads[i].new_button_state));
      memset(gamepads[i].old_button_state,0,sizeof(gamepads[i].old_button_state));
   }
}

static void update_viewport()
{
   SDL_GetWindowSize(sdl_window,&window_width,&window_height);

   SDL_Rect rect;
   if((float)window_width/(16.0f/9.0f)>=window_height)
   {
      rect.w = window_height*(16.0f/9.0f);
      rect.h = window_height;
   }
   else
   {
      rect.w = window_width;
      rect.h = window_width*(9.0f/16.0f);
   }
   rect.x = (window_width-rect.w)/2;
   rect.y = (window_height-rect.h)/2;
   window_width = rect.w;
   window_height = rect.h;
   SDL_RenderSetViewport(renderer,&rect);

   SDL_SetRenderDrawColor(renderer,0,0,0,255);
   SDL_RenderClear(renderer);
}

void sdl_update()
{
   mouse_wheel = 0;
   memcpy(old_key_state,new_key_state,sizeof(new_key_state));
   memcpy(old_mouse_state,new_mouse_state,sizeof(new_mouse_state));
   for(int i = 0;i<MAX_CONTROLLERS;i++)
      memcpy(gamepads[i].old_button_state,gamepads[i].new_button_state,sizeof(gamepads[0].new_button_state));

   mouse_x_rel = 0;
   mouse_y_rel = 0;

   //Event managing
   SDL_Event event;
   while(SDL_PollEvent(&event))
   {
      switch(event.type)
      {
      case SDL_QUIT:
         running = 0;
         break;
      case SDL_KEYDOWN:
         if(event.key.state==SDL_PRESSED)
            new_key_state[key_map[event.key.keysym.scancode]] = 1;
         break;
      case SDL_KEYUP:
         if(event.key.state==SDL_RELEASED)
            new_key_state[key_map[event.key.keysym.scancode]] = 0;
         break;
      case SDL_MOUSEBUTTONDOWN:
         if(event.button.state==SDL_PRESSED)
            new_mouse_state[mouse_map[event.button.button]] = 1;
         break;
      case SDL_MOUSEBUTTONUP:
         if(event.button.state==SDL_RELEASED)
            new_mouse_state[mouse_map[event.button.button]] = 0;
         break;       
      case SDL_MOUSEWHEEL:
         mouse_wheel = event.wheel.y;
         break;
      case SDL_MOUSEMOTION:
         mouse_x_rel+=event.motion.xrel;
         mouse_y_rel+=event.motion.yrel;
         break;
      case SDL_CONTROLLERBUTTONDOWN:
         if(event.cbutton.state==SDL_PRESSED)
         {
            int id = get_gamepad_index(event.cbutton.which);
            gamepads[id].new_button_state[gamepad_map[event.cbutton.button]] = 1;
         }
         break;
      case SDL_CONTROLLERBUTTONUP:
         if(event.cbutton.state==SDL_RELEASED)
         {
            int id = get_gamepad_index(event.cbutton.which);
            gamepads[id].new_button_state[gamepad_map[event.cbutton.button]] = 0;
         }
         break;
      case SDL_CONTROLLERDEVICEADDED:
         {
            int which = event.cdevice.which;
            if(which<MAX_CONTROLLERS)
            {
               gamepads[which].gamepad = SDL_GameControllerOpen(which);
               gamepads[which].connected = 1;
               SDL_Joystick *j = SDL_GameControllerGetJoystick(gamepads[which].gamepad);
               gamepads[which].id = SDL_JoystickInstanceID(j);
            }
         }
         break;
      case SDL_CONTROLLERDEVICEREMOVED:
         {
            int which = event.cdevice.which;
            if(which<0)
               break;
            int id = get_gamepad_index(which);
            gamepads[id].connected = 0;
            SDL_GameControllerClose(gamepads[id].gamepad);
         }
         break;
      /*case SDL_WINDOWEVENT:
         if(event.window.event==SDL_WINDOWEVENT_RESIZED)
            update_viewport();
         break;*/
      }
   }
   //-------------------------------------------
   
   SDL_GetMouseState(&mouse_x,&mouse_y);
   if(mouse_x<0)
     mouse_x = 0;
   if(mouse_y<1)
     mouse_y = 1;

   frametime = SDL_GetTicks()-framestart;

   if(framedelay>frametime)
      SDL_Delay(framedelay-frametime);

   delta = (float)(SDL_GetTicks()-framestart)/1000.0f;
   framestart = SDL_GetTicks();
}

int sdl_running()
{
   return running;
}

static int get_gamepad_index(int which)
{
   for(int i = 0;i<MAX_CONTROLLERS;i++)
      if(gamepads[i].connected&&gamepads[i].id==which)
         return i;

   return -1;
}

int sdl_key_down(int key)
{
   return new_key_state[key];
}

int sdl_key_pressed(int key)
{
   return new_key_state[key]&&!old_key_state[key];
}

int sdl_key_released(int key)
{
   return !new_key_state[key]&&old_key_state[key];
}

int sdl_mouse_down(int key)
{
   return new_mouse_state[key];
}

int sdl_mouse_pressed(int key)
{
   return new_mouse_state[key]&&!old_mouse_state[key];
}

int sdl_mouse_released(int key)
{
   return !new_mouse_state[key]&&old_mouse_state[key];
}

int sdl_mouse_wheel_get_scroll()
{
   return mouse_wheel;
}

int sdl_gamepad_down(int index, int key)
{
   return gamepads[index].new_button_state[key];
}

int sdl_gamepad_pressed(int index, int key)
{
   return gamepads[index].new_button_state[key]&&!gamepads[index].old_button_state[key];
}

int sdl_gamepad_released(int index, int key)
{
   return !gamepads[index].new_button_state[key]&&gamepads[index].old_button_state[key];
}

SDL_Texture *sdl_load_image(const char *path)
{
   cp_image_t img = cp_load_png(path);
   if(img.pix==NULL)
   {
      printf("Failed to load %s\n",path);
      return NULL;
   }

   SDL_Surface *s = SDL_CreateRGBSurfaceWithFormatFrom(img.pix,img.w,img.h,32,img.w*4,SDL_PIXELFORMAT_ARGB8888);
   SDL_Texture *out = SDL_CreateTextureFromSurface(renderer,s);
   SDL_FreeSurface(s);
   cp_free_png(&img);

   return out;
}
//-------------------------------------
