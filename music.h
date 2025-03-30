#ifndef MUSIC_H
#define MUSIC_H
#include "CommonFunc.h"

class Music
{
    public:
       void PlayBackgroundMusic ();
       void JumpSound();
       static void FreeMusic();
       static void MusicInit();

    private:
     int backgroundmusic_volumn=16;
     int jumpsound_volumn=8;
};

#endif // MUSIC_H
