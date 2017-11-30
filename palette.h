#ifndef palette_h
#define palette_h

#include "color.h"

struct Palette
{
    int numColors;
    Color *colors;
    
    /**
    * Get the interpolated color from the palette.
    * The argument is a floating number between 0 and numColors.
    */
    Color getPalColor(float i)
    {
        int i0 = (int)i%(numColors);
        int i1 = (int)(i+1)%(numColors);
        
        // decimal part is used to interpolate between the two colors
        float t0 = i - trunc(i);
        //float t0 = i - (int)i;

        return colors[i0].interpolate(colors[i1], t0);
    }
       
};



#endif