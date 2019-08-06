# Raspberry Pi APA102 library

Provides functions to control `APA102` LEDs and LED strings on a Raspberry Pi. It supports devices connected to the native `SPI` interface and devices connected to user defined `GPIO` pins (using software emulated `SPI`)

## Features

### Device initialization

```c
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
int RaspiAPA102DeviceInitHardware(RaspiAPA102Device* device, uint8_t channel);

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
int RaspiAPA102DeviceInitSoftware(RaspiAPA102Device* device, int pin_sclk, int pin_mosi, 
    int pin_cs);
```

### LED control

```c
/**
 * @brief   Updates the LEDs of the given `APA102` device.
 * 
 * @param   device  A pointer to the `RaspiAPA102Device` struct.
 * @param   colors  A pointer to an array of `RaspiAPA102ColorQuad` structs.
 * @param   count   The number of structs in the passed array.
 * 
 * @return  A status code.
 */
int RaspiAPA102DeviceUpdate(const RaspiAPA102Device* device, const RaspiAPA102ColorQuad* colors, 
    size_t count);
```

## Build

Install the required `wiringPi` library.

```bash
sudo apt install wiringpi
```

You can use CMake to generate project files for your favorite C99 compiler.

```bash
git clone --recursive 'https://github.com/flobernd/raspi-apa102.git'
cd raspi-apa102
mkdir build && cd build
cmake ..
make
```

## License

RaspiAPA102 is licensed under the MIT license.
