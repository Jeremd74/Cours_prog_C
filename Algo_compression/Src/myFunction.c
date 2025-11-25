#include "myFunction.h"

//Texte non compresse
uint8_t texte[] = "aaaabbbccd"; //je mange une banane j aime bien les bananes

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
struct noeud* racineHuffman;

uint32_t occurence(uint8_t* chaine, uint32_t tab[NB_MAX_CARACTERE]){
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

uint32_t creerFeuille(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t tab[NB_MAX_CARACTERE]){
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

void afficherTabArbreHuffman(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t taille){
	for(uint16_t iBcl = 0; iBcl < taille; iBcl++)
	{
		if(arbre[iBcl] != NULL)
		{
			printf("caractere = %c, occurence = %i \n", arbre[iBcl]->c, arbre[iBcl]->occurence);
			printf("code = %i, taille code = %i \n", arbre[iBcl]->code, arbre[iBcl]->tailleCode);
			printf("adresse a gauche = %#x, a droite = %#x \n", arbre[iBcl]->gauche, arbre[iBcl]->droite);
            printf("adresse = %#x\n\n", arbre[iBcl]);
		}
	}
}

void triArbre(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t taille){
	struct noeud* arbre_tempo;
    for (uint16_t iBcl1 = 0; iBcl1 < taille - 1; iBcl1++) 
	{
        for (uint16_t iBcl2 = 0; iBcl2 < taille - iBcl1 - 1; iBcl2++) 
		{
            if (arbre[iBcl2]->occurence > arbre[iBcl2 + 1]->occurence) 
			{
                // Échange des éléments
                arbre_tempo = arbre[iBcl2];
                arbre[iBcl2] = arbre[iBcl2 + 1];
                arbre[iBcl2 + 1] = arbre_tempo;
            }
        }
    }
}

void creationNoeud(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t taille){
	for (uint16_t iBcl = 1; iBcl < taille; iBcl++)
	{
		struct noeud* newNoeud = (struct noeud*)malloc(sizeof(struct noeud));
		newNoeud->c = '!';
		newNoeud->occurence = arbre[iBcl-1]->occurence + arbre[iBcl]->occurence;
		newNoeud->code = 0;
		newNoeud->tailleCode = 0;
		newNoeud->gauche = arbre[iBcl-1];
		newNoeud->droite = arbre[iBcl];

		arbre[iBcl] = newNoeud;
    }
	racineHuffman = arbre[taille-1];
}

void parcourirArbre(struct noeud* ptrNoeud){
	if(ptrNoeud ->droite==NULL && ptrNoeud ->gauche==NULL)
	{
	printf("Je suis une feuille\r\n");
	}
	else
	{
	printf("Je suis un noeud\r\n");
	parcourirArbre(ptrNoeud ->gauche); // On va a gauche
	parcourirArbre(ptrNoeud ->droite); // On va a droite
	}
}

void creerCode(struct noeud* noeud, uint32_t code, uint32_t taille){
	if(noeud == NULL)
		return;

	// Si c'est une feuille (pas de fils gauche et droit)
	if(noeud->gauche == NULL && noeud->droite == NULL)
	{
		noeud->code = code;
		noeud->tailleCode = taille;
		
		#ifdef DEBUG_UART_CODES
		printf("Caractere '%c' : Code = ", noeud->c);
		for(int i = taille - 1; i >= 0; i--)
		{
			printf("%i", (code >> i) & 1);
		}
		printf(" (taille: %lu bits)\n", taille);
		#endif
		
		return;
	}
	
	// Parcours récursif : gauche = 0, droite = 1
	if(noeud->gauche != NULL)
		creerCode(noeud->gauche, code << 1, taille + 1);
	
	if(noeud->droite != NULL)
	creerCode(noeud->droite, (code << 1) | 1, taille + 1);
}
