#include <stdio.h>
#include "pico/stdlib.h"
#include "buttons.h"

void buttons_init() {
    gpio_init(BUTTON1_PIN);
    gpio_set_dir(BUTTON1_PIN, GPIO_IN);
    gpio_pull_up(BUTTON1_PIN); 
    // gpio_init(BUTTON2_PIN);
    // gpio_set_dir(BUTTON2_PIN, GPIO_IN);
    // gpio_pull_up(BUTTON2_PIN); 

    gpio_init(LED1_PIN);
    gpio_set_dir(LED1_PIN, GPIO_OUT);
    // gpio_init(LED2_PIN);
    // gpio_set_dir(LED2_PIN, GPIO_OUT);
}

bool button1_pressed(void) {
    return !gpio_get(BUTTON1_PIN); // pressed == true
}