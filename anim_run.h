#ifndef anim_run_h
#define anim_run_h

#include "anim.h"

class AnimRun : public Anim {
private:
    float pos;
    float inc;
protected:
        virtual void runImpl();
        virtual void setUp();
};
#endif
