#ifndef SSID

#define SSID "SSID"
#define PASSWORD "PASSWORD"
#define SHELLY_IP "x.x.x.x"
#define LED 8 // Pin for onboard LED (lights up during boot and error)
#define LED_PIN 7 // Pin for LEDs
#define LED_COUNT 32 // amount LEDs
#define MAX_POWER LED_COUNT * 50 // max amount in consumption mode
#define MAX_PRODUCING 600 // max amount in producing mode
#define ANIMATE_DELAY 20 // wait ms to turn on next led
#define POLLING_DELAY 1000 // time to wait after each poll
#define LED_MAX_BRIGHTNESS_PRODUCING 255 // max value allowed for LEDs when producing
#define LED_MAX_BRIGHTNESS_CONSUMING 50 // max value allowed for LEDs when not producing
#define ANIMATE_STEP 4 // increase by x per animation cycle
#define LED_CHANGE_DELAY 250 // delay increase and decrease of LEDs
#define NEOPIXEL_METHOD NeoWs2812xMethod

#endif