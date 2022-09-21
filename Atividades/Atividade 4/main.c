#include "stm32f4xx.h"

void atraso(int valor){
	while(valor > 0) {
		// vai verificar se o pe3 foi pressionado para sair
		uint16_t k1_pressed = !(GPIOE->IDR & (1<<3));
		GPIOA->ODR |= 1<<6;


		if (k1_pressed) break;


		// vai contar
		--valor;
	}

	//while(!(!(GPIOE->IDR & (1 << 3)) && !(GPIOE->IDR & (1 << 4)))) || !(GPIOE->IDR & (1 << 4)));

}

//void atraso(int valor){
//
//
//	while(1) {
//		int c = 0;
//		// vai verificar se o pe3 foi pressionado para sair
//		uint16_t k1_pressed = !(GPIOE->IDR & (1<<3));
//		GPIOA->ODR |= 1<<6;
//
//
//		if (!(GPIOE->IDR & (1<<3)) && (c <=valor)){
//			GPIOA->ODR &= ~(1<<7);
//			break;
//		}
//
//
//		// vai contar
//		c=c+1;
//
//	}
//
//
//
//}



int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOA->MODER |= (0b01 << 12);
	GPIOA->MODER |= (0b01 << 14);
	GPIOE->MODER &= ~(0b11 << 6); //CONFIGURA PE3 COMO ENTRADA
	GPIOE->MODER &= ~(0b11 << 8); //CONFIGURA PE4 COMO ENTRADA
	GPIOE->PUPDR |= 0b01 << 8 ; //HABILITAR O PULLUP EM PE4
	GPIOE->PUPDR |= 0b01 << 6 ; //HABILITAR O PULLUP EM PE3


	GPIOA->ODR |= 1<<6;
	GPIOA->ODR |= 1<<7;

	while(1){

		uint16_t leitura = GPIOE->IDR;

		if(!(leitura & (1<<4))) //testa se o pe3 está em nível alto
		{
			atraso(4000000);
			if(!(leitura & (1<<3))){
					GPIOA->ODR &= ~(1<<6);
			}
		}
		else{
			GPIOA->ODR |= (1<<6);
		}

		if(!(leitura & (1<<3)))
		{
			while(!(GPIOE->IDR & (1<<3)));
		}


//		GPIOA->ODR |= 1<<6;
//		atraso(4000000);
//		GPIOA->ODR &= ~(1<<6);
//		atraso(4000000);
	}

}


//uint16_t leitura = GPIOE->IDR;
//
//		if(!(leitura & (1<<3)) && !(leitura & (1<<4))) //testa se o pe3 está em nível alto
//		{
//			GPIOA->ODR &= ~(1<<6);
////			GPIOA->ODR |= 1<<7;
//		}
//
//		else {
//			GPIOA->ODR |= 1<<6;			;
//		}
//
//
////		if(!(leitura & (1<<4))) //testa se o pe4 está em nível alto
////		{
////			GPIOA->ODR &= ~(1 << 6);
////			GPIOA->ODR &= ~(1 << 7);
////		}
//
//
////		GPIOA->ODR |= 1<<6;
////		GPIOA->ODR |= 1<<7;