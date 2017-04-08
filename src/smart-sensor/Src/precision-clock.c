#include "precision-clock.h"

#include "tim.h"

volatile uint32_t constant = 0;
volatile uint16_t lastValue = 0;
/*
extern __IO uint32_t uwTick;
uint32_t ticksHiRes = 0;*/

void prec_clock_init()
{
	HAL_TIM_Base_Start(&htim17);
}

/*
void HAL_IncTick(void)
{
  ticksHiRes++;
  if (ticksHiRes%10 == 0)
	  uwTick++;
}*/


uint32_t prec_clock_ticks()
{
	volatile uint16_t v = htim17.Instance->CNT;
	if (v < lastValue)
	{
		// Timer was restarted between calls, so increment constant
		constant += 0xFFFF;
	}
	lastValue = v;
	return (constant + v);
	//return HAL_GetTick();
}

/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim17)
	{
		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
		ticksCount++;
	}
}
*/
