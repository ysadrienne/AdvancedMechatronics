#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PICO_DEFAULT_SPI_RX_PIN 16
#define PICO_DEFAULT_SPI_SCK_PIN  18
#define PICO_DEFAULT_SPI_TX_PIN 19
#define PIN_CS_DAC   17
#define PIN_CS_RAM 13

#define RAM_WRITE 0x02
#define RAM_READ  0x03
#define RAM_WRSR  0x01

#define pi 3.141592653589793238

int sine_i = 0;
int tri_i = 0;
int counter = 0;


static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS_DAC, 0);
    asm volatile("nop \n nop \n nop");
}
static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS_DAC, 1);
    asm volatile("nop \n nop \n nop");
}

static inline void ram_select() { gpio_put(PIN_CS_RAM, 0); sleep_us(1); }
static inline void ram_deselect() { gpio_put(PIN_CS_RAM, 1); sleep_us(1); }
static inline void dac_select() { gpio_put(PIN_CS_DAC, 0); sleep_us(1); }
static inline void dac_deselect() { gpio_put(PIN_CS_DAC, 1); sleep_us(1); }

void spi_ram_init() {

    gpio_init(PIN_CS_RAM);
    gpio_set_dir(PIN_CS_RAM, GPIO_OUT);
    gpio_put(PIN_CS_RAM, 1);

    uint8_t setup[2] = {RAM_WRSR, 0x40};
    ram_select();
    spi_write_blocking(SPI_PORT, setup, 2);
    ram_deselect();
}

void setDac(int channel, uint16_t theV) {
    uint8_t data[2];
    data[0] = (channel == 0) ? 0x30 : 0xB0;
    data[0] = data[0] | ((theV >> 6) & 0x0F);
    data[1] = (theV << 2) & 0xFF;

    dac_select();
    spi_write_blocking(SPI_PORT, data, 2);
    dac_deselect();
}

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 2000*1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN,  GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    
    gpio_init(PIN_CS_DAC);
    gpio_set_dir(PIN_CS_DAC, GPIO_OUT);
    gpio_put(PIN_CS_DAC, 1);

    spi_ram_init();

    ram_select();
    uint8_t write_header[3] = {RAM_WRITE, 0x00, 0x00};
    spi_write_blocking(SPI_PORT, write_header, 3);
    
    for (int i = 0; i < 1000; i++) {
        float v = (sin(2 * pi * i / 1000.0f) + 1.0f) / 2.0f * 3.3f;
        uint16_t dac_val = (uint16_t)(v / 3.3f * 1023);
        
        uint8_t b[2];
        b[0] = (uint8_t)(dac_val >> 8);
        b[1] = (uint8_t)(dac_val & 0xFF);
        spi_write_blocking(SPI_PORT, b, 2);
    }
    ram_deselect();

    while (true) {
        uint16_t wave_data[1000];

        ram_select();
        uint8_t read_header[3] = {RAM_READ, 0x00, 0x00};
        spi_write_blocking(SPI_PORT, read_header, 3);

        for (int i = 0; i < 1000; i++) {
            uint8_t b[2];
            spi_read_blocking(SPI_PORT, 0, b, 2);
            wave_data[i] = (b[0] << 8) | b[1];
        }

        ram_deselect();

        for (int i = 0; i < 1000; i++) {
            setDac(0, wave_data[i]);
            sleep_ms(1);
        }
    }
}