#include "stm32f4xx.h"

void atraso(int valor){
	while(valor > 0) {
		if (!(GPIOE->IDR & (1 << 3))) break;

		--valor;
	}
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


	GPIOA->ODR |= 1<<6;
	GPIOA->ODR |= 1<<7;

	while(1){
		while(!(GPIOE->IDR & (1<<3))); // caso K1 for apertado primeiro trava o programa

		if(!(GPIOE->IDR & (1<<4))) // Verifica se K0 foi pressionado
		{
			atraso(4000000);
			if(!(GPIOE->IDR & (1<<4)) && !(GPIOE->IDR & (1<<3))) { // verifica se ambos os botoes estao sendo pressionados
					GPIOA->ODR &= ~(1<<6); // liga o led
					while (!(GPIOE->IDR & (1<<4)) && !(GPIOE->IDR & (1<<3))); // trava o programa com o led ligado
			}


			else { // caso o K0 se mantenha pressionado apos a verificacao de 1 segundo deve travar o programa
				while(!(GPIOE->IDR & (1<<4)));
			}
		}

		GPIOA->ODR |= (1<<6); // deve desligar o led no fim do loop
	}
}
