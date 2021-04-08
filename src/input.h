/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _INPUT_H_

#define _INPUT_H_

int input_pressed_fullscreen();
int input_pressed_up();
int input_pressed_down();
int input_pressed_confirm();
int input_pressed_back();
int input_pressed_pause();
int input_down_accel();
int input_down_decel();
int input_down_steer_left();
int input_down_steer_right();
float input_steer_left();
float input_steer_right();

#endif
