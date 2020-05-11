# ESP32_E131_Pixel_Streamer
Allows E.131 RGB Matrix Data (Wireless DMX) to be streamed to ESP32 to control NeoPixels

This is an adaptation of the E131_Pixel_Pusher by debsahu, which I never managed to get to display properly.
https://github.com/debsahu/E131_PixelPusher
It has less features and should theoretically run less efficiently since DMA isn't used to move the pixel data.

To install:
platformio.ini is needed, and using PlatformIO should download the required libraries
