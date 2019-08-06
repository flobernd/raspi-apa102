/***************************************************************************************************

  Raspberry Pi APA102 Library

  Original Author : Florian Bernd

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

***************************************************************************************************/

#include <math.h>
#include <RaspiAPA102/ColorConversion.h>

/* ============================================================================================== */
/* Internal functions                                                                             */
/* ============================================================================================== */

/* ---------------------------------------------------------------------------------------------- */
/*                                                                                                */
/* ---------------------------------------------------------------------------------------------- */



/* ---------------------------------------------------------------------------------------------- */

/* ============================================================================================== */
/* Exported functions                                                                             */
/* ============================================================================================== */

/* ---------------------------------------------------------------------------------------------- */
/* HSV                                                                                            */
/* ---------------------------------------------------------------------------------------------- */

RaspiAPA102HSV RaspiAPA102RGB2HSV(RaspiAPA102RGB in)
{
    RaspiAPA102HSV out;
    double min, max;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                              
    const double delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; 
        return out;
    }
    if (max > 0.0) 
    { 
        out.s = (delta / max);                  
    } 
    else 
    {
        out.s = 0.0;
        out.h = NAN;                       
        return out;
    }
    if (in.r >= max)                          
    {    
        out.h = ( in.g - in.b ) / delta; 
    }    
    else
    {
        if (in.g >= max)
        {
            out.h = 2.0 + ( in.b - in.r ) / delta; 
        } 
        else
        {
            out.h = 4.0 + ( in.r - in.g ) / delta;
        }
    }  

    out.h *= 60.0;                             

    if (out.h < 0.0)
    {
        out.h += 360.0;
    }

    return out;
}

RaspiAPA102RGB RaspiAPA102HSV2RGB(RaspiAPA102HSV in)
{
    RaspiAPA102RGB out;

    if (in.s <= 0.0) 
    {       
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }

    double hh = in.h;
    if (hh < 0.0)
    {
        hh = 0.0 - hh;   
    }
    if (hh >= 360.0) 
    {
        hh = hh - ((long)hh / 360);
    }
    hh /= 60.0;
    
    const long i = (long)hh;
    const double ff = hh - i;
    const double p = in.v * (1.0 - in.s);
    const double q = in.v * (1.0 - (in.s * ff));
    const double t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    
    return out;     
}

/* ---------------------------------------------------------------------------------------------- */

/***************************************************************************************************/