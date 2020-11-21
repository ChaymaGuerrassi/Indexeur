#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLEPHRASE 100
#define TAILLELIGNE 10000
#define N 50

typedef struct celpos{
    int position;
    struct celpos *suivant;
}Celpos, *Listepos;

typedef struct celmot{
    char *mot;
    Listepos positions;
}Celmot;

typedef struct cellule{
    Celmot *valeur;
    struct cellule *suivant;
}Cellule, *Liste;

//Ce fichier regroupe des fonctions de manipulations de fichiers.


//Prototypes

unsigned int hache(char *p);
void fermeFichier(FILE *fichier);
int nbMotFichier(FILE *fichier);
int nbrPhrase(FILE *fichier);
void remplihachage(Liste tabhachage[], char * tab[], int nbmot);
void affichehachage(Liste tabhachage[]);
int doublon(char *tab[], char * mot, int taille_tab);
void trieTab(char  *tab[], int taille_tab);
int tabPositionPhrase(FILE *fichier, int positionPhrase[]);
int lirefichier(FILE *fichier, char *tab[], int taille_tab);
int liste_mot(FILE *fichier, char *tab[], int taille_tab);

//Fonctions

unsigned int hache(char *p){
    unsigned int h=0,g;
    for(;*p;p++){
        h=(h<<4)+*p;
        if(g=h&0xf0000000)/*il y a des valeurs dans les 4 bits de poids fort*/
        {
            h=h^(g>>24);/*elles vont influencer l'octet de poids faible*/
            h=h^g;/*on les supprime du haut de h*/
        }
    }
    return h;
}

void fermeFichier(FILE *fichier){
    
    //Fonction qui ouvre le fichier actuel après l'avoir fermer.

    fclose(fichier);

    fichier = fopen("texte.txt", "r");

    if (fichier == NULL)
        printf("Err");
}

int nbMotFichier(FILE *fichier){
    
    //Fonction qui retourne le nombre de mot dans un fichier.
    
    int nbrmot = 0;
    char *phrase = malloc(sizeof(*phrase));
    
     if (phrase == NULL){
        printf("ERR d'allocation de mémoire pour phrase. \n ");
        exit(EXIT_FAILURE);
    }

     
     while (fgets(phrase, TAILLEPHRASE, fichier) != NULL) {
         
         const char * separateurs = "  -,:.;!?\n\t()'";
         
         char * mot = strtok ( phrase, separateurs );
         //Strtok fonction de string.h : elle récupère les mots d'une phrase à l'aide des séparateurs.
         
         while ( mot != NULL ) {
             
             nbrmot++;
             mot = strtok ( NULL, separateurs );
         }
     
     }
    free(phrase);
    rewind(fichier);
    return nbrmot;
}


int nbrPhrase(FILE *fichier) {
    
    //Fonction qui retourne le nombre de phrase dans un fichier.

    int caractereActuel = 0;
    int cpt = 0;

    do {
        caractereActuel = fgetc(fichier);
        if (caractereActuel == '.' || caractereActuel == '?' || caractereActuel == '!') {
            cpt += 1;
        }
    } while(caractereActuel != EOF);

    fermeFichier(fichier);
    rewind(fichier);
    return cpt;
}

void remplihachage(Liste tabhachage[], char * tab[], int nbmot){
    //Rempli la table de hachage avec les mots présents dans le tableau trié et sans doublon
    int i, curs = 0, positionH;
    Liste tmp;

    for(i=0;i<nbmot;i++){
        if(tab[i][0] != '\0'){
            curs = 0;
            positionH = hache(tab[i])%N;
            tmp = tabhachage[positionH];
            
            while(curs != 1){
                if(tmp->valeur != NULL){
                    Cellule *cel = (Cellule*) malloc(sizeof(Cellule));
                    tmp->suivant = cel;
                    tmp = tmp->suivant;
                }
                else{
                    Celmot *celmot = malloc(sizeof(Celmot));
                    tmp->valeur = celmot;
                    tmp->valeur->mot = tab[i];
                    curs = 1;
                }
            }
        }
    }
}

void affichehachage(Liste tabhachage[]){
    //Fonction qui affiche un à un les mots présents dans la table de hachage 
    int i;
    for(i = 0; i<N; i++){
        if(i<10)
            printf("| %d  |", i);
        else
            printf("| %d |", i);
        if(tabhachage[i]->valeur != NULL){
            while(tabhachage[i] != NULL){
                printf(" %s ",tabhachage[i]->valeur->mot);
                tabhachage[i] = tabhachage[i]->suivant;
            }
        }
        printf("\n");
    }
    printf("\n");
}

int doublon(char *tab[], char * mot, int taille_tab){
    
    //Fonction qui renvoie true quand un mot existe déjà dans un tableau et false sinon.
    
    for (int i =0; i< taille_tab; i++){
        
        if (strcmp(tab[i], mot) == 0){
            return 1;
        }
    }
    
    return 0;
}


void trieTab(char  *tab[], int taille_tab){
    
    //Fonction de trie par ordre alphabétique d'un tableau de mots.
    
    char *tmp = malloc(25 * sizeof(char));
    
    if (tmp ==NULL)
        printf("ERR d'allocation. \n");
    
    
    for (int i = 1; i< taille_tab; i++){
        for (int j = 0; j < taille_tab-1; j++ ){
            
            if (strcasecmp(tab[i], tab[j]) < 0){
                //Fonction de string.h qui compare deux mot en tenant compte des majuscules.
                tmp = tab[j];
                tab[j] = tab[i];
                tab[i] = tmp;
            }
        }
    }
    //free(tmp);//Libération de l'allocation mémoire de la variable temporaire.
    
}

int tabPositionPhrase(FILE *fichier, int positionPhrase[]) {
    
    //Fonction qui remplit le tableau de int en paramètre avec les positions de début des phrases de fichiers.
    
    int caractereActuel = 0;
    int i = 1;
    int position;
    int cpt=0;

    positionPhrase[0] = 0;
    do {
        caractereActuel = fgetc(fichier);
        if (caractereActuel == '.' || caractereActuel == '?' || caractereActuel == '!') {
            
            position = ftell(fichier);
            positionPhrase[i] = position+1; //Position du curseur au début de la phrase suivante.
            //printf("tab : %d\n", positionPhrase[i]);
            cpt+=1;
            i++;
        }
    } while(caractereActuel != EOF);
    return cpt;
}


int lirefichier(FILE *fichier, char *tab[], int taille_tab){

    //Fonction qui lit un fichier et renvoie un tableau de mots triés par ordre lexicographique et sans doublons.
    
     int i = 0;
     int j = 0;
     //int c =0;
     //int nbrmot = 0;


     char *phrase = malloc(sizeof(*phrase));

     if (phrase == NULL){
        printf("ERR d'allocation de mémoire pour phrase2. \n ");
        exit(EXIT_FAILURE);
    }
    
     while(i < taille_tab)
     {
      //Allocation mémoire pour un tableau de char(mot) pour chaque case du tableau.
      tab[i] = malloc(25 * sizeof(char));

      if (tab[i] == NULL){
          printf("ERR d'allocation de mémoire pour tab[i]. \n ");
          exit(EXIT_FAILURE);
      }

      i++;
     }

    rewind(fichier);
    while (fgets(phrase, TAILLEPHRASE, fichier) != NULL) {
        
         const char * separateurs = " -,:.;!?\n\t()'";

         char * mot = strtok ( phrase, separateurs );
         while ( mot != NULL ) {

             if (doublon(tab, mot, taille_tab) == 0){
                 
                 tab[j] = malloc(strlen(mot)+1); //Allocation de mémoire pour un mot.

                 if (tab[j] == NULL){
                     printf("ERR d'allocation de mémoire pour tab[j]. \n ");
                     exit(EXIT_FAILURE);
                 }
                 strcpy(tab[j],mot); //Assignation des mots aux cases du tableau.
                 j++;
             }
             mot = strtok ( NULL, separateurs );
         }

     }

    trieTab(tab, taille_tab);

    /*while (c < taille_tab){
        //Affichage du tableau.
        printf("tab : %s \n",tab[c]);
        c++;
    }*/

    
    return 1;
    free(tab); //Libération de l'espace alloué pour le tableau.
    //fclose(fichier2);
}

int liste_mot(FILE *fichier, char *tab[], int taille_tab){

    //Fonction qui lit un fichier et renvoie un tableau de mots triés par ordre lexicographique et sans doublons.
    
     int i = 0;
     int j = 0;
     //int c =0;
     //int nbrmot = 0;


     char *phrase = malloc(sizeof(*phrase));

     if (phrase == NULL){
        printf("ERR d'allocation de mémoire pour phrase2. \n ");
        exit(EXIT_FAILURE);
    }
    
     while(i < taille_tab)
     {
      //Allocation mémoire pour un tableau de char(mot) pour chaque case du tableau.
      tab[i] = malloc(25 * sizeof(char));

      if (tab[i] == NULL){
          printf("ERR d'allocation de mémoire pour tab[i]. \n ");
          exit(EXIT_FAILURE);
      }

      i++;
     }

    rewind(fichier);
    while (fgets(phrase, TAILLEPHRASE, fichier) != NULL) {
        
         const char * separateurs = " -,:.;!?\n\t()'";

         char * mot = strtok ( phrase, separateurs );
         while ( mot != NULL ) {
   
            tab[j] = malloc(strlen(mot)+1); //Allocation de mémoire pour un mot.

            if (tab[j] == NULL){
                printf("ERR d'allocation de mémoire pour tab[j]. \n ");
                exit(EXIT_FAILURE);
            }
            strcpy(tab[j],mot); //Assignation des mots aux cases du tableau.
            j++;
            mot = strtok ( NULL, separateurs );
         }

     }

    
    
    return 1;
    free(tab); //Libération de l'espace alloué pour le tableau.
    
}
