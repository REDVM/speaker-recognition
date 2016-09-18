#ifndef TRAITEMENTSVM__H
#define TRAITEMENTSVM__H



#include <libsvm/svm.h>
#include "includes.h"




void traitementSVM_init();

void traitementSVM_close();

void traitementSVM_entrainerC_SVC(fvec_t* donnees[][MAX_VECTORS_NUMBER], int* nbVecteursDonnees, unsigned int dimensionVecteur, int nbFichiers);

void traitementSVM_verifierDonnees(fvec_t* donnees[MAX_VECTORS_NUMBER], int nbVecteursDonnees, unsigned int dimensionVecteur, int* compteurClasses);

#endif


