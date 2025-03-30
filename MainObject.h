#ifndef MAINOBJECT_H
#define MAINOBJECT_H
#include "CommonFunc.h"
#include "BaseObject.h"
#define GRAVITY 2
#define MAX_FALL_SPEED 20
#define PLAYER_SPEED  16
#define MAX_PLAYER_SPEED 16
#define PLAYER_JUMP_VAL 20
#include <string>
#include "music.h"
class MainObject : public BaseObject
{
    public:
        MainObject();
        ~MainObject();
    enum WalkType
    {
        WALK_RIGHT=0,
        WALK_LEFT=1      //Mặc định là WALK_RIGHT
    }  ;
    std::string getScore();
    bool LoadImg(std::string path,SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void HandelInputAction(SDL_Event events,SDL_Renderer* screen);
    void set_clips();

    void Doplayer (Map& map_data);
    void CheckToMap(Map& map_data);
    bool CheckDead (Map& map_data);


    void SetMapXY (const int map_x_,const int map_y_){map_x=map_x_;map_y=map_y_;}
    void CenterEntityOnMap(Map& map_data);
    private:
        float x_val;      //độ xê dịch trái phải;
        float y_val;      //độ xê dịch trên dưới;
        int score=0;
        float x_pos;      //vị trí nhân vật
        float y_pos;

        int width_frame;
        int height_frame;

        SDL_Rect frame_clip[8];
        Input input_type;
        int frame;         //đang ở frame nào
        int status=WALK_RIGHT;        //trạng thái

        bool on_ground=false;
        bool OverJump=false;
        int map_x;
        int map_y;

};

#endif // MAINOBJECT_H
