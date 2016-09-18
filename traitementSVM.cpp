#include "traitementSVM.h"

struct svm_model*			svmModel;
struct svm_parameter		svmParametres;	// Paramètres de ma 1C-SVM
struct svm_problem			svmDonnees;			// Les donnees au format de la libsvm


void traitementSVM_init(){
	// Je met toute mes structures à zéro pour pas être emmerdé
	memset(&svmParametres, 0, sizeof(struct svm_parameter));
	memset(&svmDonnees	 , 0, sizeof(struct svm_problem));
	
	svmParametres.svm_type		= C_SVC;
	svmParametres.gamma			= 1.0/MFCC_NUMBER; 
	svmParametres.kernel_type	= RBF;
	svmParametres.cache_size	= 100;
	svmParametres.C				= 1;
	svmParametres.eps			= 0.001;
	svmParametres.nu			= 0.3;
	svmParametres.shrinking		= 0;  // ? A GARDER OU PAS ?
	svmParametres.probability	= 0;
	svmParametres.nr_weight		= 0;
}


void traitementSVM_entrainerC_SVC(fvec_t* donnees[][MAX_VECTORS_NUMBER], int* nbVecteursDonnees, unsigned int dimensionVecteur, int nbFichiers){
	
	// ASSERTIONS
	assert(nbFichiers > 0);
	assert(nbFichiers <= MAX_LOCUTEURS_NUMBER);
	assert(dimensionVecteur  >= MFCC_NUMBER);
	// FIN ASSERTIONS
	
	int h = 0;
	int i = 0;
	int j = 0;
	int somme = 0;
	const char* retour = NULL;
	
	
	// Le nombre de vecteurs dans chaque classe est egal au nombre total de vecteurs
	svmDonnees.l = 0;
	for( i = 0; i < nbFichiers; i++){
		svmDonnees.l += nbVecteursDonnees[i];
	}
	printf("Nombre de vecteurs total : %d\n", svmDonnees.l);
	
	
	somme = 0;
	svmDonnees.y = (double*) malloc(sizeof(double) * svmDonnees.l);
	for(i = 0; i < nbFichiers; i++){ // Affectation des labels
		for(j = 0; j < nbVecteursDonnees[i]; j++){
			svmDonnees.y[somme+j] = i+1;
			//~ printf("%d : %lf\t", somme+j, svmDonnees.y[somme+j]);
		}
		somme += nbVecteursDonnees[i];
	}
	
	
	
	
	somme = 0;
	svmDonnees.x = (struct svm_node**) malloc(sizeof(struct svm_node*) * svmDonnees.l);
	for(h = 0; h < nbFichiers; h++){ // On remplit le tableau de donnees
		for(i = 0; i < nbVecteursDonnees[h]; i++){ // On remplit le tableau de donnees
			svmDonnees.x[somme+i] = (struct svm_node*) malloc(sizeof(struct svm_node) * (dimensionVecteur+1) );
			for(j = 0; j < dimensionVecteur; j++){ // Valeur après valeur
				svmDonnees.x[somme+i][j].index = j+1;
				svmDonnees.x[somme+i][j].value = donnees[h][i]->data[j];
				//~ printf("(h, i, j) = (%d, %d, %d)  --->  %lf\t\t Label : %lf\n", h, i, j, donnees[h][i]->data[j], svmDonnees.y[somme+i]);
			} 
			svmDonnees.x[somme+i][dimensionVecteur].index = -1;
		}
		somme += nbVecteursDonnees[h];
	}
		
	
	retour = svm_check_parameter(&svmDonnees, &svmParametres);
	if(retour != NULL)
		printf("Erreur dans le parametrage de la SVM : %s\n", (char*) retour);
	
	 svmModel = svm_train(&svmDonnees, &svmParametres);
	
}

void traitementSVM_verifierDonnees(fvec_t* donnees[MAX_VECTORS_NUMBER], int nbVecteursDonnees, unsigned int dimensionVecteur, int* compteurClasses){
	int i = 0;
	int j = 0;
	int classe = 0;
	struct svm_node x[dimensionVecteur+1];
	
	
	
	for(i = 0; i < nbVecteursDonnees; i++){
		for(j = 0; j < dimensionVecteur; j++){ // Conversion données aubio --> données libSVM
			x[j].index = j+1;
			x[j].value = donnees[i]->data[j];
		}
		x[dimensionVecteur].index = -1;
		
		classe = svm_predict(svmModel, x);
		//~ printf("Classe d'appartenance : %d\n", classe);
		compteurClasses[classe]++;
	}

	
}

void traitementSVM_close(){
	
	
	
}

