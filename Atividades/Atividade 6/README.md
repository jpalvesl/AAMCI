# Atividade 6

> Nessa atividade tivemos que apenas Usar os comandos de Configurar o clock iniciar o timer para poder usarmos o delay e inciar a USART1, para que possamos conectar em algum terminal serial e Podermos ver o que está sendo mandado pelo nosso microcontrolador, assim como o eco recebido caso digitamos algo no terminal e apertarmos enter

## Código desenvolvido

</br>

**main.c**
```c
#include "stm32f4xx.h"
#include "Utility.h"

// comando que ja esta presente em  Utility, porem estamos apenas explicitado que agora estamos dando um eco no conteudo que a USART ESTA RECEBENDO
void USART1_IRQHandler(void)
{
	__io_putchar(__io_getchar());	//lê o dado e reenvia pela USART1
}

int main(void)
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN //habilita o clock do GPIOA

  Configure_Clock();
  Delay_Start();
  USART1_Init();

	while(1) {
    printf("Teste\n")
    Delay_ms(1000);
	}
}
```

Proxima atividade [Link](../Atividade%207%20-%20Ponto/)
