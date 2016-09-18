#ifndef TRAITEMENTSON__H
#define TRAITEMENTSON__H


#define FILTERS_NUMBER				40
#define BUFFER_SIZE					1024 // 1102 Si on veut une frame de 25 millisecondes (1102 = (25/1000)*44100)
#define OVERLAP_SIZE				BUFFER_SIZE/2

#define NB_FICHIERS_BASE_APPR_MAX	MAX_LOCUTEURS_NUMBER+1

#include <aubio.h>
#include "includes.h"




void traitementSon_init();

void traitementSon_extraireMFCC(char filename_input[MAX_LOCUTEURS_NUMBER][MAX_FILENAME_SIZE], unsigned int nbFichiers, bool isSonVerification);

void traitementSon_fourniDonneesSVM();

int traitementSon_verifierDonneesSVM(int* compteurClasses);

void traitementSon_close();



#endif

