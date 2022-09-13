/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
			
void atraso() {
	int atraso = 1000000;
	while(atraso) --atraso;
}

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= (0B01 << 12);

	while(1) {
		GPIOA->ODR |= 1 << 6;
		atraso();
		GPIOA->ODR &= ~(1 << 6);
		atraso();
	}

}
