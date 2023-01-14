#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/mutex.h"
#include "pico/sem.h"
#include "FreeRTOS.h" /* Must come first. */
#include "task.h"     /* RTOS task related API prototypes. */
#include "queue.h"    /* RTOS queue related API prototypes. */
#include "timers.h"   /* Software timer related API prototypes. */
#include "semphr.h"   /* Semaphore related API prototypes. */

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

void task0(void *pvParam)
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    for(;;)
    {
        gpio_put(LED_PIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);              // 延时1秒
        gpio_put(LED_PIN, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);              // 延时1秒
    }
}

void task1(void *pvParam)
{
    for(;;)
    {
        uint CORE_ID;
        CORE_ID = portGET_CORE_ID();
        printf("Hello World !\r\n");
        printf("Core ID is %d\r\n",CORE_ID);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

int main()
{
    stdio_init_all();

    /* 创建任务0 */
    TaskHandle_t task0_Handle = NULL;
    UBaseType_t task0_CoreAffinityMask = (1 << 0);              // 指示该任务可以运行在哪个核心上
    xTaskCreate(task0, "task0", 1024, NULL, 1, &task0_Handle);
    vTaskCoreAffinitySet(task0_Handle, task0_CoreAffinityMask); // 为任务设置核心关联掩码

    /* 创建任务1 */
    TaskHandle_t task1_Handle = NULL;
    UBaseType_t task1_CoreAffinityMask = (1 << 1);
    xTaskCreate(task1, "task1", 1024, NULL, 1, &task1_Handle);
    vTaskCoreAffinitySet(task1_Handle, task1_CoreAffinityMask);

    vTaskStartScheduler();

    return 0;
}
