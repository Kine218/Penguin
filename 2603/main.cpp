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
BaseObject g_menu;
TTF_Font* gFont = nullptr;

 SDL_Color mint =  {64, 152, 179 ,255};
 SDL_Color white = { 255, 255, 255, 255 };

bool Restart=false;

bool InitData()
{
    bool success=true;
    if (TTF_Init() == -1) {
        cerr << "SDL_ttf không thể khởi tạo! Lỗi: " << TTF_GetError() << endl;
        return false;
    }
    gFont = TTF_OpenFont("font/DungeonFont.ttf", 28);
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
void RenderText(const std::string& text, int x, int y, SDL_Color color)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(g_screen, textSurface);
    SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(g_screen, textTexture, nullptr, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}
bool LoadBacground()
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
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
   // reset_map();
}

void Game_Run()
{   random_map R;
    R.reset_map();
    R.build();
    ImpTimer fps_timer;

//    if (InitData()==false) return ;
    if (LoadBacground()==false) return ;
//map
    GAME_MAP.LoadMap("map01.dat");
    GAME_MAP.LoadTiles(g_screen);

//player
    MainObject p_player;
    p_player.LoadImg("jum_right.png",g_screen);
    p_player.set_clips();


    bool is_quit=false;
    bool game_start=false;

    while (!is_quit)
    {
        fps_timer.start();
        game_start=false;

        string score=p_player.getScore();


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
//text
        RenderText("score:", 10, 10,white);
        RenderText(score, 80, 10,mint);

//checkDead
        if (p_player.CheckDead(map_data)) is_quit=true;



        SDL_RenderPresent(g_screen);


        int real_imp_timer=fps_timer.get_ticks();
        if (real_imp_timer<FPS)
        SDL_Delay(1000/FPS-real_imp_timer);
    }
    g_background.LoadImg("img/background5.png",g_screen);
    RenderText("Press Space to restart", 300, 200, white);
    SDL_RenderPresent(g_screen);
    //SDL_Delay(3000);
    Restart=true;

}
void menu()
{
   g_menu.LoadImg("img/background5.png",g_screen);
   bool menu_isrunning=true;
   while (menu_isrunning)
   {
       g_menu.Render(g_screen);
       bool playgame=false;
       bool exit=false;
       bool setting=false;
       int mouseX,mouseY;
       SDL_GetMouseState(&mouseX,&mouseY);


       if (mouseX>=300&&mouseX<=500&&mouseY>=200&&mouseY<250)
       {playgame=true;
       RenderText("Play Game", 300, 200, mint);
       }
       else RenderText("Play Game", 300, 200, white);


       if (mouseX>=300&&mouseX<=500&&mouseY>=250&&mouseY<300)
       { setting=true;
       RenderText("Settings", 300, 250, mint);
       }
       else RenderText("Settings", 300, 250, white);


       if (mouseX>=300&&mouseX<=500&&mouseY>=300&&mouseY<=350)
       { exit=true;
        RenderText("Exit:(", 300, 300, mint);
       }
       else RenderText("Exit:(", 300, 300,white);




        while (SDL_PollEvent(&g_event)!=0)
        {      if (g_event.type == SDL_MOUSEBUTTONDOWN && g_event.button.button == SDL_BUTTON_LEFT)
              {  if (playgame)
                   { Game_Run();
                    menu_isrunning=false;
                   }
                else if (exit)
                   { close();
                    menu_isrunning=false;
                   }
                   else{}

              }

        }
       SDL_RenderPresent(g_screen);

   }


}
int main(int argc, char* argv[])
{  if (InitData()==false) return 1;

     menu();
     while (Restart)
        menu();
        close();
    return 0;
}
