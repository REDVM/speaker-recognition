#include "traitementSon.h"

fvec_t* mfccExtraits[NB_FICHIERS_BASE_APPR_MAX][MAX_VECTORS_NUMBER];
int nbVecteurs[NB_FICHIERS_BASE_APPR_MAX];
unsigned int nbFichiersBaseAppr = 0; 


void traitementSon_init(){
	int i =0;
	int j = 0;
	for(i = 0; i < NB_FICHIERS_BASE_APPR_MAX; i++){
		for(j = 0; j < MAX_VECTORS_NUMBER; j++){
			mfccExtraits[i][j] = new_fvec(MFCC_NUMBER);
		}
		nbVecteurs[i] = 0;
	}
	
	nbFichiersBaseAppr = 0;
}


void traitementSon_extraireMFCC(char filename_input[MAX_LOCUTEURS_NUMBER][MAX_FILENAME_SIZE], unsigned int nbFichiers, bool isSonVerification){
	
	assert(nbFichiers <= MAX_LOCUTEURS_NUMBER);
	assert(nbFichiers > 0);
	assert(!isSonVerification || (isSonVerification && nbFichiers == 1));
	
	unsigned int i = 0;			 
	unsigned int j = 0;			 
	unsigned int numFichierCourant = 0;			 
	smpl_t samplerate = 0;		// samplerate
	
	uint_t read = 0;
	int pos = 0; /*frames%dspblocksize*/
	int frames = 0; 
	
	aubio_mfcc_t * mfcc				= NULL;
	aubio_pvoc_t *pv				= NULL;
	cvec_t *fftgrain				= NULL;

	aubio_source_t*	source_input	= NULL;
	fvec_t* ibuf					= NULL;
	
	

	for(numFichierCourant = 0; numFichierCourant < nbFichiers; numFichierCourant++){ 
		frames	= 0;
		pos		= 0;
		read	= 0;
		
		printf ("Ouverture de %s en cours....\n", filename_input[numFichierCourant]);
		source_input = new_aubio_source ((char_t*)filename_input[numFichierCourant], 0, OVERLAP_SIZE);
		if (source_input == NULL) {
			printf ("Impossible d'ouvrir le fichier %s.\n", filename_input[numFichierCourant]);
			exit (EXIT_FAILURE);
		}
		
		
		ibuf = new_fvec (OVERLAP_SIZE);
		samplerate = aubio_source_get_samplerate(source_input);
		pv = new_aubio_pvoc (BUFFER_SIZE, OVERLAP_SIZE);
		fftgrain = new_cvec (BUFFER_SIZE);
		//populating the filter
		mfcc = new_aubio_mfcc(BUFFER_SIZE, FILTERS_NUMBER, MFCC_NUMBER, samplerate);
		
		// FIN DE L'INITIALISATION
		
		
		// Si c'est le son a analyser non pas pour la base de données mais
		// pour la vérification, on le place en fin de tableau
		if(isSonVerification){
			numFichierCourant = MAX_LOCUTEURS_NUMBER;
			nbFichiers = 0; // Pour arreter la boucle
		} else {
			nbFichiersBaseAppr = nbFichiers;
		}
		
		
		
		do {
			aubio_source_do (source_input, ibuf, &read);	
			for (j = 0; j < (unsigned)OVERLAP_SIZE; j++) {
				/*time for fft*/
				if (pos == OVERLAP_SIZE-1) {				 
					/* block loop */
					
					//compute mag spectrum
					aubio_pvoc_do (pv, ibuf, fftgrain);
				 
					//compute mfccs
					aubio_mfcc_do(mfcc, fftgrain, mfccExtraits[numFichierCourant][frames]);
					
					/* end of block loop */
					pos = -1; /* so it will be zero next j loop */
				}
				pos++;
			}
		
		
			//~ printf("%f\t", frames*OVERLAP_SIZE/(float)samplerate);
			//~ fvec_print(mfccExtraits[numFichierCourant][frames]);
			frames++;
		} while (read == OVERLAP_SIZE && frames < MAX_VECTORS_NUMBER); 
		// Tant qu'on n'est pas à la fin du fichier et qu'on a pas lu MAX_VECTORS_NUMBER frames (MAX_VECTORS_NUMBER = 512)
		
		nbVecteurs[numFichierCourant] = frames;
		
		printf ("Traitement effectué : %d frames de %d échantillons.\n", frames, BUFFER_SIZE);


		// Pour du debug, je vais enregistre dans un fichier
		//~ enregistrerTableauFormatLIBSVM(filename_input[numFichierCourant], mfccExtraits[numFichierCourant], frames);
		

		del_aubio_source (source_input);


		//destroying mfcc 
		del_aubio_pvoc (pv);
		del_cvec (fftgrain);
		del_aubio_mfcc(mfcc);
	}
	
	
	del_fvec (ibuf);
	aubio_cleanup();	
}

void traitementSon_fourniDonneesSVM(){
	traitementSVM_entrainerC_SVC(mfccExtraits, nbVecteurs, MFCC_NUMBER, nbFichiersBaseAppr);
}

int traitementSon_verifierDonneesSVM(int* compteurClasses){
	traitementSVM_verifierDonnees(mfccExtraits[MAX_LOCUTEURS_NUMBER], nbVecteurs[MAX_LOCUTEURS_NUMBER], MFCC_NUMBER, compteurClasses);
	return nbVecteurs[MAX_LOCUTEURS_NUMBER];
}



void traitementSon_close(){
	int i =0;
	int j = 0;
	for(i = 0; i < NB_FICHIERS_BASE_APPR_MAX; i++){
		for(j = 0; j < MAX_VECTORS_NUMBER; j++){
			del_fvec(mfccExtraits[i][j]);
		}
	}
	aubio_cleanup();
}
