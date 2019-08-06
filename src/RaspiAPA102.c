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

#include <RaspiAPA102/RaspiAPA102.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

/* ============================================================================================== */
/* Internal constants                                                                             */
/* ============================================================================================== */

#define RASPI_APA102_CLCK_STRETCH 5

/* ============================================================================================== */
/* Internal functions                                                                             */
/* ============================================================================================== */

/* ---------------------------------------------------------------------------------------------- */
/* SPI                                                                                            */
/* ---------------------------------------------------------------------------------------------- */

/**
 * @brief   Writes the specified amount of bits from the given buffer to the `SPI` device.
 * 
 * @param   device          A pointer to the `RaspiAPA102Device` struct.
 * @param   buffer          A pointer to the data buffer.
 * @param   number_of_bits  The number of bits to write.
 */
static void RaspiAPA102SPIWriteBuffer(const RaspiAPA102Device* device, const uint8_t* buffer, 
    size_t number_of_bits)
{
    // TODO: native SPI

    const uint8_t pin_sclk = device->pin_sclk;
    const uint8_t pin_mosi = device->pin_mosi;
    const uint8_t pin_cs = device->pin_cs;

    if (pin_cs >= 0)
    {
        digitalWrite(pin_sclk, LOW);
    }

    size_t number_of_bytes = number_of_bits / 8;
    size_t remaining_bits = number_of_bits % 8;
    if (remaining_bits > 0)
    {
        ++number_of_bytes;
    }

    for (int i = 0; i < number_of_bytes; ++i)
    {
        const uint8_t byte = buffer[i];
        uint8_t bits = 8;
        if (i == number_of_bytes - 1)
        {
            bits = remaining_bits;
        }

        for (int j = 0; j < 8; ++j)
        {
            digitalWrite(pin_mosi, (byte & (1 << (7 - j))) > 0);
            digitalWrite(pin_sclk, HIGH);
            usleep(RASPI_APA102_CLCK_STRETCH);
            digitalWrite(pin_sclk, LOW);
            usleep(RASPI_APA102_CLCK_STRETCH);
        }
    }

    if (pin_cs >= 0)
    {
        digitalWrite(pin_sclk, HIGH);
    }
}

/* ---------------------------------------------------------------------------------------------- */

/* ============================================================================================== */
/* Exported functions                                                                             */
/* ============================================================================================== */

/* ---------------------------------------------------------------------------------------------- */
/* APA102 Device                                                                                  */
/* ---------------------------------------------------------------------------------------------- */

int RaspiAPA102DeviceInitHardware(RaspiAPA102Device* device, uint8_t channel)
{
    if (!device || (channel > 1))
    {
        return -1;
    }

    device->native_spi = 1;
    device->channel    = channel;

    wiringPiSPISetup(channel, 500000);

    return 0;
}

int RaspiAPA102DeviceInitSoftware(RaspiAPA102Device* device, int pin_sclk, int pin_mosi, 
    int pin_cs)
{
    if (!device || 
        (pin_sclk < 0) || (pin_sclk > 29) || 
        (pin_mosi < 0) || (pin_mosi > 29) || 
                          (pin_cs   > 29))
    {
        return -1;
    }

    device->native_spi = 0;
    device->pin_sclk   = pin_sclk;
    device->pin_mosi   = pin_mosi;
    device->pin_cs     = pin_cs;

    wiringPiSetupGpio();
    pinMode(pin_sclk, OUTPUT);
    pinMode(pin_mosi, OUTPUT);
    if (pin_cs >= 0)
    {
        pinMode(pin_cs, OUTPUT);
        digitalWrite(device->pin_sclk, HIGH);
    }

    return 0;
}

int RaspiAPA102DeviceUpdate(const RaspiAPA102Device* device, const RaspiAPA102ColorQuad* colors, 
    size_t count)
{
    if (!device || !colors || !count)
    {
        return -1;
    }

    // A start frame of 32 zero bits (<0x00> <0x00> <0x00> <0x00>)
    const uint32_t init = 0;
    RaspiAPA102SPIWriteBuffer(device, (uint8_t*)&init, 32);

    // A 32 bit LED frame for each LED in the string (<0xE0+brightness> <blue> <green> <red>) 
    RaspiAPA102SPIWriteBuffer(device, (uint8_t*)colors, count * 32);

    // An end frame consisting of at least (n/2) bits of 1, where n is the number of LEDs in the 
    // string
    size_t bits = count / 2;
    if (count % 2 > 0)
    {
        ++bits;
    }
    const size_t bytes = ((bits + 7) & ~7) / 8;

    void* const buffer = malloc(bytes);
    memset(buffer, 0xFF, bytes);
    RaspiAPA102SPIWriteBuffer(device, buffer, bits);
    free(buffer);

    return 0;
}

/* ---------------------------------------------------------------------------------------------- */
/* APA102 LED                                                                                     */
/* ---------------------------------------------------------------------------------------------- */

int RaspiAPA102ColorQuadInit(RaspiAPA102ColorQuad* value, uint8_t r, uint8_t g, uint8_t b, 
    uint8_t brightness)
{
    if (!value)
    {
        return -1;
    }

    value->r = r;
    value->g = g;
    value->b = b;
    value->brightness = 0b11100000 | (brightness & 0b00011111);

    return 0;
}

/* ---------------------------------------------------------------------------------------------- */

/***************************************************************************************************/
