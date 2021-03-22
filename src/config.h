/*
SoftLK-demos - a collection of bigger examples for SoftLK-lib

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _CONFIG_H_

#define _CONFIG_H_

#define XRES 320
#define YRES 240
#define FPS 30
#define SEGLEN ULK_fixed_from_int(32)
#define MAX_SPEED ULK_fixed_from_int(24)
#define ACCEL ULK_fixed_div(MAX_SPEED,ULK_fixed_from_int(10*FPS))
#define DECEL ULK_fixed_div(-MAX_SPEED,ULK_fixed_from_int(5*FPS))
#define RENDER_DISTANCE 64
#define ROAD_WIDTH 30000
#define CAM_HEIGHT ULK_fixed_32_from_int(64)
#define CAM_DEPTH 54991

#endif
