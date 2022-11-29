#include "stm32f4xx.h"
#include "Utility.h"

void USART1_IRQHandler(void)
{
	__io_getchar(); // so pra nao ficar dando eco no terminal
}

#define existeNovoDadoAleatorio (RNG->SR & RNG_SR_DRDY)
#define aguardaK (USART1->DR != 'k')
#define buscaValorAleatorio (RNG->DR%6) + 1

// variaveis de controle para analisar o jogo e mostrar resultados
uint8_t pontuacao_jogador= 0;
uint8_t pontuacao_computador = 0;
uint8_t rodada = 0;

// variaves que guardarão o valor dos dados rolados
unsigned int valorAleatorioJogador;
unsigned int valorAleatorioComputador;

int main(void)
{
    Configure_Clock(); // configurando o clock para poder usar o delay
    Delay_Start(); // configurando o timer
    USART1_Init(); // configurando a USART1

    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    RNG->CR |= RNG_CR_RNGEN;



	while(1) {
		if (rodada == 0) {
			printf("-------- Bem vindo ao programa de rolar dados! --------\n");
			printf("Para que o jogo inicie precisamos que você insira \"k\"\n");
		}
		else {
			printf("\n-----------------------------------------------------------------------------------------------------------\n\n");
			printf("Pressione \"k\" para a proxima rolagem de dados\n");
		}

		while(aguardaK);
		printf("-----------------------------------------------------------------------------------------------------------\n\n");

		// atribuindo valores aleatorios tanto para o jogador quanto para o computador
		while(!existeNovoDadoAleatorio);
		valorAleatorioJogador = buscaValorAleatorio;

		while(!existeNovoDadoAleatorio);
		valorAleatorioComputador = buscaValorAleatorio;


		// logica para pontuação dos jogadores
		if(valorAleatorioJogador > valorAleatorioComputador){
			pontuacao_jogador++;
		}
		else if(valorAleatorioJogador < valorAleatorioComputador){
			pontuacao_computador++;
		}

		// mostrando prints em tela
		printf("Jogador: %d\n",valorAleatorioJogador);
		printf("Computador: %d\n\n",valorAleatorioComputador);

		if (valorAleatorioJogador == valorAleatorioComputador) {
			printf("--- Ocorreu empate no lançamento, ninguém pontuou\n\n");
		}

		if(pontuacao_computador > pontuacao_jogador){
			printf("%d X %d para o computador\n", pontuacao_computador, pontuacao_jogador);
		}

		else if(pontuacao_computador < pontuacao_jogador){
			printf("%d X %d para o jogador\n", pontuacao_jogador, pontuacao_computador);
		}
		else {
			printf("%d X %d jogo empatado\n", pontuacao_jogador, pontuacao_computador);
		}

		rodada+=1;

		// analisando se alguem ganhou
		if(pontuacao_jogador == 2 || pontuacao_computador == 2) {
			if (pontuacao_jogador == 2 ){
				printf("\nParabéns! Você é o vencedor da melhor de três!!!\n");
			}
			else{
				printf("\nVocê perdeu :(\n");
			}
			pontuacao_jogador = 0;
			pontuacao_computador = 0;
			rodada = 0;

			printf("-\n");
			Delay_ms(500);
			printf("-\n");
			Delay_ms(500);
			printf("-\n\n\n\n");
			Delay_ms(500);

			printf("Iniciando um novo jogo\n");
		}
	}
}
