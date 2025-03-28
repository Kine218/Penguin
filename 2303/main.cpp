#include <iostream>
#include "CommonFunc.h"
//#include "stdafx.h"
#include "game_map.h"
#include "random_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
using namespace std;
BaseObject g_background;
GameMap GAME_MAP;

bool InitData()
{
    bool success=true;
    int ret=SDL_Init(SDL_INIT_VIDEO);
    if (ret<0)
        return false;
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");

    g_window = SDL_CreateWindow("GameWindow",
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
    return success;

}
bool LoadBacground()
{
    bool ret=g_background.LoadImg("img/background.png",g_screen);
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
    IMG_Quit();
    SDL_Quit();
}
void Map_run(bool run)
{
    GAME_MAP.LoadMap("map01.dat");
    if (run)
    GAME_MAP.map_run+=16;
}
void Game_Run()
{   random_map R;
    R.build();
    ImpTimer fps_timer;

    if (InitData()==false) return ;
    if (LoadBacground()==false) return ;
//map
    GAME_MAP.LoadMap("map01.dat");
    GAME_MAP.LoadTiles(g_screen);

//player
    MainObject p_player;
    p_player.LoadImg("player_right.png",g_screen);
    p_player.set_clips();


    bool is_quit=false;
    bool game_start=false;

    while (!is_quit)
    {
        fps_timer.start();
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
     //   Map_run(game_start);
        Map map_data=GAME_MAP.getMap();
//player
        p_player.SetMapXY(map_data.start_x,map_data.start_y);
        p_player.Doplayer(map_data);
        p_player.Show(g_screen);
//render
        GAME_MAP.SetMap(map_data);
        GAME_MAP.DrawMap(g_screen);
        SDL_RenderPresent(g_screen);

        int real_imp_timer=fps_timer.get_ticks();
        if (real_imp_timer<FPS)
        SDL_Delay(1000/FPS-real_imp_timer);
    }
    close();

}
int main(int argc, char* argv[])
{
   Game_Run();
    return 0;
}
