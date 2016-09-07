#include <p18f4550.h>
#include <delays.h>
#include <usart.h>
#include <stdio.h>

#pragma config PLLDIV	 = 5
#pragma config CPUDIV 	= OSC1_PLL2
#pragma config FOSC   	= HSPLL_HS
#pragma config WDT      = OFF   

// to do: Fazer algoritmo que crie uma estrutura do tipo {{{RGB},{RGB}}},{{RGB},{RGB}}}, por exemplo, para dar ctrl + c e colar a imagem aqui.
rom unsigned char mapPixelsSour[3][3][3] = {{{10,20,30},{100,150,200},{7,8,9}},
											{{1,2,3},{7,8,9},{7,8,9}}};
rom unsigned char mapPixelsDest[2][2][3] = {{{10,20,30},{100,150,200}},
											{{1,2,3},{7,8,9}}};

// Variáveis de resultado,conversão e envio
unsigned char channelBuffer[4],centena,dezena,unidade,numero;
unsigned char pixelRGB[1][1][3];// Variável de resultado em cada pixel do blend alpha entre as duas imagens

// Converte um numero char em unidades de char para serem enviados pela USART. Ex: Entrada: char 123 Converte em: char 1 char 2 char 3 Saída: "123"
void channelToUSART(int channel){
	numero = pixelRGB[0][0][channel];
	if(numero < 10){
		unidade = numero%10;
		sprintf(channelBuffer,"%c ",unidade+48);
	}
	else if(numero < 100){
		unidade = numero%10;
	    numero /= 10;
	    dezena = numero%10;
		sprintf(channelBuffer,"%c%c ",dezena+48,unidade+48);
	}
	else{
		unidade = numero%10;
    numero /= 10;
    dezena = numero%10;
    numero /= 10;
    centena = numero%10;
	sprintf(channelBuffer,"%c%c%c ",centena+48,dezena+48,unidade+48);
	}
}

void rgbToUSART(){
	channelToUSART(0);
	putsUSART(channelBuffer);
	channelToUSART(1);
	putsUSART(channelBuffer);
	channelToUSART(2);
	putsUSART(channelBuffer);
}

void configuraPIC(){
	TRISC = 0b10000000;
}

void configuraUSART(){
	OpenUSART(USART_TX_INT_OFF &
                USART_RX_INT_OFF &
                USART_ASYNCH_MODE &
                USART_EIGHT_BIT &
                USART_SYNC_SLAVE &
                USART_CONT_RX & 
                USART_BRGH_HIGH, 154);
}

void main(){
	configuraPIC();
	configuraUSART();
	// Exemplo
	pixelRGB[0][0][0] = mapPixelsDest[0][0][0] + mapPixelsSour[1][1][0];
	pixelRGB[0][0][1] = mapPixelsDest[0][0][1] + mapPixelsSour[1][1][1];
	pixelRGB[0][0][2] = mapPixelsDest[0][0][2] + mapPixelsSour[1][1][2];
	while (1) {
		rgbToUSART();
		Delay10KTCYx(100);
	}
}
