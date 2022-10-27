# Atividade 1

> Consiste em fazer os leds presentes na placa piscarem como também configurar o SystemWorkbench e gravar no microcontrolador.


## Código desenvolvido

</br>

**main.c**
```c
#include "stm32f4xx.h"
			
void atraso() {
	int atraso = 1000000;
	while(atraso) --atraso;
}

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Habilita o clock no barramento da porta GPIOA
	GPIOA->MODER |= (0b0101 << 12);      // Define os pinos PA6 e PA7 como modo de saída 
	//GPIOA->MODER |= (0b01 << 14);      // Define o pino PA7 como saída

	while(1) {
		GPIOA->ODR |= 0b11 << 6;           // Apaga os leds que estão ligados nas portas PA6 e PA7
		atraso();
		GPIOA->ODR &= ~(0b11 << 6);        // Acende os lends que estão ligados em PA6 e PA7
		atraso();
	}
}
```

<div style="display: flex; justify-content:space-between;">
	<a href="../Atividade 2">Atividade anterior</a>
	<a href="../Atividade 2">Próxima atividade</a>
</div>
