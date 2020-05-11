#include <Arduino.h>
#include <FastLED.h>
#include <ESPAsyncE131.h>

#define UNIVERSE 1       // First DMX Universe to listen for
#define UNIVERSE_COUNT 1 // Total number of Universes to listen for, starting at UNIVERSE

// Neopixel Setup
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS 100
#define DATA_PIN 14
#define BRIGHTNESS 80

const char ssid[] = "LAN Before Time-2G"; // Replace with your SSID
const char passphrase[] = "littlefoot";   // Replace with your WPA2 passphrase

const bool unicast_flag = true; // Set true for unicast, false for multicast

// ESPAsyncE131 instance with UNIVERSE_COUNT buffer slots
ESPAsyncE131 e131(UNIVERSE_COUNT);
// Array of NeoPixels
CRGB leds[NUM_LEDS];

// 2D Matrix Properties
const uint8_t height = 10;
const uint8_t width = 10;
const bool serpentineLayout = true;

uint64_t frameCt = 0;
uint64_t PM = 0;
float interval = 10 * 1000.0; // 10s

void setup()
{
  Serial.begin(115200);
  delay(10);

  // Make sure you're in station mode
  WiFi.mode(WIFI_STA);

  Serial.println("");
  Serial.print(F("Connecting to "));
  Serial.print(ssid);

  if (passphrase != NULL)
    WiFi.begin(ssid, passphrase);
  else
    WiFi.begin(ssid);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print(F("Connected with IP: "));
  Serial.println(WiFi.localIP());

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  // listening for E1.31 data

  if (unicast_flag && e131.begin(E131_UNICAST)) // Listen via Unicast
                                                //if (e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT))   // Listen via Multicast
    Serial.println(F("Listening for data via Unicast..."));
  else if (!unicast_flag && e131.begin(E131_MULTICAST, UNIVERSE, UNIVERSE_COUNT))
    Serial.println(F("Listening for data via Multicast..."));
  else
    Serial.println(F("*** e131.begin failed ***"));
}

void loop()
{
  if (!e131.isEmpty())
  {
    e131_packet_t packet;
    e131.pull(&packet); // Pull packet from ring buffer

    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[i].setRGB(packet.property_values[3 * i + 1], packet.property_values[3 * i + 2], packet.property_values[3 * i + 3]);
    }
    FastLED.show();
    frameCt++;

    //     Serial.printf("Universe %u / %u Channels | Packet#: %u / Errors: %u / CH1: %u, %u, %u\n",
    //             htons(packet.universe),                 // The Universe for this packet
    //             htons(packet.property_value_count) - 1, // Start code is ignored, we're interested in dimmer data
    //             e131.stats.num_packets,                 // Packet counter
    //             e131.stats.packet_errors,               // Packet error counter
    //             packet.property_values[1],             // Dimmer data for Channel 1
    //             packet.property_values[2],
    //             packet.property_values[3]);
  }
  if (millis() - PM >= interval)
    {
        PM = millis();
        Serial.printf("FPS: %.2f\n", interval / frameCt);
        frameCt = 0;
    }
}