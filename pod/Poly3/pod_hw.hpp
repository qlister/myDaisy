#pragma once
#ifndef POD_HW_H
#define POD_HW_H /**< & */

#include "per/i2c.h"
#include "per/spi.h"
#include "per/gpio.h"
#include "sys/system.h"




#define SSD1305_I2C_ADDRESS 0x3C // 011110+SA0+RW - 0x3C or 0x3D

#define SSD1305_SETLOWCOLUMN 0x00
#define SSD1305_SETHIGHCOLUMN 0x10
#define SSD1305_MEMORYMODE 0x20
#define SSD1305_SETCOLADDR 0x21
#define SSD1305_SETPAGEADDR 0x22
#define SSD1305_SETSTARTLINE 0x40

#define SSD1305_SETCONTRAST 0x81
#define SSD1305_SETBRIGHTNESS 0x82

#define SSD1305_SETLUT 0x91

#define SSD1305_SEGREMAP 0xA0
#define SSD1305_DISPLAYALLON_RESUME 0xA4
#define SSD1305_DISPLAYALLON 0xA5
#define SSD1305_NORMALDISPLAY 0xA6
#define SSD1305_INVERTDISPLAY 0xA7
#define SSD1305_SETMULTIPLEX 0xA8
#define SSD1305_DISPLAYDIM 0xAC
#define SSD1305_MASTERCONFIG 0xAD
#define SSD1305_DISPLAYOFF 0xAE
#define SSD1305_DISPLAYON 0xAF

#define SSD1305_SETPAGESTART 0xB0

#define SSD1305_COMSCANINC 0xC0
#define SSD1305_COMSCANDEC 0xC8
#define SSD1305_SETDISPLAYOFFSET 0xD3
#define SSD1305_SETDISPLAYCLOCKDIV 0xD5
#define SSD1305_SETAREACOLOR 0xD8
#define SSD1305_SETPRECHARGE 0xD9
#define SSD1305_SETCOMPINS 0xDA
#define SSD1305_SETVCOMLEVEL 0xDB


  static const uint8_t init_128x64[] = {
      // Init sequence for 128x32 OLED module
      SSD1305_DISPLAYOFF,          // 0xAE
      SSD1305_SETLOWCOLUMN | 0x4,  // low col = 0
      SSD1305_SETHIGHCOLUMN | 0x4, // hi col = 0
      SSD1305_SETSTARTLINE | 0x0,  // line #0
      0x2E,                        //??
      SSD1305_SETCONTRAST,
      0x32, // 0x81, 0x32
      SSD1305_SETBRIGHTNESS,
      0x80, // 0x82, 0x80
      SSD1305_SEGREMAP | 0x01,
      SSD1305_NORMALDISPLAY, // 0xA6
      SSD1305_SETMULTIPLEX,
      0x3F, // 0xA8, 0x3F (1/64)
      SSD1305_MASTERCONFIG,
      0x8E, /* external vcc supply */
      SSD1305_COMSCANDEC,
      SSD1305_SETDISPLAYOFFSET,
      0x40, // 0xD3, 0x40
      SSD1305_SETDISPLAYCLOCKDIV,
      0xF0, // 0xD5, 0xF0
      SSD1305_SETAREACOLOR,
      0x05,
      SSD1305_SETPRECHARGE,
      0xF1, // 0xd9, 0xF1
      SSD1305_SETCOMPINS,
      0x12, // 0xDA, 0x12
      SSD1305_SETLUT,
      0x3F,
      0x3F,
      0x3F,
      0x3F};



namespace daisy
{
    /**
     * A driver implementation for the SSD1305
     */
    template <size_t width, size_t height, typename Transport>
    class SSD1305Driver
    {
    public:
        struct Config
        {
            typename Transport::Config transport_config;
        };

        void Init(Config config)
        {
            transport_.Init(config.transport_config);

            // Init routine...

            // Display Off
            transport_.SendCommand(0xaE);
            // Dimension dependent commands...
            switch(height)
            {
                case 16:
                    // Display Clock Divide Ratio
                    transport_.SendCommand(0xD5);
                    transport_.SendCommand(0x60);
                    // Multiplex Ratio
                    transport_.SendCommand(0xA8);
                    transport_.SendCommand(0x0F);
                    // COM Pins
                    transport_.SendCommand(0xDA);
                    transport_.SendCommand(0x02);
                    break;
                case 32:
                    // Display Clock Divide Ratio
                    transport_.SendCommand(0xD5);
                    transport_.SendCommand(0x80);
                    // Multiplex Ratio
                    transport_.SendCommand(0xA8);
                    transport_.SendCommand(0x1F);
                    // COM Pins
                    transport_.SendCommand(0xDA);
                    if(width == 64)
                    {
                        transport_.SendCommand(0x12);
                    }
                    else
                    {
                        transport_.SendCommand(0x02);
                    }

                    break;
                case 48:
                    // Display Clock Divide Ratio
                    transport_.SendCommand(0xD5);
                    transport_.SendCommand(0x80);
                    // Multiplex Ratio
                    transport_.SendCommand(0xA8);
                    transport_.SendCommand(0x2F);
                    // COM Pins
                    transport_.SendCommand(0xDA);
                    transport_.SendCommand(0x12);
                    break;
                default: // 128
                    // Display Clock Divide Ratio
                    transport_.SendCommand(0xD5);
                    transport_.SendCommand(0x80);
                    // Multiplex Ratio
                    transport_.SendCommand(0xA8);
                    transport_.SendCommand(0x3F);
                    // COM Pins
                    transport_.SendCommand(0xDA);
                    transport_.SendCommand(0x12);
                    break;
            }

            // Display Offset
            transport_.SendCommand(0xD3);
            transport_.SendCommand(0x00);
            // Start Line Address
            transport_.SendCommand(0x40);
            // Normal Display
            transport_.SendCommand(0xA6);
            // All On Resume
            transport_.SendCommand(0xA4);
            // Charge Pump
            transport_.SendCommand(0x8D);
            transport_.SendCommand(0x14);
            // Set Segment Remap
            transport_.SendCommand(0xA1);
            // COM Output Scan Direction
            transport_.SendCommand(0xC8);
            // Contrast Control
            transport_.SendCommand(0x81);
            transport_.SendCommand(0x8F);
            // Pre Charge
            transport_.SendCommand(0xD9);
            transport_.SendCommand(0x25);
            // VCOM Detect
            transport_.SendCommand(0xDB);
            transport_.SendCommand(0x34);


            // Display On
            transport_.SendCommand(0xAF); //--turn on oled panel
        };

        size_t Width() const { return width; };
        size_t Height() const { return height; };

        void DrawPixel(uint_fast8_t x, uint_fast8_t y, bool on)
        {
            if(x >= width || y >= height)
                return;
            if(on)
                buffer_[x + (y / 8) * width] |= (1 << (y % 8));
            else
                buffer_[x + (y / 8) * width] &= ~(1 << (y % 8));
        }

        void Fill(bool on)
        {
            for(size_t i = 0; i < sizeof(buffer_); i++)
            {
                buffer_[i] = on ? 0xff : 0x00;
            }
        };

        /**
         * Update the display 
        */
        void Update()
        {
            uint8_t i;
            uint8_t high_column_addr;
            switch(height)
            {
                case 32: high_column_addr = 0x12; break;

                default: high_column_addr = 0x10; break;
            }
            for(i = 0; i < (height / 8); i++)
            {
                transport_.SendCommand(0xB0 + i);
                transport_.SendCommand(0x00);
                transport_.SendCommand(high_column_addr);
                transport_.SendData(&buffer_[width * i], width);
            }
        };

    private:
        Transport transport_;
        uint8_t   buffer_[width * height / 8];
    };

    /**
     * 4 Wire SPI Transport for SSD1305 OLED display devices
     */
    class SSD13054WireSpiTransport
    {
    public:
        struct Config
        {
            Config()
            {
                // Initialize using defaults
                Defaults();
            }
            SpiHandle::Config spi_config;
            struct
            {
                dsy_gpio_pin dc;    /**< & */
                dsy_gpio_pin reset; /**< & */
            } pin_config;
            void Defaults()
            {
                // SPI peripheral config
                spi_config.periph = SpiHandle::Config::Peripheral::SPI_1;
                spi_config.mode   = SpiHandle::Config::Mode::MASTER;
                spi_config.direction
                    = SpiHandle::Config::Direction::TWO_LINES_TX_ONLY;
                spi_config.datasize       = 8;
                spi_config.clock_polarity = SpiHandle::Config::ClockPolarity::LOW;
                spi_config.clock_phase    = SpiHandle::Config::ClockPhase::ONE_EDGE;
                spi_config.nss            = SpiHandle::Config::NSS::HARD_OUTPUT;
                spi_config.baud_prescaler = SpiHandle::Config::BaudPrescaler::PS_8;
                // SPI pin config
                spi_config.pin_config.sclk = {DSY_GPIOG, 11};
                spi_config.pin_config.miso = {DSY_GPIOX, 0};
                spi_config.pin_config.mosi = {DSY_GPIOB, 5};
                spi_config.pin_config.nss  = {DSY_GPIOG, 10};
                // SSD130x control pin config
                pin_config.dc    = {DSY_GPIOB, 4};
                pin_config.reset = {DSY_GPIOA, 5};          // <- this is the difference!
            }
        };
        void Init(const Config& config)
        {
            // Initialize both GPIO
            pin_dc_.mode = DSY_GPIO_MODE_OUTPUT_PP;
            pin_dc_.pin  = config.pin_config.dc;
            dsy_gpio_init(&pin_dc_);
            pin_reset_.mode = DSY_GPIO_MODE_OUTPUT_PP;
            pin_reset_.pin  = config.pin_config.reset;
            dsy_gpio_init(&pin_reset_);

            // Initialize SPI
            spi_.Init(config.spi_config);

            // Reset and Configure OLED.
            dsy_gpio_write(&pin_reset_, 0);
            System::Delay(10);
            dsy_gpio_write(&pin_reset_, 1);
            System::Delay(10);              // unit of ms


        };
        void SendCommand(uint8_t cmd)
        {
            dsy_gpio_write(&pin_dc_, 0);
            spi_.BlockingTransmit(&cmd, 1);
        };

        void SendData(uint8_t* buff, size_t size)
        {
            dsy_gpio_write(&pin_dc_, 1);
            spi_.BlockingTransmit(buff, size);
        };

    private:
        SpiHandle spi_;
        dsy_gpio  pin_reset_;
        dsy_gpio  pin_dc_;
    };

        /**
     * A driver for the SSD1305 128x64 OLED displays connected via 4 wire SPI  
     */
    using SSD13054WireSpi128x64Driver
        = daisy::SSD1305Driver<128, 64, SSD13054WireSpiTransport>;

}
#endif

