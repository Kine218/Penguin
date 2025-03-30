#include "music.h"
#include "CommonFunc.h"
Mix_Music* backgroundMusic = nullptr;
Mix_Chunk* jumpsound = nullptr;

void Music::MusicInit()
{
    backgroundMusic = Mix_LoadMUS("music/m2.mp3");
    jumpsound=Mix_LoadWAV("music/jump.wav");
    if (!jumpsound)
    {
    std::cout << "Lỗi tải hiệu ứng: " << Mix_GetError() << std::endl;
    }
    if (!backgroundMusic)
    {
    std::cout << "Lỗi tải âm thanh nền: " << Mix_GetError() << std::endl;
    }
}
void Music::JumpSound()
{
   Mix_VolumeChunk(jumpsound, jumpsound_volumn);
   Mix_PlayChannel(-1, jumpsound, 0);
}
void Music::FreeMusic()
{
    Mix_FreeMusic(backgroundMusic);
}
void Music::PlayBackgroundMusic()
{   Mix_VolumeMusic(backgroundmusic_volumn);
       if (backgroundMusic)
    {
        Mix_PlayMusic(backgroundMusic, -1); // Lặp vô hạn
    }
}

