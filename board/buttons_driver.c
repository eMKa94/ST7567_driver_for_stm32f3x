/*
 * buttons_driver.c
 *
 *  Created on: 13 gru 2020
 *      Author: mkowalski
 */


#include "buttons_driver.h"

void buttons_state_update(void);

button_t *button_list[BUTTONS_MAX_QTY];
uint8_t	buttons_qty_counter = 0;
uint8_t any_action_flag = 0;

void (*any_action_function_pointer)(void) = &blank_any_action_function;
void (*any_press_function_pointer)(void) = &blank_any_action_function;

bool isButtonAllreadyOnList(button_t * button)
{
	bool button_allready_on_list = false;
	for (int button_n = 0 ; button_n < BUTTONS_MAX_QTY ; button_n++)
	{
		if (button_list[button_n] == button) button_allready_on_list = true;
	}
	return(button_allready_on_list);
}

button_t create_button(void)
{

	button_t new_button;

	new_button.temp_state = released;
	new_button.state = released;
	new_button.counter = 0;
	new_button.button_press_function = &blank_any_action_function;
	new_button.button_hold_function	= &blank_any_action_function;
	new_button.holdFunctionInvoked	= false;
	new_button.any_action_flag = 1;

	return(new_button);

}

void add_button(button_t * button_to_handle)
{
	if (!isButtonAllreadyOnList(button_to_handle))
	{
		if (get_buttons_qty_counter() < BUTTONS_MAX_QTY)
		{
			button_list[buttons_qty_counter ++] = button_to_handle;
		}
	}
}

buton_state check_button_input(button_t * button)
{
	return(button->temp_state);
}

void button_check_routine(void)
{
	
	if (buttons_qty_counter > 0)
	{
		buttons_state_update();
		for (int i = 0 ; i < buttons_qty_counter ; i++)
		{
			if (check_button_input(button_list[i]))
			{
				if (button_list[i]->counter < 10000) button_list[i]->counter ++;
			}
			else
			{
				if ( (button_list[i]->counter >= MIN_PRESS_TIME_MS) && (button_list[i]->counter < MIN_HOLD_TIME_MS) )
				{
					button_list[i]->state = pressed;
				}
				button_list[i]->counter = 0;
			}
			if (button_list[i]->counter >= MIN_HOLD_TIME_MS)
			{
				button_list[i]->state = holded;
			}
		}
	}

}

void button_action_routine(void)
{
	uint8_t any_action_indicator = 0;
	if (buttons_qty_counter > 0)
	{
		for (int i = 0 ; i < buttons_qty_counter ; i++)
		{
			if ( (button_list[i]->counter > MIN_PRESS_TIME_MS) && (button_list[i]->any_action_flag != 0) )
			{
				any_action_indicator = 1;
			}
		    switch (button_list[i]->state)
		    {

				case pressed:
					button_list[i]->button_press_function();
					button_list[i]->state = released;
					any_press_function_pointer();
					break;

				case holded:
					if (button_list[i]->holdFunctionInvoked == false)
					{
						button_list[i]->button_hold_function();
						button_list[i]->holdFunctionInvoked = true;
					}
					if (button_list[i]->counter < MIN_HOLD_TIME_MS)
					{
						button_list[i]->state = released;
						button_list[i]->holdFunctionInvoked = false;
					}
					break;

				case released:
				default:
					break;

		    }
		}
		if (any_action_indicator == 0)
		{
			any_action_flag = 0;
		}
		else
		{
			if (any_action_flag == 0)
			{
				any_action_flag = 1;
				any_action_function_pointer();

			}
		}
	}
}

void assignButtonPressFunction(button_t * button, void (*function_pointer)(void))
{
	button->button_press_function = function_pointer;
}

void assignButtonHoldFunction(button_t * button, void (*function_pointer)(void))
{
	button->button_hold_function = function_pointer;	
}

void button_any_action_enable(button_t * button, bool en_ds)
{
	button->any_action_flag = en_ds;
}

void assign_any_action_function(void (*function_pointer)(void))
{
	any_action_function_pointer = function_pointer;
}

void assign_any_press_function(void (*function_pointer)(void))
{
	any_press_function_pointer = function_pointer;
}

uint8_t get_buttons_qty_counter(void)
{
	return(buttons_qty_counter);
}

void blank_any_action_function(void)
{

}

__attribute__((weak)) void buttons_state_update(void)
{

}
