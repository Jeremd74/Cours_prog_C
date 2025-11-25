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

	printf("<----------------Affichage---------------->\n");
	afficherTabArbreHuffman(arbreHuffman, nbrCaractereDifferent);

	triArbre(arbreHuffman, nbrCaractereDifferent);

	printf("<----------------Affichge apres tri---------------->\n");
	afficherTabArbreHuffman(arbreHuffman, nbrCaractereDifferent);

	creationNoeud(arbreHuffman, nbrCaractereDifferent);

	printf("<----------------Affichge apres noeud---------------->\n");
	afficherTabArbreHuffman(arbreHuffman, nbrCaractereDifferent);

	printf("<----------------Affichge parcoursArbre---------------->\n");
	parcourirArbre(racineHuffman);

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
