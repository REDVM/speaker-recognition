#include "affichage.h"


void affichage_init(){
	char *error=NULL;
	Ihandle *dlg=NULL;
	IupOpen(NULL, NULL);

	if((error = IupLoad("affichage.led")) != NULL){
		IupMessage("Erreur %s\n", error);
		return;
	}

	dlg = IupGetHandle("fenetrePrincipale");


	IupShowXY(dlg,IUP_CENTER,IUP_CENTER);
	
	
	// Affectation des callbacks (signaux vers fonctions)
	IupSetCallback(IupGetHandle("boutonAjouterSonApprentissage"), "ACTION", (Icallback) affichageCB_boutonAjouterSonApprentissage_click);
	IupSetCallback(IupGetHandle("boutonSupprimerSonApprentissage"), "ACTION", (Icallback) affichageCB_boutonSupprimerSonApprentissage_click);
	IupSetCallback(IupGetHandle("boutonLancerApprentissage"), "ACTION", (Icallback) affichageCB_boutonLancerApprentissage_click);
	IupSetCallback(IupGetHandle("boutonAjouterVoixIdentifier"), "ACTION", (Icallback) affichageCB_boutonAjouterVoixIdentifier_click);
	IupSetCallback(IupGetHandle("itemQuitter"), "ACTION", (Icallback) affichageCB_itemQuitter_click);
	IupSetCallback(IupGetHandle("itemAbout"), "ACTION", (Icallback) affichageCB_itemAbout_click);

	IupMainLoop();

}

void affichage_close(){
	IupDestroy(IupGetHandle("dlg"));
	IupClose();
}


int affichageCB_boutonAjouterSonApprentissage_click(Ihandle* ih){
	Ihandle* maFenetre = IupGetHandle("fenetreSonApprentissage");
	Ihandle* listeApprentissage = IupGetHandle("listeApprentissage");
	int retour = 0;
	char* monFichier = NULL;
	
	if(IupGetInt(listeApprentissage, "COUNT") >= MAX_LOCUTEURS_NUMBER){
		IupMessage("Erreur", "Nombre de fichiers maximum atteint.");
		return IUP_DEFAULT;
	}
	
	retour = IupPopup(maFenetre, IUP_CENTER, IUP_CENTER);
	monFichier = IupGetAttribute(maFenetre, "VALUE");
	
	if(retour == IUP_NOERROR && IupGetInt(maFenetre, "STATUT") != -1 && monFichier != NULL){
		printf("Fichier choisi pour l'apprentissage : %s\n", monFichier);
		IupSetAttribute(listeApprentissage, "APPENDITEM", monFichier);
	}
	
	IupSetAttribute(IupGetHandle("frameResultat"), "ACTIVE", "NO");
	IupSetAttribute(IupGetHandle("frameIdentifier"), "ACTIVE", "NO");
	return IUP_DEFAULT;
}

int affichageCB_boutonSupprimerSonApprentissage_click(Ihandle* ih){
	Ihandle* listeApprentissage = IupGetHandle("listeApprentissage");
	IupSetAttribute(listeApprentissage, "REMOVEITEM", IupGetAttribute(listeApprentissage, "VALUE"));
	IupSetAttribute(IupGetHandle("frameResultat"), "ACTIVE", "NO");
	IupSetAttribute(IupGetHandle("frameIdentifier"), "ACTIVE", "NO");
	return IUP_DEFAULT;
}


int affichageCB_boutonLancerApprentissage_click(Ihandle* ih){
	Ihandle* listeApprentissage = IupGetHandle("listeApprentissage");
	int nombreElements = IupGetInt(listeApprentissage, "COUNT");
	char maChaine[2] = "1";
	char mesFichiers[MAX_LOCUTEURS_NUMBER][MAX_FILENAME_SIZE];
	int i = 0;
	
	if(nombreElements < 1){
		IupMessage("Attention", "Veuillez ajouter des fichiers dans la base d'apprentissage.");
		return IUP_DEFAULT;
	}
	
	for(i = 0; i < nombreElements; i++){
		maChaine[0] = '1' + i;
		strcpy(mesFichiers[i], IupGetAttribute(listeApprentissage, maChaine));
	}
	
	traitementSon_extraireMFCC(mesFichiers, nombreElements, APPRENTISSAGE);
	traitementSon_fourniDonneesSVM();	
	
	IupSetAttribute(IupGetHandle("frameIdentifier"), "ACTIVE", "YES");
	IupSetAttribute(IupGetHandle("frameApprentissage"), "ACTIVE", "NO");
	return IUP_DEFAULT;
}




int affichageCB_boutonAjouterVoixIdentifier_click(Ihandle* ih){
	Ihandle* maFenetre = IupGetHandle("fenetreSonApprentissage");
	Ihandle* listeApprentissage = IupGetHandle("listeApprentissage");
	Ihandle* textResultat = IupGetHandle("textResultat");
	int nombreElements = IupGetInt(listeApprentissage, "COUNT");
	int retour = 0;
	char monIndice[2] = "1";
	char ligneResultat[MAX_FILENAME_SIZE*2] = "";
	char* monFichier = NULL;
	char mesFichiers[1][MAX_FILENAME_SIZE];
	int compteurClasses[MAX_LOCUTEURS_NUMBER+1] = {0}; // +1 parce que l'indiceage ira de 1 a MAX_LOCUTEURS_NUMBER
	memset(compteurClasses, 0, MAX_LOCUTEURS_NUMBER);
	int nbVecteursDonnees = 0;
	int i = 0;
	
	retour = IupPopup(maFenetre, IUP_CENTER, IUP_CENTER);
	monFichier = IupGetAttribute(maFenetre, "VALUE");
	
	if(retour == IUP_NOERROR && IupGetInt(maFenetre, "STATUT") != -1 && monFichier != NULL){
		printf("Fichier choisi pour l'identification : %s\n", monFichier);
		IupSetAttribute(IupGetHandle("textVoixIdentifier"), "VALUE", monFichier);
		
		strcpy(mesFichiers[0], monFichier);
		traitementSon_extraireMFCC(mesFichiers, 1, IDENTIFICATION);
		nbVecteursDonnees = traitementSon_verifierDonneesSVM(compteurClasses);
		
		
		IupSetAttribute(textResultat, "CARET", "1,1");
		IupSetAttribute(textResultat, "INSERT", "\n---------------------------------------------\n");
		IupSetAttribute(textResultat, "CARET", "1,1");
		sprintf(ligneResultat, "Fichier à identifier : %s\n", IupGetAttribute(IupGetHandle("textVoixIdentifier"), "VALUE"));
		IupSetAttribute(textResultat, "INSERT", ligneResultat);
		for(i = 0; i < nombreElements; i++){
			monIndice[0] = '1' + i;
			sprintf(ligneResultat, "%d. %s\t\t%f%%\n", i+1, IupGetAttribute(listeApprentissage, monIndice),
				compteurClasses[i+1]*100.0/nbVecteursDonnees);
			IupSetAttribute(textResultat, "INSERT", ligneResultat);
			//~ printf("Occurence classe %d : %d\n", i+1, compteurClasses[i+1]);
		}
		
	}
	
	
	IupSetAttribute(IupGetHandle("frameResultat"), "ACTIVE", "YES");
	IupSetAttribute(IupGetHandle("frameApprentissage"), "ACTIVE", "YES");
	return IUP_DEFAULT;
}


int affichageCB_itemQuitter_click(Ihandle* ih){
	IupClose();
	return IUP_DEFAULT;
}

int affichageCB_itemAbout_click(Ihandle* ih){
	IupMessagef("À propos", "Description : ce logiciel extrait des MFCCs de plusieurs fichiers audios pour entrainer une base d'apprentissage SVM. Ensuite, un fichier audio à identifier est traité, on extrait aussi ses MFCCs et il est identifié par rapport à la base d'apprentissage.\nAuteur : Victor MARTIN <victormartinfr@gmail.com>\nVersion : %d.%d\nLicence : CeCILL v2.1", VERSION, SUBVERSION);
	return IUP_DEFAULT;
}
