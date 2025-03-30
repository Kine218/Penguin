#include "MainObject.h"
#include <iostream>
MainObject::MainObject()
{
    frame=0;
    x_pos=0;
    y_pos=200;
    x_val=0;
    y_val=0;
    width_frame=64;
    height_frame=64;
    status=WALK_RIGHT;  // cố định trong game này
    input_type.left=0;
    input_type.right=0;
    input_type.jump=0;
    input_type.up=0;
    input_type.down=0;
    map_x=0;
    map_y=0;
}
std::string MainObject::getScore()
{
    std::string x=" ";
    int alter=score;
    while (alter>0)
    {
        x=char(alter%10+'0')+x;
        alter/=10;
    }
    return x;
}

MainObject::~MainObject()
{

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret=BaseObject::LoadImg(path, screen);
    /*if(ret!=true)
        {width_frame=rect.w/6;;
        height_frame=rect.h;
        }
    */

    return ret;
}

void MainObject::set_clips()
{
    if (width_frame>0&&height_frame>0)
    {
        for(int i=0;i<8;i++)
        {
        frame_clip[i].x=i*width_frame;
        frame_clip[i].y=0;
        frame_clip[i].w=width_frame;
        frame_clip[i].h=height_frame;
        }

    }
}

void MainObject::Show(SDL_Renderer* des)
{  if (on_ground==true)
   {
       LoadImg("img/player_right.png",des);
   }
   else
   {
       LoadImg("img/jum_right.png",des);
   }
   if (input_type.left==1||input_type.right==1)
       frame++;
   else
   {
       frame=0;
   }
   frame%=8;
   score++;
   rect.x =x_pos-map_x;
   rect.y =y_pos-map_y;
   if (on_ground) rect.y+=2;

   SDL_Rect* current_clip =&frame_clip[frame];

   SDL_Rect renderQuad= {rect.x,rect.y,width_frame,height_frame};

   SDL_RenderCopy(des,p_object,current_clip,&renderQuad);

}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
    {
        switch (events.key.keysym.sym)
        {

         //xử lí nhảy
            case SDLK_SPACE:
            {
                    if (OverJump==false)
                    {
                    input_type.jump=1;

                    }
                    OverJump=true;
                  //  status=JUMP;
            }
            default: break;
        }

    }
    else if (events.type == SDL_MOUSEBUTTONDOWN && events.button.button == SDL_BUTTON_LEFT)
    {
                if (OverJump==false)
                input_type.jump=1;
                OverJump=true;
    }
    else if (events.type == SDL_MOUSEBUTTONUP && events.button.button == SDL_BUTTON_LEFT)
    {
         OverJump=false;
    }
    else if (events.type == SDL_KEYUP)
    {     switch (events.key.keysym.sym)
        {
            case SDLK_SPACE:
            {
               OverJump=false;
            }

            default: break;
        }
    }

}
void MainObject::Doplayer(Map& map_data)
{   Music music;
    input_type.right=1;
    input_type.left=0;
    x_val=0;
    if (input_type.right==1) x_val+=PLAYER_SPEED;
    if (x_val>MAX_PLAYER_SPEED) x_val=MAX_PLAYER_SPEED;
    if (input_type.left==1)  x_val-=PLAYER_SPEED;
    if (x_val<-MAX_PLAYER_SPEED) x_val=-MAX_PLAYER_SPEED;
    y_val+=GRAVITY;
    if(y_val>=MAX_FALL_SPEED) y_val=MAX_FALL_SPEED;
    CheckToMap(map_data);
    CenterEntityOnMap(map_data);

    //xử lí nhảy
    if (input_type.jump==1)
    {   if (on_ground==true)
        {y_val= -PLAYER_JUMP_VAL;
        input_type.jump=0;
        music.JumpSound();
        }
    }
}
void MainObject::CenterEntityOnMap(Map& map_data)
{
    map_data.start_x=x_pos-(SCREEN_WIDTH/4);
    if (map_data.start_x<0)
        map_data.start_x=0;
    //map_data.start_y=y_pos-(SCREEN_HEIGHT)/2;
    if (map_data.start_y<0)
        map_data.start_y=0;
}
bool MainObject::CheckDead(Map& map_data)
{   int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;

    int height_min=height_frame<TILE_SIZE ? height_frame:TILE_SIZE;

    x1=(x_pos+x_val)/TILE_SIZE;
    x2=(x_pos+x_val+width_frame-1)/TILE_SIZE;

    y1=(y_pos)/TILE_SIZE;
    y2=(y_pos+height_min-1)/TILE_SIZE;

    if (map_data.tile[y1][x2]==10||map_data.tile[y2][x2]==10||map_data.tile[y1][x2]==20||map_data.tile[y2][x2]==20)
        {
          return true;
        }
    return false;

}

void MainObject::CheckToMap(Map& map_data)
{
    int x1=0;
    int x2=0;

    int y1=0;
    int y2=0;

    //check horizontal
    int height_min=height_frame<TILE_SIZE ? height_frame:TILE_SIZE;

    x1=(x_pos+x_val)/TILE_SIZE;
    x2=(x_pos+x_val+width_frame-1)/TILE_SIZE;

    y1=(y_pos)/TILE_SIZE;
    y2=(y_pos+height_min-1)/TILE_SIZE;

    if (x1>=0 && x2< MAX_MAP_X && y1>=0 &&y2< MAX_MAP_Y)
     {

     if (x_val>0)
        if (map_data.tile[y1][x2]%10!=0||map_data.tile[y2][x2]%10!=0)
        {
            x_pos=x2*TILE_SIZE;
            x_pos-=width_frame+1;
            x_val=0;
        }
        //else x_val=0;

    if (x_val<0)
        if(map_data.tile[y1][x1]%10!=0||map_data.tile[y2][x1]%10!=0)
        {
            x_pos=(x1+1)*TILE_SIZE;
            x_val=0;
        }
     }

    //check vertical
    int width_min=width_frame<TILE_SIZE ? width_frame:TILE_SIZE;
    x1=(x_pos)/TILE_SIZE;
    x2=(x_pos+width_min)/TILE_SIZE;

    y1=(y_pos+y_val)/TILE_SIZE;
    y2=(y_pos+y_val+height_frame-1)/TILE_SIZE;
    //check chạm đất
    if (map_data.tile[y2][x1]%10!=0||map_data.tile[y2][x2]%10!=0) on_ground=true;
    else on_ground=false;

    if (x1>=0 && x2< MAX_MAP_X && y1>=0 && y2<MAX_MAP_Y)
    {
       if (y_val>0)
       {
           if (map_data.tile[y2][x1]%10!=0||map_data.tile[y2][x2]%10!=0)
           {
               y_pos=y2*TILE_SIZE;
               y_pos-=(height_frame+1);
               y_val=0;

               on_ground=true;
           }
           else on_ground=false;
       }
       if (y_val<0)
       {
           if (map_data.tile[y1][x1]%10!=0|| map_data.tile[y1][x2]%10!=0)
           {
               y_pos=(y1+1)*TILE_SIZE;
               y_val=0;
           }
       }
    }
    x_pos+=x_val;
    y_pos+=y_val;
    if (x_pos<0) x_pos=0;


}


