#include "stdio.h"
#include "stdint.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define OUTPUT  0x1
#define INPUT   0x0
#define INPUT_OUTPUT    0x2




/*
Manually utilizing Embedded Memory which in Internal SRAM's
SRAM1 = 0x3FFE_0000   - 0x3FFF_FFFF  128KB
SRAM2 = 0x3FFA_E000   - 0x3FFD_FFFF  200KB
*/

typedef int (*func_t)(int, int);

int returnPtr(int x, int y);

/* Interface to PIN_0 */
__attribute__((always_inline)) static inline void pinMode(int PINNUM, int Mode)
{
    gpio_config_t builtInLed = {};
    builtInLed.intr_type = GPIO_INTR_DISABLE;
    if (Mode == OUTPUT)  builtInLed.mode = GPIO_MODE_OUTPUT;
    if (Mode == INPUT)  builtInLed.mode = GPIO_MODE_INPUT;
    if (Mode == INPUT_OUTPUT)  builtInLed.mode = GPIO_MODE_INPUT_OUTPUT;
    builtInLed.pin_bit_mask = BIT(PINNUM);
    builtInLed.pull_down_en = 0;
    builtInLed.pull_up_en = 0;

    gpio_config(&builtInLed);
}
void app_main()
{

    volatile uint16_t *sram1_sec = (uint16_t *)0x3FFE0000; /*  Explicit */

    int ***multiArray = (int ***)0x3FFE00FF;

    /* created ptrtab[x][y][z] */
    /* multiArray[5][5][5] */
    multiArray = (int ***)malloc(5 * sizeof(uint16_t **)); /* ptrtab[] */

    uint8_t i = 0;

    for (i = 0; i < 5; i++)
    {
        multiArray[i] = (int **)malloc(5 * sizeof(int *));
    }
    for (i = 0; i < 5; i++)
    {
        multiArray[i] = (int *)malloc(5 * sizeof(int));
    }

    uint16_t cnt = 0;

    func_t addFunction = &returnPtr;

    for (;;)
    {
        *sram1_sec = cnt;

        multiArray[1][1][1] = cnt;

        pinMode(0, INPUT_OUTPUT);

        gpio_set_level(GPIO_NUM_0, cnt % 2);

        printf("%p [%d] {multiArray[%d]} [ptr Func_t :[%d]]\n", sram1_sec, *sram1_sec, multiArray[1][1][1], addFunction(cnt, cnt));
       
        cnt++;

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

int returnPtr(int x, int y)
{

    int ptr;
    ptr = x + y;

    return ptr;
}