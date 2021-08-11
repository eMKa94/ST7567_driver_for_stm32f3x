/*
    Board_core.c
    Board core driver source file
*/

#include "board_core.h"

#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_utils.h"


uint64_t board_runtime_ms=0;

void SystemClock_Config(void)
{
  /* Set FLASH latency */ 
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  /* Enable HSE and wait for activation*/
  LL_RCC_HSE_Enable(); 
  while(LL_RCC_HSE_IsReady() != 1) 
  {
  };

    LL_RCC_HSI_Enable();
    while(!LL_RCC_HSI_IsReady());
  
  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  
  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
  
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
  
   /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1); 
    
  /* Set systick to 1ms in using frequency set to 72MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSE_VALUE, LL_RCC_PLL_MUL_9) */
  LL_Init1msTick(72000000);
  
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(72000000);

}

void board_core_init(void)
{
  SystemClock_Config();
  SysTick_Config(72000000/1000);
  fpuEnable();
}


void fpuEnable(void)
{
	// Enable access to coprocessor registers
	SCB->CPACR |= (0x0F << 20);
	// 
	FPU->FPCCR |= (1 << FPU_FPCCR_THREAD_Pos) | (1 << FPU_FPCCR_HFRDY_Pos);
}

// Systick interrupt handler function
void SysTick_Handler(void)
{
	if ((board_runtime_ms % 1000) == 0)
	{
		board_1s_routine();
	}

	if ((board_runtime_ms % 100) == 0)
	{
		board_100ms_routine();
	}
    
    board_runtime_ms++;
	board_1ms_routine();
}


void board_wait_ms(uint32_t delay_ms)
{
  uint64_t stop_time = board_runtime_ms + (uint64_t)delay_ms;
	
	while (board_runtime_ms <= stop_time)
	{
    __NOP();
		if (board_runtime_ms >= stop_time)
		{
			break;
		}
	}
}

void boardReboot(void)
{
  	__NVIC_SystemReset();
}