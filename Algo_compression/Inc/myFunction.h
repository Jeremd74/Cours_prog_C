#ifndef myFunction_h
#define myFunction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

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
extern uint8_t texte[]; 

//Texte compresse
extern uint8_t texteCompress[TAILLE_MAX_COMPRESS];

//Tableau du nombre d'occurrence de chaque caractere
extern uint32_t tabCaractere[NB_MAX_CARACTERE];

//Nombre de caractere total dans le texte non compresse
extern uint32_t nbrCaractereTotal;

//Nombre de caractere different dans le texte non compresse
extern uint32_t nbrCaractereDifferent;

//Arbre de Huffman
extern struct noeud* arbreHuffman[NB_MAX_CARACTERE];
extern struct noeud* racineHuffman;

/**
 * @brief compte le nombre total de caractere differents et compte le nombre d'occurence de chaque caractere
 *
 * @param chaine adresse du tableau du texte non compresse
 * @param tab tableau qui sert a compte le nombre d'occurence dans le texte
 * @return uint32_t  nombre de caractere dans le texte
 */
 uint32_t occurence(uint8_t* chaine, uint32_t tab[NB_MAX_CARACTERE]);


 /**
 * @brief Creer une feuille et avec son caractere et son nombre d'occurence
 * 
 * @param arbre tableau de l'arbre de Huffman
 * @param tab Tableau d'occurence des caractere
 * @return uint32_t  nombre de caractere different dans le texte
 */
 uint32_t creerFeuille(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t tab[NB_MAX_CARACTERE]);

/**
 * @brief Permet d'afficher le contenue de l'arbre
 * 
 * @param arbre tableau de l'arbre de Huffman
 * @param taille Nombre de case dans le tableau de struct
 */
void afficherTabArbreHuffman(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t taille);

/**
 * @brief trie pas ordre croissant les occurances des caracteres
 * 
 * @param arbre tableau de l'arbre de Huffman
 * @param taille Nombre de case dans le tableau de struct
 */
 void triArbre(struct noeud* arbre[NB_MAX_CARACTERE], uint32_t taille);

 void creationNoeud(struct noeud* arbre[NB_MAX_CARACTERE], struct noeud* racine, uint32_t taille);

#endif // myFunction_h