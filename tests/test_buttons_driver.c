/*
    test_buttons_driver.c
    Buttons driver tests source file
*/

#include "unity.h"
#include "buttons_driver.h"

#ifdef _TEST
#define BUTTONS_MAX_QTY 4
#endif

void setUp(void);
void tearDown(void);

button_t test_button_1;
button_t test_button_2;
button_t test_button_3;
button_t test_button_4;
button_t test_button_5;

void test_button_create(void)
{
    TEST_ASSERT_EQUAL(released, test_button_1.temp_state);
    TEST_ASSERT_EQUAL(released, test_button_1.state);
    TEST_ASSERT_EQUAL(0, test_button_1.counter);
    TEST_ASSERT_EQUAL(&blank_any_action_function, test_button_1.button_press_function);
    TEST_ASSERT_EQUAL(1, test_button_1.any_action_flag);
}

void test_button_input_check(void)
{
    test_button_1.temp_state = released;
    TEST_ASSERT(check_button_input(&test_button_1) == released);
}


void test_add_button(void)
{
    /*
        1.  Each button should be able to be added to queue only once!
            If button is allready in queue, adding should be ignored
    */
    add_button(&test_button_1);   // add 1
    add_button(&test_button_1);   // add 2
    add_button(&test_button_1);   // add 3

    TEST_ASSERT_EQUAL(1, get_buttons_qty_counter());
    TEST_ASSERT(isButtonAllreadyOnList(&test_button_1));

    /*
        2. Function should not be able to add more buttons then declared maximum buttons amount ()
    */
    add_button(&test_button_2);
    add_button(&test_button_3);
    add_button(&test_button_4);
    add_button(&test_button_5);

    // 5 times button added but the 5-th button should not be in queue and the buttons qty counter should be 4
    TEST_ASSERT(isButtonAllreadyOnList(&test_button_4) == true);
    TEST_ASSERT(isButtonAllreadyOnList(&test_button_5) == false);
    TEST_ASSERT_EQUAL(4, get_buttons_qty_counter());

}
void test_button_function_assign(void)
{

}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_button_create);
    RUN_TEST(test_button_input_check);
    RUN_TEST(test_add_button);

    return UNITY_END();
}

void setUp(void)
{
    test_button_1 = create_button();
    test_button_2 = create_button();
    test_button_3 = create_button();
    test_button_4 = create_button();
    test_button_5 = create_button();
}

void tearDown(void)
{
    test_button_1 = create_button();
    test_button_2 = create_button();
    test_button_3 = create_button();
    test_button_4 = create_button();
    test_button_5 = create_button();
}
