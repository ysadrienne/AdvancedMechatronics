#include "as5600.h"

int main()
{
    stdio_init_all();
    init_as5600();

    while (true) {
        int angle = read_encoder();
        printf("angle: %d", angle);
        sleep_ms(1000);
    }
}