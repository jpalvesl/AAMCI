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
	GPIOA->MODER |= (0b0101 << 12);
	//GPIOA->MODER |= (0b01 << 14);

	while(1) {
		GPIOA->ODR |= 0b11 << 6;
		atraso();
		GPIOA->ODR &= ~(0b11 << 6);
		atraso();
	}

}
