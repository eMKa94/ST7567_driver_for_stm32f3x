/*
 * emka_button.h
 *
 *  Created on: 4 gru 2020
 *      Author: mkowalski
 */

#ifndef EMKA_BUTTON_H_
#define EMKA_BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

#define	BUTTONS_MAX_QTY		4	//Predefined buttons quantity
#define MIN_PRESS_TIME_MS	5
#define MIN_HOLD_TIME_MS	2000

typedef enum button_state
{
	released,
	pressed,
	holded
}buton_state;

typedef struct button_t
{
	buton_state		temp_state;
	buton_state		state;
	uint16_t		counter;
	bool			any_action_flag;
	bool			holdFunctionInvoked;
	void (*button_press_function)(void);
	void (*button_hold_function)(void);

}button_t;

button_t create_button();
void add_button(button_t *);
buton_state check_button_input(button_t *);
void button_check_routine(void);
void button_action_routine(void);
void blank_any_action_function(void);
void button_any_action_enable(button_t * button, bool en_ds);
void assignButtonPressFunction(button_t * button, void (*function_pointer)(void));
void assignButtonHoldFunction(button_t * button, void (*function_pointer)(void));
void assign_any_action_function(void (*function_pointer)(void));
void assign_any_press_function(void (*function_pointer)(void));
uint8_t get_buttons_qty_counter(void);
bool isButtonAllreadyOnList(button_t * button);


#endif /* EMKA_BUTTON_H_ */
