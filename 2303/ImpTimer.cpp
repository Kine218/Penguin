#include "ImpTimer.h"
#include "CommonFunc.h"
ImpTimer::ImpTimer()
{
    paused_tick=0;
    start_tick=0;
    is_paused_=false;
    is_started_=false;
}

ImpTimer::~ImpTimer()
{


}
void ImpTimer::start()
{
   is_started_=true;
   is_paused_=false;
   start_tick=SDL_GetTicks();
}

void ImpTimer::stop()
{
   is_started_=false;
   is_paused_=false;
}

void ImpTimer::paused()
{  if (is_started_==true&&is_paused_==false)
   {is_paused_=true;
   paused_tick=SDL_GetTicks()-start_tick;
   }
}

void ImpTimer::unpaused()
{
   if (is_paused_==true)
   {

       is_paused_=false;
       start_tick=SDL_GetTicks()-paused_tick;
       paused_tick=0;
   }
}

int ImpTimer::get_ticks()
{
   if (is_started_)
   {
       if (is_paused_)
        return paused_tick;
       else
        return SDL_GetTicks()-start_tick;
   }
   return 0;
}

bool ImpTimer::is_paused()
{
   return is_paused_;
}

bool ImpTimer::is_started()
{
   return is_started_;
}
