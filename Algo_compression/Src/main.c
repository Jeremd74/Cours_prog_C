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

//#define DEBUG_UART_OCCURENCE
//#define DEBUG_UART_CREA_FEUILLE

#define TAILLE_MAX_COMPRESS 500
#define NB_MAX_CARACTERE 256

struct noeud{
	uint8_t c; 						//Caractere initial
	uint32_t occurence;				//Nombre d'occurence
	uint32_t code;					//Code binaire dans l'arbre
	uint32_t tailleCode;			//Nombre de bits dans le code
	struct noeud *gauche, *droite;	//Liens vers les noeuds suivants
};

//Texte non compresse
uint8_t texte[] = "aaaabbbccd";

//Texte compresse
uint8_t texteCompress[TAILLE_MAX_COMPRESS];

//Tableau du nombre d'occurrence de chaque caractere
uint32_t tabCaractere[NB_MAX_CARACTERE] = {0};

//Nombre de caractere total dans le texte non compresse
uint32_t nbrCaractereTotal = 0;

//Nombre de caractere different dans le texte non compresse
uint32_t nbrCaractereDifferent = 0;

//Arbre de Huffman
struct noeud* arbreHuffman[NB_MAX_CARACTERE];

/**
 * @brief compte le nombre total de caractere differents et compte le nombre d'occurence de chaque caractere
 *
 * @param chaine adresse du tableau du texte non compresse
 * @param tab tableau qui sert a compte le nombre d'occurence dans le texte
 * @return uint32_t  nombre de caractere dans le texte
 */
uint32_t occurence(uint8_t* chaine, uint32_t tab[NB_MAX_CARACTERE])
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

#ifdef DEBUG_UART_OCCURENCE
			printf("caractere = %c, occurence = %i \n", chaine[iBcl], tab[chaine[iBcl]]);
#endif
		}

		iBcl++;
	}while(l_ui8_while);

	ret_nbrCaractereTotal = iBcl - 1;

#ifdef DEBUG_UART_OCCURENCE
			printf("caractere total = %i \n", ret_nbrCaractereTotal);
#endif

	return ret_nbrCaractereTotal;
}

/**
 * @brief Creer une feuille et avec son caractere et son nombre d'occurence
 * 
 * @param arbre tableau de l'arbre de Huffman
 * @param tab Tableau d'occurence des caractere
 * @return uint32_t  nombre de caractere different dans le texte
 */
 uint32_t creerFeuille(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t tab[NB_MAX_CARACTERE])
{
	uint32_t ret_nbrCaractereDifferent = 0;
	uint16_t l_ui8_cptArbre = 0;
	for(uint16_t iBcl = 0; iBcl < NB_MAX_CARACTERE; iBcl++)
	{
		if(tab[iBcl] != 0)
		{
			
			arbre[l_ui8_cptArbre] = (struct noeud*)malloc(sizeof(struct noeud));
			arbre[l_ui8_cptArbre]->c = iBcl;
			arbre[l_ui8_cptArbre]->occurence = tab[iBcl];
			arbre[l_ui8_cptArbre]->code = 0;
			arbre[l_ui8_cptArbre]->tailleCode = 0;
			arbre[l_ui8_cptArbre]->gauche = NULL;
			arbre[l_ui8_cptArbre]->droite = NULL;

#ifdef DEBUG_UART_CREA_FEUILLE
			printf("caractere = %c, occurence = %i \n", arbre[l_ui8_cptArbre]->c, arbre[l_ui8_cptArbre]->occurence);
#endif
			l_ui8_cptArbre++;
		}
	}
	ret_nbrCaractereDifferent = l_ui8_cptArbre;

#ifdef DEBUG_UART_CREA_FEUILLE
			printf("caractere total different = %i \n", ret_nbrCaractereDifferent);
#endif

	return ret_nbrCaractereDifferent;
}

void afficherTabArbreHuffman(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t taille)
{
	struct noeud* affiche;
	for(uint16_t iBcl = 0; iBcl < taille; iBcl++)
	{
		if(arbre[iBcl] != NULL)
		{
			affiche = arbre[iBcl];
			printf("caractere = %c, occurence = %i \n", affiche->c, affiche->occurence);
			printf("code = %i, taille code = %i \n", affiche->code, affiche->tailleCode);
			printf("adresse a gauche = %#x, a droite = %#x \n\n", affiche->gauche, affiche->droite);
		}
	}
}

int main(void)
{
	//HAL_Init();
	//SystemClock_Config();

	FPU_Init();
	GPIO_Init();
	USART2_Init();
	SYSTICK_Init();

	printf("\n\r\n\r\n\rStart\n\r");

	nbrCaractereTotal = occurence(texte, tabCaractere);
	printf("\n");
	nbrCaractereDifferent = creerFeuille(arbreHuffman, tabCaractere);
	printf("\n");
	afficherTabArbreHuffman(arbreHuffman, nbrCaractereDifferent);

	for(uint32_t i = 0; i < nbrCaractereDifferent; i++)
    {
        if(arbreHuffman[i] != NULL)
        {
            free(arbreHuffman[i]);
            arbreHuffman[i] = NULL;
        }
    }
	
	while(1){
		SYSTICK_Delay(500);
		GPIOA->ODR ^= 1<<5;
		//printf("Hello\r\n");
	}
}
