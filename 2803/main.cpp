#include <iostream>
#include "CommonFunc.h"
//#include "stdafx.h"
#include "game_map.h"
#include "random_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "music.h"
using namespace std;
BaseObject g_background;
GameMap GAME_MAP;
BaseObject g_menu;
TTF_Font* gFont = nullptr;
string SCORE;

 SDL_Color mint =  {20, 193, 225, 1};
 SDL_Color white = { 12, 60, 88, 1};
 //rgba(12, 60, 88, 1)

bool Restart=false;


bool InitData()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cerr << "SDL_mixer không thể khởi tạo! Lỗi: " << Mix_GetError() <<endl;
        return false;
    }

    bool success=true;
    if (TTF_Init() == -1)
    {
        cerr << "SDL_ttf không thể khởi tạo! Lỗi: " << TTF_GetError() <<endl;
        return false;
    }


    gFont = TTF_OpenFont("font/XR.ttf", 16);
    int ret=SDL_Init(SDL_INIT_VIDEO);
    if (ret<0)
        return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    g_window = SDL_CreateWindow("Pengoin",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);
    if (g_window==nullptr)
    {
        success=false;
    }
    else
    {
        g_screen=SDL_CreateRenderer(g_window,-1,SDL_RENDERER_ACCELERATED);
        if(g_screen==nullptr)
            success=false;
        else
        {
            SDL_SetRenderDrawColor(g_screen,255,255,255,255);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }

    Music::MusicInit();

    return success;

}
void RenderText(const std::string& text, int x, int y, SDL_Color color)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(g_screen, textTexture, nullptr, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
bool LoadBackground()
{
    bool ret=g_background.LoadImg("img/background5.png",g_screen);
    if (ret==false)
    return false;
    return true;

}
void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen=nullptr;
    SDL_DestroyWindow(g_window);
    g_window=nullptr;
    TTF_CloseFont(gFont);
    gFont = nullptr;

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_CloseAudio();
    Music::FreeMusic();
   // reset_map();
}

void Game_Run()
{
    srand(time(nullptr)+clock());
    random_map R;
    R.reset_map();
    R.build();
    ImpTimer fps_timer;
    MainObject p_player;
//    if (InitData()==false) return ;
    if (LoadBackground()==false) return ;
//map
    GAME_MAP.LoadMap("map01.dat");
    GAME_MAP.LoadTiles(g_screen);

//player

    p_player.LoadImg("jum_right.png",g_screen);
    p_player.set_clips();


    bool is_quit=false;
    bool game_start=false;

    while (!is_quit)
    {
        fps_timer.start();
        game_start=false;

        string score=p_player.getScore();
        SCORE=score;

        while (SDL_PollEvent(&g_event)!=0)
        {      if (g_event.type ==SDL_KEYDOWN)
                switch (g_event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    game_start=true;
                    break;
                default: break;
                }
             if (g_event.type==SDL_QUIT)
                is_quit=true;

             p_player.HandelInputAction(g_event,g_screen);

        }
//background
        SDL_SetRenderDrawColor(g_screen,255,255,255,255);
        SDL_RenderClear(g_screen);
        g_background.Render(g_screen,nullptr);
//map
        Map map_data=GAME_MAP.getMap();
//player
        p_player.SetMapXY(map_data.start_x,map_data.start_y);
        p_player.Doplayer(map_data);
        p_player.Show(g_screen);
//render
        GAME_MAP.SetMap(map_data);
        GAME_MAP.DrawMap(g_screen);
//text
        RenderText("score:", 10, 10,white);
        RenderText(score, 100, 10,mint);

//checkDead
        if (p_player.CheckDead(map_data)) is_quit=true;



        SDL_RenderPresent(g_screen);


        int real_imp_timer=fps_timer.get_ticks();
        if (real_imp_timer<FPS)
        SDL_Delay(1000/FPS-real_imp_timer);
    }
   // g_background.LoadImg("img/background5.png",g_screen);
   // RenderText("Press Space to restart", 300, 200, white);
    SDL_RenderPresent(g_screen);
    //SDL_Delay(3000);
    Restart=true;
}
void Setting()
{




}
void menu()
{
   g_menu.LoadImg("img/start_background.png",g_screen);
   bool menu_isrunning=true;
   while (menu_isrunning)
   {   SDL_RenderClear(g_screen);
       g_menu.Render(g_screen);
       bool playgame=false;
       bool exit=false;
       bool setting=false;
      // bool restart=false;
       int mouseX,mouseY;
       SDL_GetMouseState(&mouseX,&mouseY);
     //  if (score!=" ") restart=true;
       if (!Restart)
    {  if (mouseX>=250&&mouseX<=500&&mouseY>=200&&mouseY<250)
       {playgame=true;
       RenderText("Play Game", 255, 205, mint);
       }
       else RenderText("Play Game", 250, 200, white);


       if (mouseX>=250&&mouseX<=500&&mouseY>=250&&mouseY<300)
       { setting=true;
       RenderText("Settings", 255, 255, mint);
       }
       else RenderText("Settings", 250, 250, white);


       if (mouseX>=250&&mouseX<=500&&mouseY>=300&&mouseY<=350)
       { exit=true;
        RenderText("Exit:(", 255, 305, mint);
       }
       else RenderText("Exit:(", 250, 300,white);

    }
       else
    {   RenderText("Score:", 250, 200, mint);
        RenderText(SCORE, 340, 200, mint);

        if (mouseX>=250&&mouseX<=500&&mouseY>=250&&mouseY<300)
       {playgame=true;
       RenderText("Restart", 255, 255, mint);
       }
       else RenderText("Restart", 250, 250, white);


       if (mouseX>=250&&mouseX<=500&&mouseY>=300&&mouseY<350)
       { setting=true;
       RenderText("Settings", 255, 305, mint);
       }
       else RenderText("Settings", 250, 300, white);


       if (mouseX>=250&&mouseX<=500&&mouseY>=350&&mouseY<=400)
       { exit=true;
        RenderText("Exit:(", 255, 355, mint);
       }
       else RenderText("Exit:(", 250, 350,white);

        g_menu.LoadImg("img/restart.png",g_screen);
    }

        while (SDL_PollEvent(&g_event)!=0)
        {
                 if (g_event.type == SDL_MOUSEBUTTONDOWN && g_event.button.button == SDL_BUTTON_LEFT)
              {  if (playgame)
                   { Game_Run();
                    menu_isrunning=false;
                   }
                else if (exit)
                   { close();
                    menu_isrunning=false;
                   }
                else if (setting)
                   {
                     Setting();
                   }
              }
        }
       SDL_RenderPresent(g_screen);

   }


}
int main(int argc, char* argv[])
{  if (InitData()==false) return 1;
    Music g_music;
    g_music.PlayBackgroundMusic();
     menu();
     while (Restart)
        menu();
        close();

    return 0;
}
