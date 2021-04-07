/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "ULK_fixed.h"
#include "config.h"
#include "util.h"
#include "input.h"
//-------------------------------------

//#defines
#define DEADZONE 0.2f

#define MIN(a,b) \
   ((a)<(b)?(a):(b))
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

int input_pressed_fullscreen()
{
   return IsKeyPressed(KEY_M);
}

int input_pressed_up()
{
   return IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_LEFT_FACE_UP);
}

int input_pressed_down()
{
   return IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_LEFT_FACE_DOWN);
}

int input_pressed_confirm()
{
   return IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
}

int input_pressed_pause()
{
   return IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0,GAMEPAD_BUTTON_MIDDLE_RIGHT);
}

int input_down_accel()
{
   return IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
}

int input_down_decel()
{
   return IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
}

int input_down_steer_left()
{
   return IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT) || (GetGamepadAxisMovement(0,GAMEPAD_AXIS_LEFT_X)<-DEADZONE);
}

int input_down_steer_right()
{
   return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || (GetGamepadAxisMovement(0,GAMEPAD_AXIS_LEFT_X)>DEADZONE);
}

float input_steer_left()
{
   float amount = 0.0f;
   amount = MIN(1.0f,-GetGamepadAxisMovement(0,GAMEPAD_AXIS_LEFT_X));

   if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_LEFT))
      amount = 1.0f;

   return amount;
}

float input_steer_right()
{
   float amount = 0.0f;
   amount = MIN(1.0f,GetGamepadAxisMovement(0,GAMEPAD_AXIS_LEFT_X));

   if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsGamepadButtonDown(0,GAMEPAD_BUTTON_LEFT_FACE_RIGHT))
      amount = 1.0f;

   return amount;
}
//-------------------------------------
