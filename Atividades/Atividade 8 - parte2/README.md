# Atividade 8 - Ponto

> Na segunda parte tinhamos que implementar uma forma de achar o valor de pi usando apenas a geração de números aleatórios.

## Código desenvolvido

</br>

**main.c**
```c
#include "stm32f4xx.h"
#include "Utility.h"

void USART1_IRQHandler(void)
{
    __io_getchar(); // so pra nao ficar dando eco no terminal
}

#define existeNovoDadoAleatorio (RNG->SR & RNG_SR_DRDY)
#define MAIOR_INTEIRO 0xFFFFFFFF
#define ITERACOES 999999
#define RAIO 1


float geraValorAleatorioNecessario() {
    while (!existeNovoDadoAleatorio);

    return (float)  RNG->DR/MAIOR_INTEIRO;
}

int main(void)
{
    Configure_Clock(); // configurando o clock para poder usar o delay
    Delay_Start(); // configurando o timer
    USART1_Init(); // configurando a USART1

    RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
    RNG->CR |= RNG_CR_RNGEN;


    float x;
    float y;

    int pontosDentroDoCirculo;


    int porcentagem;
    int porcento10 = (int) ITERACOES/10;
    int porcentoAtual;
    while(1) {

        porcentagem = 1;
        pontosDentroDoCirculo = 0;
        porcentoAtual = porcento10;
        for (int i = 0; i < ITERACOES; ++i) {
            x = geraValorAleatorioNecessario();
            y = geraValorAleatorioNecessario();

            float xQuadrado = x*x;
            float yQuadrado = y*y;


            if ((xQuadrado + yQuadrado) < RAIO) {
                pontosDentroDoCirculo++;
            }

            // print de porcentagem
            if (porcentoAtual == i) {
                printf("------ %d%% ------\n", porcentagem*10);
                porcentagem++;
                porcentoAtual += porcento10;
            }
        }

        printf("PI: %.4f\n", (float) 4*pontosDentroDoCirculo/ITERACOES);
        Delay_ms(1000);
    }
}
```
