#ifndef AFFICHAGE__H
#define AFFICHAGE__H




#include "includes.h"

#include <iup/iup.h>

void affichage_init();

void affichage_close();

int affichageCB_boutonAjouterSonApprentissage_click(Ihandle* ih);

int affichageCB_boutonSupprimerSonApprentissage_click(Ihandle* ih);

int affichageCB_boutonLancerApprentissage_click(Ihandle* ih);

int affichageCB_boutonAjouterVoixIdentifier_click(Ihandle* ih);

int affichageCB_itemQuitter_click(Ihandle* ih);

int affichageCB_itemAbout_click(Ihandle* ih);




#endif
