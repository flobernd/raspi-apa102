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
 * @brief   Provides functions to control `APA102` LEDs and LED strings on a Raspberry Pi.
 */

#ifndef RASPI_APA102_H
#define RASPI_APA102_H

#include <RaspiAPA102ExportConfig.h>
#include <stddef.h>
#include <stdint.h>

/* ============================================================================================== */
/* Enums and types                                                                                */
/* ============================================================================================== */

/**
 * @brief   Defines the `RaspiAPA102Device` struct.
 *
 * All fields in this struct should be considered as "private". Any changes may lead to unexpected
 * behavior.
 */
typedef struct RaspiAPA102Device_
{
    /**
     * @brief   Signals, if configured to use hardware `SPI`.
     */
    uint8_t native_spi;
    /**
     * @brief   The native hardware `SPI` channel (0..1). 
     */
    uint8_t channel;
    /**
     * @brief   The GPIO `SCLK` pin for software `SPI`.
     */
    uint8_t pin_sclk;
    /**
     * @brief   The GPIO `MOSI` pin for software `SPI`.
     */
    uint8_t pin_mosi;
    /**
     * @brief   The chip select pin for software `SPI`.
     */
    uint8_t pin_cs;
} RaspiAPA102Device;

#pragma pack(push, 1)

/**
 * @brief   Defines the `RaspiAPA102ColorQuad` struct.
 *
 * All fields in this struct should be considered as "private". Any changes may lead to unexpected
 * behavior.
 */
typedef struct RaspiAPA102ColorQuad_
{
    /**
     * @brief   The LED brightness (0..31).
     */
    uint8_t brightness;
    /**
     * @brief   The blue color component.
     */
    uint8_t b;
    /**
     * @brief   The green color component.
     */
    uint8_t g;
    /**
     * @brief   The red color component.
     */
    uint8_t r;
} RaspiAPA102ColorQuad;

#pragma pack(pop)

/* ============================================================================================== */
/* Macros                                                                                         */
/* ============================================================================================== */

/* ---------------------------------------------------------------------------------------------- */
/* Initializer                                                                                    */
/* ---------------------------------------------------------------------------------------------- */

/**
 * @brief   Defines a `RaspiAPA102ColorQuad` struct with the given brightness and color values.
 */
#define RASPI_APA102_COLOR_QUAD_INITIALIZER(ar, ag, ab, abrightness) \
    { \
        /* brightness */ 0b11100000 | ((abrightness) & 0b00011111), \
        /* b          */ ab, \
        /* g          */ ag, \
        /* r          */ ar, \
    }

/* ---------------------------------------------------------------------------------------------- */
/* Helper                                                                                         */
/* ---------------------------------------------------------------------------------------------- */

#define RASPI_APA102_COLOR_QUAD_R(quad) ((quad).r)
#define RASPI_APA102_COLOR_QUAD_G(quad) ((quad).g)
#define RASPI_APA102_COLOR_QUAD_B(quad) ((quad).b)
#define RASPI_APA102_COLOR_QUAD_BRIGHTNESS(quad) ((quad).brightness & 0b00011111)

/* ---------------------------------------------------------------------------------------------- */

/* ============================================================================================== */
/* Exported functions                                                                             */
/* ============================================================================================== */

/* ---------------------------------------------------------------------------------------------- */
/* APA102 Device                                                                                  */
/* ---------------------------------------------------------------------------------------------- */

/**
 * @brief   Initializes a new `APA102` device and configures it to use native hardware `SPI` on 
 *          the selected channel/chip. 
 * 
 * @param   device  A pointer to the `RaspiAPA102Device` struct.
 * @param   channel The number of the `SPI` channel/chip to use (either `0` or `1`).
 * 
 * This function initializes the given `SPI` channel with a frequency of `500000`.
 * 
 * @return  A status code.
 */
RASPI_APA102_EXPORT int RaspiAPA102DeviceInitHardware(RaspiAPA102Device* device, uint8_t channel);

/**
 * @brief   Initializes a new `APA102` device and configures it to use software emulated `SPI` on 
 *          the given `GPIO` pins.
 * 
 * The pin numbers are following the Raspberry Pi GPIO naming sheme. 
 * 
 * @param   device      A pointer to the `RaspiAPA102Device` struct.
 * @param   pin_sclk    The number of the GPIO pin (broadcom numbering scheme) to use as `SCLK` 
 *                      output.
 * @param   pin_mosi    The number of the GPIO pin (broadcom numbering scheme) to use as `MOSI` 
 *                      output.
 * @param   pin_cs      The number of the GPIO pin (broadcom numbering scheme) to use as channel 
 *                      select output, or `-1` if not needed.
 * 
 * This function sets the given GPIO pins to `OUTPUT` mode.
 * 
 * @return  A status code.
 */
RASPI_APA102_EXPORT int RaspiAPA102DeviceInitSoftware(RaspiAPA102Device* device, int pin_sclk, 
    int pin_mosi, int pin_cs);

/**
 * @brief   Updates the LEDs of the given `APA102` device.
 * 
 * @param   device  A pointer to the `RaspiAPA102Device` struct.
 * @param   colors  A pointer to an array of `RaspiAPA102ColorQuad` structs.
 * @param   count   The number of structs in the passed array.
 * 
 * @return  A status code.
 */
RASPI_APA102_EXPORT int RaspiAPA102DeviceUpdate(const RaspiAPA102Device* device, 
    const RaspiAPA102ColorQuad* colors, size_t count);

/* ---------------------------------------------------------------------------------------------- */
/* APA102 Color Quad                                                                              */
/* ---------------------------------------------------------------------------------------------- */

/**
 * @brief   Initializes a new `RaspiAPA102ColorQuad` struct with the given brightness and color 
 *          values.
 * 
 * @param   value       A pointer to the `RaspiAPA102ColorQuad` struct.
 * @param   r           The red color component.
 * @param   g           The green color component.
 * @param   b           The blue color component.
 * @param   brightness  The LED brightness (0..31).
 * 
 * @return  A status code.
 */
RASPI_APA102_EXPORT int RaspiAPA102ColorQuadInit(RaspiAPA102ColorQuad* value, uint8_t r, uint8_t g, 
    uint8_t b, uint8_t brightness);

/* ---------------------------------------------------------------------------------------------- */

/* ============================================================================================== */

#endif /* RASPI_APA102_H */
