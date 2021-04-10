/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

#ifndef _AUDIO_H_

#define _AUDIO_H_

extern Music *music_current;
extern Music sound_drive;
extern Sound sound_hit;
extern Sound sound_countdown_0;
extern Sound sound_countdown_1;
extern Sound sound_boost;

void audio_load();
void audio_set_track(unsigned track);
void audio_unload();

#endif
