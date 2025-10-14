#include <stdint.h>
#include <stm32f446xx.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "util.h"

#define DEBUG_UART

#define TAILLE_MAX_COMPRESS 500

//Texte non compresse
uint8_t texte[] = "aaaabbbccd";

//Texte compresse
uint8_t texteCompress[TAILLE_MAX_COMPRESS];

//Tableau du nombre d'occurrence de chaque caractere
uint32_t tabCaractere[256] = {0};

//Nombre de caractere total dans le texte non compresse
uint32_t nbrCaractereTotal = 0;

//Nombre de caractere different dans le texte non compresse
uint32_t nbrCaractereDifferent = 0;

//Arbre de Huffman
//struct noeud* arbreHuffman[256];

/**
 * @brief
 *
 * @param chaine adresse du tableau du texte non compresse
 * @param tab tableau qui sert a compte le nombre d'occurence dans le texte
 * @return uint32_t  nombre de caractere dans le texte
 */
uint32_t occurence(uint8_t* chaine, uint32_t tab[256])
{
	uint32_t ret_nbrCaractereTotal = 0;
	uint8_t l_ui8_while = 1;
	uint32_t iBcl = 0;
	do{
		if(chaine[iBcl] == '\0')
		{
			l_ui8_while = 0;
		}
		else
		{
			tab[chaine[iBcl]] = tab[chaine[iBcl]] + 1;


#ifdef DEBUG_UART
			printf("caractere = %c, occurence = %i \n", chaine[iBcl], tab[chaine[iBcl]]);
#endif
		}

		iBcl++;
	}while(l_ui8_while);

	ret_nbrCaractereTotal = iBcl - 1;

#ifdef DEBUG_UART
			printf("caractere total = %i \n", ret_nbrCaractereTotal);
#endif

	return ret_nbrCaractereTotal;
}

int main(void)
{
	//HAL_Init();
	//SystemClock_Config();

	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();

	nbrCaractereTotal = occurence(texte, tabCaractere);

	while(1){
		SYSTICK_Delay(500);
		GPIOA->ODR ^= 1<<5;
		//printf("Hello\r\n");
	}
}
