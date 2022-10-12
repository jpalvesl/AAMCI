# Atividade 3

> Consiste em configurar os pinos que estão ligados nos leds (D2 e D3) presentes na placa como saída, e também configurar os pinos dos botões (K0 e K1) como entrada e para evitar flutuação quando a chave estiver aberta adicionar o resistor de pull-up. Ao apertar K0 ambos os leds se mantém ligados e ao apertar K1 ambos os leds são desligados.


## Código desenvolvido

</br>

**main.c**
```c
#include "stm32f4xx.h"

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


  // Desliga os leds antes de entrar no loop
	GPIOA->ODR |= 1 << 6;
	GPIOA->ODR |= 1 << 7;

	while(1){
		uint16_t leitura = GPIOE->IDR;

		if(!(leitura & (1 << 3))) // Testa se K1 está fechado (Nível lógico baixo) para desligar os leds
		{
			GPIOA->ODR |= 1 << 6;
			GPIOA->ODR |= 1 << 7;
		}


		if(!(leitura & (1 << 4))) // Testa se K0 está fechado (Nível lógico baixo) para acender os leds
		{
			GPIOA->ODR &= ~(1 << 6);
			GPIOA->ODR &= ~(1 << 7);
		}
	}
}
```

Proxima atividade [Link](../Atividade%204/)