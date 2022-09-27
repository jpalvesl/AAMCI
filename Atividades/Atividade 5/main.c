#include "stm32f4xx.h"

void atraso(int delay) {
	while(--delay);
}


int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Habilita o clock no barramento da GPIOA
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // Habilita o clock no barramento da GPIOE
	GPIOA->MODER |= (0b01 << 12);        // Configura PA6 como saída (Led D2)
	GPIOA->MODER |= (0b01 << 14);        // Configura PA7 como saída (Led D3)
	GPIOE->MODER &= ~(0b11 << 6);        // Configura PE3 como entrada
	GPIOE->MODER &= ~(0b11 << 8);        // Configura PE4 como entrada
	GPIOE->PUPDR |= 0b01 << 8 ;          // Habilita o resistor de pull-up em PE4 (Botão K0)
	GPIOE->PUPDR |= 0b01 << 6 ;          // Habilita o resistor de pull-up em PE3 (Botão K1)

	GPIOA->MODER |= (0b01 << 2);         // Configurando PE1 como entrada

	GPIOA->ODR &= ~(1 << 6);
	GPIOA->ODR |= 1 << 7;

	GPIOA->ODR |= (1 << 7);

	while(1) {
		if (!(GPIOE->IDR & (1 << 4))) {
			GPIOA->ODR ^= (1 << 6);
			GPIOA->ODR |= (1 << 1);
			atraso(30000);
			GPIOA->ODR &= ~(1 << 1);
		}

		while(!(GPIOE->IDR & (1 << 4)));

		if (!(GPIOE->IDR & (1 << 3))) {
			GPIOA->ODR ^= (1 << 7);
			GPIOA->ODR |= (1 << 1);
			atraso(30000);
			GPIOA->ODR &= ~(1 << 1);
		}


		while(!(GPIOE->IDR & (1 << 3)));
	}
}