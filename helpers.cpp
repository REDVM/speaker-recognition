#include "helpers.h"





// Retourne l'indice du plus grand entier dans un tableau
int max_indice(int* tab, int nbElements){
	int max = 0;
	int indice_max = 0;
	int i = 0;
	
	for(i = 0; i < nbElements; i++){
		if(tab[i] > max){
			max = tab[i];
			indice_max = i;
		}	
	}	
	return indice_max;
}


void enregistrerTableauFormatLIBSVM(char* filename, fvec_t* donnees[MAX_VECTORS_NUMBER], int nbVecteursDonnees){
	
	int i = 0, j = 0;
	FILE* fichier = NULL;
	char filenameF[256] = "";
	char maChaine[256] = "";
	strcpy(filenameF, filename);
	strcat(filenameF, ".txt");
	fichier = fopen(filenameF, "w");
	
	
	
	if (fichier != NULL)
	{
		printf("Enregistrement des MFCCs dans : %s\n", filenameF);
		for(i = 0; i < nbVecteursDonnees; i++){
			fputc('+', fichier);
			fputc('1', fichier);
			fputc(' ', fichier);
			
			for(j = 0; j < MFCC_NUMBER; j++){
				fprintf(fichier, "%d:%f ", j+1, donnees[i]->data[j]);
			}
			
			
			
			
			fputc('\n', fichier);
		}
		
		
		fclose(fichier);
	} else {
		printf("Erreur a l'enregistrement des MFCCs dans : %s\n", filenameF);
	}


}

