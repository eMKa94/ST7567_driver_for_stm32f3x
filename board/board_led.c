#include "board_led.h"

#define BOARD_LED_GPIO_PORT GPIOF
#define BOARD_LED_GPIO_PIN  LL_GPIO_PIN_6
#define board_led_on()      LL_GPIO_ResetOutputPin(BOARD_LED_GPIO_PORT, BOARD_LED_GPIO_PIN)
#define board_led_off()     LL_GPIO_SetOutputPin(BOARD_LED_GPIO_PORT, BOARD_LED_GPIO_PIN)
#define board_led_toggle()  LL_GPIO_TogglePin(BOARD_LED_GPIO_PORT, BOARD_LED_GPIO_PIN)

typedef enum board_led_mode_t
{
    SOLID,
    BLINK
} board_led_mode_t;

board_led_mode_t    board_led_mode          = SOLID;
board_led_state_t   board_led_state         = LED_OFF;
uint32_t            board_led_off_time_ms   = 0;
uint32_t            board_led_on_time_ms    = 0;

ErrorStatus board_led_init(void)
{
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_GPIO_InitTypeDef board_led_gpio;
    board_led_gpio.Pin  = BOARD_LED_GPIO_PIN;
    board_led_gpio.Mode = LL_GPIO_MODE_OUTPUT;
    return(LL_GPIO_Init(BOARD_LED_GPIO_PORT, &board_led_gpio));
}

void board_led_set_solid(board_led_state_t led_state)
{
    board_led_mode = SOLID;
    if (led_state == LED_OFF)
    {
        board_led_state = LED_OFF;
    }
    if (led_state == LED_ON )
    {
        board_led_state = LED_ON;
    }
}

void board_led_set_blink(uint32_t led_on_time_ms, uint32_t led_off_time_ms)
{
    board_led_mode          = BLINK;
    board_led_off_time_ms   = led_off_time_ms;
    board_led_on_time_ms    = led_on_time_ms;
}

void board_led_routine(void)
{
    static uint32_t board_led_ms_counter = 0;
    if (board_led_mode == BLINK)
    {
        switch (board_led_state)
        {
            case LED_OFF:
                if (board_led_ms_counter < board_led_off_time_ms)
                {
                    board_led_ms_counter ++;
                }
                else
                {
                    board_led_state = LED_ON;
                    board_led_ms_counter = 0;
                }
                break;

            case LED_ON:
                if (board_led_ms_counter < board_led_on_time_ms)
                {
                    board_led_ms_counter ++;
                }
                else
                {
                    board_led_state = LED_OFF;
                    board_led_ms_counter = 0;
                }
                break;      

            default:
                break;
        }
    }

    switch (board_led_state)
    {
        case LED_OFF:
            board_led_off();
            break;
        
        case LED_ON:
            board_led_on();
            break;
    
        default:
            break;
    }
}
