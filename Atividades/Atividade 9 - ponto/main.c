#include "stm32f4xx.h"
#include "Utility.h"

typedef struct
{
	uint8_t formato, //especifica o formato da hora
	dia, //especifica o dia da semana
	data, //especifica a data do mês
	mes, //especifica o mês
	ano, //especifica o ano
	horas, //especifica a hora
	minutos, //especifica os minutos
	segundos; //especifica os segundos
} RTC_CalendarTypeDef;

void RTC_Config(void);
void RTC_SetCalendar(RTC_CalendarTypeDef* RTC_CalendarStruct);


char dias[8][20] = {
	"",
	"segunda-feira",
	"terça-feira",
	"quarta-feira",
	"quinta-feira",
	"sexta-feira",
	"sábado",
	"domingo"
};

char meses[13][20] = {
	"",
	"janeiro",
	"fevereiro",
	"março",
	"abril",
	"maio",
	"junho",
	"julho",
	"agosto",
	"setembro",
	"outubro",
	"novembro",
	"dezembro"
};


int main(void)
{
    Configure_Clock(); // configurando o clock para poder usar o delay
    Delay_Start(); // configurando o timer
    USART1_Init(); // configurando a USART1
    RTC_Config();

    RTC_CalendarTypeDef RTC_Calendar;

    RTC_Calendar.formato = 0;
    RTC_Calendar.dia = 6;
    RTC_Calendar.data = 3;
    RTC_Calendar.mes = 12;
    RTC_Calendar.ano = 22;
    RTC_Calendar.horas= 16;
    RTC_Calendar.minutos = 0;
    RTC_Calendar.segundos = 0;

    RTC_SetCalendar(&RTC_Calendar);


    int registador_segundos_anteior = RTC->TR;
	while(1) {
		while (registador_segundos_anteior == RTC->TR);
		registador_segundos_anteior = RTC->TR;

		// registrador 1
		uint8_t du = RTC->DR & (RTC_DR_DU);
		uint8_t dt = (RTC->DR & (RTC_DR_DT)) >> 4;
		uint8_t mu = (RTC->DR & (RTC_DR_MU)) >> 8;
		uint8_t mt = (RTC->DR & (RTC_DR_MT)) >> 12;
		uint8_t wdu = (RTC->DR & (RTC_DR_WDU)) >> 13;
		uint8_t yu = (RTC->DR & (RTC_DR_YU)) >> 16;
		uint8_t yt = (RTC->DR & (RTC_DR_YT)) >> 20;


		uint8_t mes = 10*mt + mu;

		// registrador 2
		uint8_t su = RTC->TR & (RTC_TR_SU);
		uint8_t st = (RTC->TR & (RTC_TR_ST)) >> 4;
		uint8_t mnu = (RTC-> TR & (RTC_TR_MNU)) >> 8;
		uint8_t mnt = (RTC-> TR & (RTC_TR_MNT)) >> 12;
		uint8_t hu = (RTC-> TR & (RTC_TR_HU)) >> 16;
		uint8_t ht = (RTC-> TR & (RTC_TR_HT)) >> 20;

		printf("Hoje é %s, %d%d de %s de 20%d%d.\n",dias[wdu], dt, du, meses[mes], yt, yu);
		printf("A hora atual é %d%d:%d%d:%d%d.\n\n", ht, hu, mnt, mnu, st, su);
	}
}

void RTC_Config(void)
{
	//Habilitando o acesso ao domínio de backup
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; //habilita o clock da interface de energia
	PWR->CR |= PWR_CR_DBP; //acesso de escrita aos registradores do RTC
	RCC->APB1ENR &= ~RCC_APB1ENR_PWREN; //desabilita o clock da interface de energia
	RCC->BDCR |= RCC_BDCR_BDRST; //reseta o domínio de backup
	RCC->BDCR &= ~RCC_BDCR_BDRST; //reativa o domínio de backup

	//Configurando a fonte de clock para o RTC no registrador BDCR do módulo RCC
	RCC->BDCR &= ~(RCC_BDCR_LSEON | RCC_BDCR_LSEBYP); //desliga o oscilador LSE
	RCC->BDCR |= RCC_BDCR_LSEON; //liga o oscilador LSE
	while(!(RCC->BDCR & RCC_BDCR_LSERDY)); //espera o clock LSE ficar pronto
	RCC->BDCR &= ~RCC_BDCR_RTCSEL; //limpa os bits RTCSEL
	RCC->BDCR |= RCC_BDCR_RTCSEL_0; //seleciona LSE como fonte
	RCC->BDCR |= RCC_BDCR_RTCEN; //habilita o clock do RTC

	//Desabilitando a proteção de escrita dos registradores do RTC
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	//Geração do sinal de clock de 1Hz a partir de LSE (32,768 kHz)
	RTC->ISR |= RTC_ISR_INIT; //entra no modo de inicialização
	while(!(RTC->ISR & RTC_ISR_INITF)); //aguarda o modo de inicialização
	RTC->PRER |= 255; //prescaler síncrono = 255
	RTC->PRER |= (127 << 16); //prescaler assíncrono = 127
	RTC->ISR &= ~RTC_ISR_INIT; //sai do modo de inicialização

	//Reabilitando a proteção de escrita nos registradores do RTC
	RTC->WPR = 0xFF;
}


void RTC_SetCalendar(RTC_CalendarTypeDef* RTC_CalendarStruct)
{
	//Habilitando o acesso ao domínio de backup
	RCC->APB1ENR |= RCC_APB1ENR_PWREN; //habilita o clock da interface de energia
	PWR->CR |= PWR_CR_DBP; //acesso de escrita aos registradores do RTC
	RCC->APB1ENR &= ~RCC_APB1ENR_PWREN; //desabilita o clock da interface de energia

	//Desabilitando a proteção de escrita dos registradores do RTC
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	RTC->ISR |= RTC_ISR_INIT; //entra no modo de inicialização
	while(!(RTC->ISR & RTC_ISR_INITF)); //aguarda o modo de inicialização

	//Escolha do formato da hora
	if(RTC_CalendarStruct->formato)
	RTC->CR |= RTC_CR_FMT; //formato AM/PM
	else
	RTC->CR &= ~RTC_CR_FMT; //formato 24h

	//Atualização de data e hora
	uint8_t dezena_data = (RTC_CalendarStruct->data)/10;
	uint8_t unidade_data = (RTC_CalendarStruct->data) - 10*dezena_data;
	uint8_t dezena_mes = (RTC_CalendarStruct->mes)/10;
	uint8_t unidade_mes = (RTC_CalendarStruct->mes) - 10*dezena_mes;
	uint8_t dezena_ano = (RTC_CalendarStruct->ano)/10;
	uint8_t unidade_ano = (RTC_CalendarStruct->ano) - 10*dezena_ano;
	uint8_t dezena_hora = (RTC_CalendarStruct->horas)/10;
	uint8_t unidade_hora = (RTC_CalendarStruct->horas) - 10*dezena_hora;
	uint8_t dezena_minuto = (RTC_CalendarStruct->minutos)/10;
	uint8_t unidade_minuto = (RTC_CalendarStruct->minutos) - 10*dezena_minuto;
	uint8_t dezena_segundo = (RTC_CalendarStruct->segundos)/10;
	uint8_t unidade_segundo = (RTC_CalendarStruct->segundos) - 10*dezena_segundo;

	RTC->TR = dezena_hora<<20 | unidade_hora<<16 | //ajusta a hora como HH:MM:SS
	dezena_minuto<<12 | unidade_minuto<<8 |
	dezena_segundo<<4 | unidade_segundo;

	RTC->DR = dezena_ano<<20 | unidade_ano<<16 | //ajusta a data como YY/MM/DD

	(RTC_CalendarStruct->dia)<<13 |
	dezena_mes<<12 | unidade_mes<<8 |
	dezena_data<<4 | unidade_data;

	//Saída do modo de inicialização
	RTC->ISR &= ~RTC_ISR_INIT;
	while(!(RTC->ISR & RTC_ISR_RSF)); //aguarda o calendário sincronizar

	//Reabilitando a proteção de escrita nos registradores do RTC
	RTC->WPR = 0xFF;
}
