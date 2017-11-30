#ifndef anim_start_h
#define anim_start_h

#include "anim.h"

class AnimStart : public Anim 
{
private:
    int phase_c = 0;
    int phase=0;
protected:
        virtual void runImpl();
};

#endif