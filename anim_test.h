#ifndef anim_test_h
#define anim_test_h

#include "anim.h"

class AnimTest : public Anim {
private:
    int c=0;
protected:
        virtual void runImpl();
        virtual void setUp();
};
#endif
