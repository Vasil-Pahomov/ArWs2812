#ifndef anim_start_h
#define anim_start_h

#include "anim.h"

class AnimStart : public Anim 
{
private:
    int phase;
protected:
        virtual void runImpl();
        virtual void setUp();
};

#endif