#include "GameRun.h"

void Game_Run()
{
    srand(time(nullptr)+clock());
    random_map R;
    R.build();
    ImpTimer fps_timer;
    MainObject p_player;
    std::vector<EnemyObject> p_enemy;
//    if (InitData()==false) return ;
//map
    GAME_MAP.LoadMap("map01.dat");
    GAME_MAP.LoadTiles(g_screen);

//player
    p_player.LoadImg("jum_right.png",g_screen);
    p_player.set_clips();
//enemy
 //   p_enemy.LoadImg("enemy.png",g_screen);
 //   p_enemy.set_clips();

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
//enemy
          Spawning(p_enemy,g_screen);



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
