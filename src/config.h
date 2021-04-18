/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _CONFIG_H_

#define _CONFIG_H_

//0 for vsync
#define FPS 60
#define XRES 320
#define YRES 240
#define TITLE "What the road brings"
#define MAX_PATH_LENGTH 512

//Input
#define DEADZONE 0.2f

#define SEGLEN Fixed2408_from_int(32)
#define MAX_SPEED Fixed2408_from_int(24*60)
#define MAX_SPEED_BOOST Fixed2408_from_int(30*60)
#define CAR_MAX_SPEED Fixed2408_from_int(24*60)
#define ACCEL Fixed2408_div(MAX_SPEED,Fixed2408_from_int(20))
#define DECEL Fixed2408_div(-MAX_SPEED,Fixed2408_from_int(10))
#define RENDER_DISTANCE 96
#define CAM_HEIGHT Fixed1616_from_int(64)
#define PLAYER_OFFSET (Fixed1616_mul(CAM_HEIGHT,CAM_DEPTH)>>8)
#define CAM_DEPTH 54991
#define CAM_DEPTHF 0.83909963f
#define GOAL_POS (PLAYER_OFFSET+8*SEGLEN)

#define ROAD_WIDTH 96
#define SPRITE_SCALE (1.0/90.0f)

#endif
