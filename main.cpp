#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <math.h>
#include "pico/time.h"
#include "hardware/timer.h"
#include "ssd1306.h"

//Pin definitions
#define LED1_PIN 22
#define LED2_PIN 21
#define LED3_PIN 20

#define USE_SSD1306


int main() {

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);


    return 0;
}
