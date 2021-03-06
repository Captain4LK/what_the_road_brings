/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _PLAYER_H_

#define _PLAYER_H_

struct Player
{
   Fixed2408 pz;
   Fixed1616 px;
   Fixed2408 vz;
   int steer;
   int stopped;
   int lap;
   int collisions;
   Fixed1616 time;
};

extern struct Player player;

void player_reset();
void player_update(int controll, float dt);
int player_pos();

#endif
