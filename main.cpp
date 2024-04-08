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


void create_led(uint led) {
    uint period, freq;
    period = 1000;
    freq = 125; //Division of 125MHz / 1MHz
    uint slice_num = pwm_gpio_to_slice_num(led);
    pwm_config config_led = pwm_get_default_config();
    pwm_config_set_clkdiv_int(&config_led, freq);
    pwm_config_set_wrap(&config_led, period - 1); //Set the PWM period
    pwm_set_enabled(slice_num, led);
    pwm_init(slice_num, &config_led, true);
    gpio_set_function(led, GPIO_FUNC_PWM);
}

// Timer for slow LED blink
bool timer_callback(repeating_timer_t *rt) {
    ledTimerFired = true;
    return true; // keep repeating
}


int main() {

    // Initialize LED pins
    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_init(LED2_PIN);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_init(LED3_PIN);
    gpio_set_dir(LED3_PIN, GPIO_OUT);

    // Initialize chosen serial port
    stdio_init_all();

    // Create the timer
    repeating_timer_t timer;
    //Set it to 1Hz
    if (!add_repeating_timer_us(1000, timer_callback, NULL, &timer)) {
        printf("Failed to add timer\n");
        return 1;
    }

    // Initialize OLED
#ifdef USE_SSD1306
    i2c_init(i2c1, 400 * 1000);
    gpio_set_function(14, GPIO_FUNC_I2C); // the display has external pull-ups
    gpio_set_function(15, GPIO_FUNC_I2C); // the display has external pull-ups
    i2c_init(i2c0, 100 * 1000); // EEPROM
    gpio_set_function(16, GPIO_FUNC_I2C);
    gpio_set_function(17, GPIO_FUNC_I2C);
    gpio_pull_up(16);
    gpio_pull_up(17);
    ssd1306 display(i2c1);
    display.fill(0);
    display.text("Hello", 0, 0);
    display.show();
#endif

    while (true) {
        // Blink LED (pin 21) at 1Hz
        if (ledTimerFired) {
            if (isCalibrated.state == 0) {
                adjust_bright(LED2_PIN, ledState);
                if (ledState) {
                    ledState = 0;
                } else {
                    ledState = 100;
                }
            }
            ledTimerFired = false;
        }


    }
}
}