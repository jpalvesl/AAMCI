#include "stm32f4xx.h"
#include "Utility.h"

#define TOGGLE_LED1 'A'
#define TOGGLE_LED2 'B'
#define ACK 'C'

int teveComunicacao = 1;

void buzzer(void);

// ao receber um dado uma interrupcao de fim de comunicacao eh lancada e
void USART1_IRQHandler(void)
{
	uint8_t pacote_recebido = USART1->DR;

	if(pacote_recebido == TOGGLE_LED1) {
		GPIOA->ODR ^= 1 << 6;
		USART1->DR = ACK;
	}

	if(pacote_recebido == TOGGLE_LED2) {
		GPIOA->ODR ^= 1 << 7;
		USART1->DR = ACK;
	}

	if (pacote_recebido == ACK) {
		teveComunicacao = 1;
	}
}

int main(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN; // Habilita o clock no barramento da porta GPIOA
    Configure_Clock();
    Delay_Start();
    USART1_Init();

    GPIOA->ODR |= (1 << 7) | (1 << 6);
	GPIOA->MODER |= (0b01 << 14) | (0b01 << 12) | (0b01 << 2);
//    GPIOE->MODER	// Configurar PE4 e PE3 como entrada
	GPIOE->PUPDR |= (0b01 << 8) | (0b01 << 6);

	GPIOA->ODR &= ~(1 << 1);

	Delay_ms(100); // aguarda os sinais estabilizarem // precisa
	while(1) {
		if (!(GPIOE->IDR & (1 << 4))) { // Verifica se PE4 foi pressionado
			USART1->DR = TOGGLE_LED1;
			//definir uma flag e caso nao seja alterada em x ms
			teveComunicacao = 0;
			Delay_us(100);

			if (!teveComunicacao) {
				GPIOA->ODR ^= 1 << 6;
			}
			//definir uma flag e caso nao seja alterada em x ms

			buzzer();							//sinaliza o fim do envio
			while(!(GPIOE->IDR & (1 << 4)));	//aguarda o botão ser solto
		}

		if (!(GPIOE->IDR & (1 << 3))) { // Verifica se PE3 foi pressionado
			USART1->DR = TOGGLE_LED2;
			//definir uma flag e caso nao seja alterada em x ms
			teveComunicacao = 0;
			Delay_us(100);

			if (!teveComunicacao) {
				GPIOA->ODR ^= 1 << 7;
			}


			buzzer();							//sinaliza o fim do envio
			while(!(GPIOE->IDR & (1 << 3)));	//aguarda o botão ser solto
		}
	}
}

//Função de sinalização de fim de envio de dado
void buzzer(void)
{
	GPIOA->ODR |= 1 << 1;			//liga o buzzer
	Delay_ms(10);					//aguarda
	GPIOA->ODR &= ~(1 << 1);		//desliga o buzzer
	Delay_ms(80);					//aguarda
	GPIOA->ODR |= 1 << 1;			//liga o buzzer
	Delay_ms(10);					//aguarda
	GPIOA->ODR &= ~(1 << 1);		//desliga o buzzer
}
