#include "random_map.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "game_map.h"

using namespace std;
int a[105][10500];
void cloud_spawn()
{
    int z=1;
    while (z<=MAX_MAP_X)
   {
      int t=rand()%5+5;
      int m=rand()%3+1;
      z+=t;
      a[m][z]=rand()%2+4;
   }

}
void land_spawn()
{
    int mm=rand()%3+7;
    int z=1;
   while (z<=MAX_MAP_X)
   {    int m=rand()%3+7;
        if (mm-m==2) m=mm-1;
       int t=rand()%20+1;          //độ dài dãy land
       if (z==1) t+=10;
              //độ cao của land
      for (int x=z;x<z+t;x++)
        for (int y=m;y<=10;y++)
        {
            if (y==m)
            a[y][x]=2+x%2;
            else a[y][x]=1;
            if (x==z+t) a[y][x]=0;
            if (y==10&&y!=m) a[y][x]=6;
        }
        z+=t+rand()%2+2;
        mm=m;
   }
   for (int x=1;x<=MAX_MAP_X;x++)
   {
       if (a[10][x]==0) a[10][x]=10;
   }
}
void clearFile(const string& filename)
{
    ofstream file(filename, ios::trunc); // Mở file với chế độ xóa nội dung
    if (file.is_open()) {
        cout << "Đã xóa nội dung của tệp: " << filename << endl;
        file.close();
    } else
    {
        cout << "Không thể mở tệp: " << filename << endl;
    }
}
void random_map::reset_map()
{     clearFile("map01.txt");
      freopen("map01.dat","w",stdout);
       for (int y=1;y<=10;y++)
       for (int x=1;x<=MAX_MAP_X;x++)
        a[y][x]=0;
}
void random_map::build()
{
    freopen("map01.dat","w",stdout);
    srand(time(nullptr));
    cloud_spawn();
    land_spawn();

   for (int y=1;y<=10;y++)
   {
       for (int x=1;x<=MAX_MAP_X;x++)
        cout<<a[y][x]<<' ';
       cout<<endl;
   }
   fclose(stdout);
}

