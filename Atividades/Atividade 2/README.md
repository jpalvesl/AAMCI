# Atividade 2

> Consiste em configurar os pinos que estão ligados nos leds (D2 e D3) presentes na placa como saída, e também configurar os pinos dos botões (K0 e K1) como entrada e para evitar flutuação quando a chave estiver aberta adicionar o resistor de pull-up. Ao apertar K0, D2 deve ser ligado e ao apertar K1, D3 tem que ser ligado.


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



	while(1){
		uint16_t leitura = GPIOE->IDR;

		if(leitura & (1<<4))               // Testa se o PE4 está em nível alto (chave aberta)
		{
			GPIOA->ODR |= 1<<6;
		}
		else
		{
		  GPIOA->ODR &= ~(1 << 6);
		}


		if(leitura & (1<<3))               // Testa se o PE3 está em nível alto (chave aberta)
		{
			GPIOA->ODR |= 1<<7;
		}
		else
		{
		  GPIOA->ODR &= ~(1 << 7);
		}
	}
}
```

<div style="display:flex;">
	<a style="display:inline-block; margin-right:auto;" href="../Atividade 1">Atividade anterior</a>
	<a style="display:inline-block;" href="../Atividade 3">Próxima atividade</a>
</div>