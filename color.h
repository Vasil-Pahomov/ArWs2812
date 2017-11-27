#ifndef color_h
#define color_h
#include <Arduino.h>

struct AlaColor
{
    union
    {
        struct
        {
            byte r;
            byte g;
            byte b;
        };
        byte raw[3];
    };
    
    void fade(byte k)
    {
      if (r>=k) { r=r-k; } else { r=0; }
      if (g>=k) { g=g-k; } else { g=0; }
      if (b>=k) { b=b-k; } else { b=0; }
    }

    void fade3(byte dr, byte dg, byte db)
    {
      if (r>=dr) { r=r-dr; } else { r=0; }
      if (g>=dg) { g=g-dg; } else { g=0; }
      if (b>=db) { b=b-db; } else { b=0; }
    }  
} ;

#endif