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

/**
 * @file
 * @brief   Provides functions for conversion between RGB and HSV color space.
 */

#ifndef COLOR_CONVERSION_H
#define COLOR_CONVERSION_H

#include <RaspiAPA102ExportConfig.h>
#include <stdint.h>

/* ============================================================================================== */
/* Enums and types                                                                                */
/* ============================================================================================== */

/**
 * @brief   Defines the `RaspiAPA102RGB` struct.
 */
typedef struct RaspiAPA102RGB_
{
    /**
     * @brief   The red color component as a percentual fraction between `0.0` and `1.0`.
     */
    double r;
    /**
     * @brief   The green color component as a percentual fraction between `0.0` and `1.0`.
     */
    double g;
    /**
     * @brief   The blue color component as a percentual fraction between `0.0` and `1.0`.
     */
    double b;
} RaspiAPA102RGB;

/**
 * @brief   Defines the `RaspiAPA102HSV` struct.
 */
typedef struct RaspiAPA102HSV_
{
    /**
     * @brief   The hue in degrees (between `0.0` and `360.0`).
     */
    double h;
    /**
     * @brief   The saturation as a percentual fraction between `0.0` and `1.0`.
     */
    double s;
    /**
     * @brief   The value as a percentual fraction between `0.0` and `1.0`.
     */
    double v;
} RaspiAPA102HSV;

/* ============================================================================================== */
/* Macros                                                                                         */
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

/**
 * @brief   Converts the given `RGB` color value to a `HSV` color value.
 * 
 * @param   in  The `RGB` color value.
 * 
 * @return  A `HSV` color value.
 */
RASPI_APA102_EXPORT RaspiAPA102HSV RaspiAPA102RGB2HSV(RaspiAPA102RGB in);


/**
 * @brief   Converts the given `HSV` color value to a `RGB` color value.
 * 
 * @param   in  The `HSV` color value.
 * 
 * @return  A `RGB` color value.
 */
RASPI_APA102_EXPORT RaspiAPA102RGB RaspiAPA102HSV2RGB(RaspiAPA102HSV in);

/* ---------------------------------------------------------------------------------------------- */

/* ============================================================================================== */

#endif /* COLOR_CONVERSION_H */