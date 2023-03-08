#include "app.h"

#include "stm32h7xx_ll_gpio.h"
#include "main.h"
#include "tx_api.h"
#include <cstdio>

#define DEMO_STACK_SIZE		2500
TX_THREAD   thread_0;
uint32_t    thread_0_stack[DEMO_STACK_SIZE / sizeof(uint32_t)];

void thread_0_entry(ULONG thread_input)
{
    (void)thread_input;

    while(1) {
        LL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
        LL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

        tx_thread_sleep(200);
    }
}


/*!
 * @brief User defined initialization function
 */
void tx_application_define(void *first_unused_memory)
{
    (void)first_unused_memory;

    (void)tx_thread_create(&thread_0, const_cast<char*>("thread 0"), thread_0_entry, 0,
                           &thread_0_stack[0], DEMO_STACK_SIZE, 1, 1, TX_NO_TIME_SLICE, TX_AUTO_START);
}

void app_entry()
{
    /* hw init */
    tx_kernel_enter();
}
