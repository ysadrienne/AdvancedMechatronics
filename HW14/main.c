#include <stdio.h>
#include "pico/stdlib.h"
#include "hx711.h"

int main()
{
    stdio_init_all();

    init_hx711();

    int i = 0;
    uint64_t last_t = 0;

    while (true) {
        char m[100];
        int v[1000];
        int num = 0;
        uint64_t t[1000];
        scanf("%d", &num);
        int avg = 875000;
        for (i = 0; i < num; i++){
            int val = read_hx711();
            avg = val * 0.1 + avg * 0.9;
            v[i] = avg;
            t[i] = to_ms_since_boot(get_absolute_time());
        }
        for (i = 0; i < num; i++){
            printf("%d %llu %d\n", i, t[i], v[i]);
        }
    }
}