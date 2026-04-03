#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define PWM 16

void setServo(int);

int main()
{
    stdio_init_all();

    gpio_set_function(PWM, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(PWM);
    float div = 50;
    pwm_set_clkdiv(slice_num, div);
    uint16_t wrap = 60000;
    pwm_set_wrap(slice_num, wrap);

    pwm_set_enabled(slice_num, true);
    pwm_set_gpio_level(PWM, 0);

    while (true) {
        int i = 0;
        for (i = 10; i < 170; i++) {
            setServo(i);
            sleep_ms(10);
        }
        for (i = 170; i > 10; i--) {
            setServo(i);
            sleep_ms(10);
        }
    }
}

void setServo(int angle){
    int level = (0.05 + (angle / 180.0) * 0.05) * 60000;
    pwm_set_gpio_level(PWM, level);
}