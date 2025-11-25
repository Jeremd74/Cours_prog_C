#include <stdint.h>
#include <stm32f446xx.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "util.h"
#include "myFunction.h"

int main(void)
{

	uint32_t codeHuffman = 0;

	//HAL_Init();
	//SystemClock_Config();

	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();

	printf("\n\n\n<----------------Start---------------->\n");

	nbrCaractereTotal = occurence(texte, tabCaractere);

	printf("\n");
	nbrCaractereDifferent = creerFeuille(arbreHuffman, tabCaractere);

	#ifdef DEBUG_UART_AFFICHE_ARBRE
	printf("\n<----------------Affichage---------------->\n");
	afficherTabArbreHuffman(arbreHuffman, nbrCaractereDifferent);
	#endif

	triArbre(arbreHuffman, nbrCaractereDifferent);

	#ifdef DEBUG_UART_AFFICHE_TRI
	printf("<----------------Affichge apres tri---------------->\n");
	afficherTabArbreHuffman(arbreHuffman, nbrCaractereDifferent);
	#endif

	creationNoeud(arbreHuffman, nbrCaractereDifferent);

	#ifdef DEBUG_UART_AFFICHE_NOEUD
	printf("<----------------Affichge apres noeud---------------->\n");
	afficherTabArbreHuffman(arbreHuffman, nbrCaractereDifferent);
	#endif

	#ifdef DEBUG_UART_PARCOUR_ARBRE
	printf("<----------------Affichge parcoursArbre---------------->\n");
	parcourirArbre(racineHuffman);
	#endif

	printf("\n<----------------Creation du code---------------->\n");
	creerCode(racineHuffman, codeHuffman, nbrCaractereDifferent);

	printf("\n<----------------Get Address---------------->\n");
	struct noeud* Add_lue = getAddress(arbreHuffman, 'c');
	
	afficherTabArbreHuffman(Add_lue, 1);

	/*
	for(uint32_t i = 0; i < nbrCaractereDifferent; i++)
    {
        if(arbreHuffman[i] != NULL)
        {
            free(arbreHuffman[i]);
            arbreHuffman[i] = NULL;
        }
    }*/

	while(1){
		SYSTICK_Delay(500);
		GPIOA->ODR ^= 1<<5;
		//printf("Hello\r\n");
	}
}
