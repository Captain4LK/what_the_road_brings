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
#include "audio.h"
#include "mode.h"
#include "player.h"
#include "car.h"
#include "segment.h"
#include "draw.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
Music *music_current = NULL;
Music sound_drive;

static Music current_music;
static int current_music_track = -1;
static const char *tracks[] = 
{
   "music/title.ogg",
   "music/track1.mp3",
};
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void audio_load()
{
   sound_drive = LoadMusicStream("sounds/drive.ogg");
   SetMusicVolume(sound_drive,0.3f);

   SetMasterVolume(0.4f);
}

void audio_set_track(unsigned track)
{
   if(track==current_music_track&&music_current!=NULL)
   {
      if(!IsMusicPlaying(current_music))
         PlayMusicStream(current_music);
   }
   else
   {
      if(music_current!=NULL)
         UnloadMusicStream(*music_current);

      current_music_track = track;
      current_music = LoadMusicStream(tracks[track]);
      music_current = &current_music;
      PlayMusicStream(current_music);
   }
}

void audio_unload()
{
   UnloadMusicStream(sound_drive);
   if(music_current!=NULL)
      UnloadMusicStream(current_music);

   CloseAudioDevice();
}
//-------------------------------------
