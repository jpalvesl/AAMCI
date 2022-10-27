#include "stm32f4xx.h"
#include "Utility.h"

// comando que ja esta presente em  Utility, porem estamos apenas explicitado que agora estamos dando um eco no conteudo que a USART ESTA RECEBENDO
void USART1_IRQHandler(void)
{
	__io_putchar(__io_getchar());	//lê o dado e reenvia pela USART1
}

int main(void)
{
    Configure_Clock();
    Delay_Start();
    USART1_Init();

	while(1) {
        printf("Teste\n");
        Delay_ms(1000);
	}
}