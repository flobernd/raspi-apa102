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
#include <stdio.h>
#include <unistd.h>
#include <RaspiAPA102/APA102.h>
#include <RaspiAPA102/ColorConversion.h>

/* ============================================================================================== */
/* Constants                                                                                      */
/* ============================================================================================== */

#define RASPI_APA102_SPI_SCLK 14
#define RASPI_APA102_SPI_MOSI 15
#define RASPI_APA102_SPI_CS   -1

/* ============================================================================================== */
/* Internal Functions                                                                             */
/* ============================================================================================== */



/* ============================================================================================== */
/* Entry Point                                                                                    */
/* ============================================================================================== */

int main(void)
{
    RaspiAPA102Device device;
    RaspiAPA102DeviceInitSoftware(&device, RASPI_APA102_SPI_SCLK, RASPI_APA102_SPI_MOSI, 
        RASPI_APA102_SPI_CS);

    RaspiAPA102RGB lrgb;
    RaspiAPA102HSV lhsv;

    // Color 1 (purple)
    lrgb.r = 255 / 255;
    lrgb.g =   0 / 255;
    lrgb.b = 255 / 255;
    lhsv = RaspiAPA102RGB2HSV(lrgb);
    printf("RGB: %3.2f, %3.2f, %3.2f, HSV: %3.2f, %3.2f, %3.2f\n", lrgb.r, lrgb.g, lrgb.b, 
        lhsv.h, lhsv.s, lhsv.v);
    const float h_f = lhsv.h;

    // Color 2 (green)
    lrgb.r =   0 / 255;
    lrgb.g = 255 / 255;
    lrgb.b =   0 / 255;
    lhsv = RaspiAPA102RGB2HSV(lrgb);
    printf("RGB: %3.2f, %3.2f, %3.2f, HSV: %3.2f, %3.2f, %3.2f\n", lrgb.r, lrgb.g, lrgb.b, 
        lhsv.h, lhsv.s, lhsv.v);
    const float h_t = lhsv.h;

    usleep(1000 * 1000 * 10);

    // Delta hue
    const float h_d = h_t - h_f;

    // Fade animation
    for (int i = 0; i <= 100; ++i)
    {
        lhsv.h = h_f + h_d * (float)i / 100.0f;
        lrgb = RaspiAPA102HSV2RGB(lhsv);

        const uint8_t ar = (uint8_t)floor(255 * lrgb.r);
        const uint8_t ag = (uint8_t)floor(255 * lrgb.g);
        const uint8_t ab = (uint8_t)floor(255 * lrgb.b);

        printf("RGB: %3d, %3d, %3d, Factor: %2.2f\n", ar, ag, ab, (float)i / 100.0f);

        // Update LED
        const RaspiAPA102ColorQuad color = RASPI_APA102_COLOR_QUAD_INITIALIZER(ar, ag, ab, 16);
        RaspiAPA102DeviceUpdate(&device, &color, 1);

        usleep(1000 * 50);
    }

    return 0;  
}

/* ============================================================================================== */