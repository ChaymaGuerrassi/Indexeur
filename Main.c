#include <stdlib.h>
#include <stdio.h>
#include "Fonction.h"
#include <string.h>
#define N 50

int main(int argc, char const *argv[]){

	char choix/*Choix de l'action*/, mot[30]/*Tableau initiale du mot qui va être choisi*/;
	int i,j;//Compteurs
	FILE * fichier;//Fichier texte
	int nbphrase, nbmot;
	int* positiontab;//Tableau contenant les positions des phrases du texte
	int ap = 0, taille, nbposition;

//Partie Index

	fichier = fopen("texte.txt", "r");

	nbphrase = nbrPhrase(fichier);
	nbmot = nbMotFichier(fichier);//On détermine le nombre de mots et de phrases du texte

	Liste tabhachage[N];

	positiontab = malloc(nbphrase*sizeof(int));

	nbposition = tabPositionPhrase(fichier, positiontab);//On détermine le nombre de position possibles et on crée un tableau les contenant

	char *tab[nbmot], *tabinit[nbmot];

	lirefichier(fichier, tab, nbmot);//crée un tableau contenant tout les mots dans l'ordre alphabétique et sans doublon
	liste_mot(fichier, tabinit, nbmot);//Crée un tableau contenant tout les mots

	for(i=0; i<N; i++){
		Cellule *cel = malloc(sizeof(Cellule));
		tabhachage[i] = cel;
	}//On initialise la table de hachage

	remplihachage(tabhachage, tab, nbmot);//On rempli la table de hachage


//Partie Intéraction

	if(argc>1){//Si l'utilisateur entre des arguments
		if(argv[1][0] == '-'){
			choix = argv[1][1];
		}
		if(argc > 2)
			for(i =0; i< sizeof(argv[2]); i++)
				mot[i]=argv[2][i];
	}

	else{//Si aucun arguement n'est rentré
		printf("Que souhaitez vous faire ?\n");
		printf("- Tester l'appartenance d'un mot au texte : a\n");
		printf("- Afficher la liste triée des mots du texte : l\n");
		printf("- Afficher l'ensemble des mots ayant un mot pour préfixe : d\n");
		printf("- Afficher toutes les positions du texte: m\n");
		printf("- Afficher la table de hachage : h\n");

		printf("Votre choix : ");
		scanf(" %c", &choix);
		printf("\n");

		if(choix == 'a' || choix == 'p' || choix == 'P' || choix == 'd'){
			printf("Veuillez entrer votre mot : ");
			scanf(" %s", mot);
		}
	}

	for(i=0; i<30; i++){
		if(mot[i] == '\0'){
			taille = i;
			break;//n détermine la taille précise du mot choisi
		}
	}
			
	char motOk[taille];//On crée un tableau de la taille exacte du mot

	for(i=0; i<taille; i++){
		motOk[i] = mot[i];
	}//On entre le mot dans le tableau adapté
	
	if(choix == 'a'){
		for(i=0; i<nbmot; i++){
			ap =0;//On met le compteur à 0 pour chaque nouveau mot à comparer
			for(j=0; j<100; j++){
				if(motOk[j] == tab[i][j])//Si une lettre correspond on augmente le compteur
					ap += 1;
				if(motOk[j] != tab[i][j]){//Si une lettre est différente on change de mot
					break;
				}
				if(ap == taille){//Si le compteur correspond à la taille du mot, c'est le bon
					if(tab[i][ap+1] == '\0'){//Si le mot est plus long, ce n'est pas le bon
						printf("Votre mot appartient au texte !");
						return 1;}
				}
			}
		}
		printf("Votre mot n'appartient pas au texte !");//Sinon le mot n'appartient pas au texte
	}
	
	if(choix == 'l'){
		printf("La liste des mots du texte, triée est :\n\n");
		for(i=0; i<nbmot; i++){
			if(tab[i][0] != '\0')
				printf("- %s\n", tab[i]);//On affiche un à un les mots présent dans le tableau trié
		}
	}
	
	if(choix == 'd'){
		printf("\nLes mots commençant par '%s' sont :\n\n", mot);
		for(i=0; i<nbmot; i++){
			ap =0;//On met à 0 pour chaque mot à comparer
			for(j=0; j<100; j++){
				if(tab[i][j] == mot[j])//Si la lettre correspond on augmente de 1
					ap += 1;
				if(motOk[j] != tab[i][j]){//Sinon on reinitialise
					break;
				}
				if(ap == taille){//Si le compteur à la taille du mot on affiche puis on continue
					printf("- %s\n", tab[i]);
				}
			}
		}
	}
	
	if(choix == 'h'){
		printf("- Table de Hachage -\n\n");
		affichehachage(tabhachage);//Affiche la table de hachage
	}
	
	if(choix == 'm'){
		printf("Les différentes positions possibles sont :\n");
		for(i=0;i<nbposition;i++)
			printf("- %d\n", positiontab[i]);//Affiche les positions possible du texte
	}

	return 0;

}
