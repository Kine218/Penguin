#ifndef ENEMYOBJECT_H
#define ENEMYOBJECT_H
#include "CommonFunc.h"
#include "BaseObject.h"
#include <string>
#include <vector>

class EnemyObject : public BaseObject
{
    public:
        EnemyObject();
        ~EnemyObject();
    bool LoadImg(std::string path,SDL_Renderer* screen);
    void Show(SDL_Renderer* des);
    void set_clips();
    enum lifestate
    {
        Alive=1,
        Dead=0
    };
    void SpawnEnemy(std::vector<EnemyObject> Vector_Enemy,SDL_Renderer* des);
    private:
        float x_val;      //độ xê dịch trái phải;
        float y_val;      //độ xê dịch trên dưới;

        float x_pos;      //vị trí nhân vật
        float y_pos;

        int width_frame;
        int height_frame;

        SDL_Rect frame_clip[8];

        int frame;
        int status=Alive;

};

#endif // ENEMYOBJECT_H
