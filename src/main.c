#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"

/*
Manually utilizing Embedded Memory which in Internal SRAM's
SRAM1 = 0x3FFE_0000   - 0x3FFF_FFFF  128KB
SRAM2 = 0x3FFA_E000   - 0x3FFD_FFFF  200KB
*/

void app_main()
{

    volatile uint16_t *sram1_sec = (uint16_t *)0x3FFE0000; /* created Explicity */

    int ***multiArray = (int ***)0x3FFE00FF;

    /* created ptrtab[x][y][z] */
    /* multiArray[5][50][25] */
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

    for (;;)
    {
        *sram1_sec = cnt;
        multiArray[1][1][1] = cnt;
        cnt++;
        printf("Variable cnt MEM : %p [%d] {multiArray[%d]}\n", sram1_sec, *sram1_sec, multiArray[1][1][1]);
    }
}