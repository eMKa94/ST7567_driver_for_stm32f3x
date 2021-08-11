/*
    test_buttons_driver.c
    Buttons driver tests source file
*/

#include "unity.h"
#include <string.h>

#define DISPLAY_COLUMNS 128
#define DISPLAY_PAGES   8
#include "gui_core.h"

#ifdef _TEST

#endif

void setUp(void);
void tearDown(void);

uint8_t displayBuffer[DISPLAY_BUFFER_SIZE];

// TEST RUNNERS
void test_getDisplayBufferPosition(void);
void test_clearDisplayBuffer(void);

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_getDisplayBufferPosition);
    RUN_TEST(test_clearDisplayBuffer);

    return UNITY_END();
}

void setUp(void)
{
    memset(&displayBuffer, 0, DISPLAY_BUFFER_SIZE);
    setGuiDisplayBuffer(displayBuffer);
}

void tearDown(void)
{
    memset(&displayBuffer, 0, DISPLAY_BUFFER_SIZE);
}


void test_getDisplayBufferPosition(void)
{
    TEST_ASSERT_EQUAL(0,    getDisplayBufferPosition(0, 0));
    TEST_ASSERT_EQUAL(127,  getDisplayBufferPosition(0, 127));
    TEST_ASSERT_EQUAL(127,  getDisplayBufferPosition(0, 128));  // Col > MAX
    
    TEST_ASSERT_EQUAL(128,  getDisplayBufferPosition(1, 0));
    TEST_ASSERT_EQUAL(255,  getDisplayBufferPosition(1, 127));
    TEST_ASSERT_EQUAL(255,  getDisplayBufferPosition(1, 128));  // Col > MAX

    TEST_ASSERT_EQUAL(896,  getDisplayBufferPosition(7, 0));
    TEST_ASSERT_EQUAL(896,  getDisplayBufferPosition(8, 0));    // Pag > MAX
    TEST_ASSERT_EQUAL(1023,  getDisplayBufferPosition(7, 127));
    TEST_ASSERT_EQUAL(1023,  getDisplayBufferPosition(7, 128)); // Col > Max
    TEST_ASSERT_EQUAL(1023,  getDisplayBufferPosition(8, 128)); // Pag > Max & Col > Max

}

void test_clearDisplayBuffer(void)
{
    memset(displayBuffer, 0xFF, DISPLAY_BUFFER_SIZE);
    clearDisplayBuffer();
    TEST_ASSERT_EQUAL(0, displayBuffer[0]);
    TEST_ASSERT_EQUAL(0, displayBuffer[DISPLAY_BUFFER_SIZE / 2]);
    TEST_ASSERT_EQUAL(0, displayBuffer[DISPLAY_BUFFER_SIZE-1]);
}