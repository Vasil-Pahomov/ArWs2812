#ifndef palette_h
#define palette_h

#include "color.h"
#include "options.h"

struct Palette
{
    int numColors;
    Color *colors;
    
    /**
    * Get the interpolated color from the palette.
    * The argument is between 0 and 254; the value of 255 is used for white
    */
    Color getPalColor(byte i)
    {
      if (i == 255) { //special case of "spark" color
        return Color(SPARK_R, SPARK_G, SPARK_B);
      }
      int i0 = (int)(i*numColors/256)%(numColors);
      int i1 = (int)(i*numColors/256+1)%(numColors);
      
      // decimal part is used to interpolate between the two colors
      float t0 = i*numColors/256 - trunc(i*numColors/256);

      return colors[i0].interpolate(colors[i1], t0);
    }
       
};

////////////////////////////////////////////////////////////////////////////////
// Palette definitions
////////////////////////////////////////////////////////////////////////////////
extern Palette PalRgb;
extern Palette PalRainbow;
extern Palette PalRainbowStripe;
extern Palette PalParty;
extern Palette PalHeat;
extern Palette PalFire;
extern Palette PalIceBlue;

#endif
