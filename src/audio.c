/*
what the road brings - a racing game

Written in 2021 by Lukas Holzbeierlein (Captain4LK) email: captain4lk [at] tutanota [dot] com

To the extent possible under law, the author(s) have dedicated all copyright and related and neighboring rights to this software to the public domain worldwide. This software is distributed without any warranty.

You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>. 
*/

//External includes
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <raylib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
//-------------------------------------

//Internal includes
#include "fixed.h"
#include "config.h"
#include "util.h"
#include "audio.h"
//-------------------------------------

//#defines
//-------------------------------------

//Typedefs
//-------------------------------------

//Variables
Music *music_current = NULL;
Music sound_drive;
Sound sound_hit;
Sound sound_countdown_0;
Sound sound_countdown_1;
Sound sound_boost;

static Music current_music;
static int current_music_track = -1;
static char tracks[8][MAX_PATH_LENGTH] = 
{
   "data/music/title.ogg",
};
//-------------------------------------

//Function prototypes
//-------------------------------------

//Function implementations

void audio_load()
{
   InitAudioDevice();
   sound_drive = LoadMusicStream("data/sounds/drive.ogg");
   SetMusicVolume(sound_drive,0.3f);

   sound_hit = LoadSound("data/sounds/hit.ogg");
   sound_countdown_0 = LoadSound("data/sounds/countdown_start.ogg");
   sound_countdown_1 = LoadSound("data/sounds/countdown_end.ogg");
   sound_boost = LoadSound("data/sounds/boost.ogg");

   SetMasterVolume(0.3f);
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
      SetMusicVolume(current_music,0.7f);
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

void audio_set_path(unsigned track, const char *path)
{
   strcpy(tracks[track],path);
}
//-------------------------------------
