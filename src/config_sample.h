#ifndef SSID

#define SSID "SSID"
#define PASSWORD "PASSWORD"
#define SHELLY_IP "x.x.x.x"
#define LED 8 // Pin for onboard LED (lights up during boot and error)
#define LED_PIN 7 // Pin for LEDs
#define LED_COUNT 32 // amount LEDs
#define MAX_POWER LED_COUNT * 50 // max amount in consumption mode
#define MAX_PRODUCING LED_COUNT * 50 // max amount in producing mode
#define ANIMATE_DELAY 20 // wait ms to turn on next led
#define POLLING_DELAY 1000 // time to wait after each poll
#define LED_MAX_BRIGHTNESS_PRODUCING 255 // max value allowed for LEDs when producing
#define LED_MAX_BRIGHTNESS_CONSUMING 30 // max value allowed for LEDs when not producing
#define ANIMATE_STEP 2 // increase by x per animation cycle
#define LED_CHANGE_DELAY 250 // delay increase and decrease of LEDs
#define NEOPIXEL_METHOD NeoEsp32Rmt0Ws2812xMethod //NeoWs2812xMethod
#define GREE_AC_REMOTE gree_ac_remote_model_t::YBOFB
#define GREE_AC_PIN 3
#define GREE_AC_TEMP 16 // 16-30C
#define GREE_AC_FAN 0 // 1-3, 0 auto
#define GREE_AC_MODE kGreeHeat // kGreeAuto, kGreeDry, kGreeCool, kGreeFan, kGreeHeat
#define GREE_AC_LED true
#define GREE_AC_SEND_REPEAT 1
#define GREE_AC_TURN_ON_THRESHOLD 1800
#define GREE_AC_GRACE_PERIOD 5*60*1000 // 5 minutes
#define ERROR_RESTART 30
#define WDT_TIMEOUT 3

#endif