#include "enemyobject.h"

EnemyObject::EnemyObject()
{
    frame=0;
    x_pos=1280;
    y_pos=rand()%320+100;
  //  x_pos=640;
  //  y_pos=320;
    x_val=0;
    y_val=0;
    width_frame=32;
    height_frame=32;
}
EnemyObject::~EnemyObject()
{
    //dtor
}
bool EnemyObject::LoadImg(std::string path, SDL_Renderer* screen)
{
   bool ret=BaseObject::LoadImg(path, screen);
   return ret;
}

void EnemyObject::Show(SDL_Renderer* des)
{
    if (status==Alive)
    {
        LoadImg("img/enemy.png",des);
    }
    else if (status==Dead)
    {
        LoadImg("img/enemydead.png",des);
    }
    frame++;
    frame%=8;

    x_val=-(rand()%2+1)*8;
    y_val=(rand()%3-1)*2;

    x_pos=x_pos+x_val;
    y_pos=y_pos+y_val;


    rect.x=x_pos;
    rect.y=y_pos;



   SDL_Rect* current_clip =&frame_clip[frame];

   SDL_Rect renderQuad= {rect.x,rect.y,width_frame,height_frame};

   SDL_RenderCopy(des,p_object,current_clip,&renderQuad);

}

void EnemyObject::set_clips()
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
void EnemyObject::SpawnEnemy(std::vector<EnemyObject> Vector_Enemy,SDL_Renderer* des)
{
}


