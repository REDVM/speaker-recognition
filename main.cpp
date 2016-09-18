#include "main.h"


		
int main (int argc, char** argv)
{
	int i = 0;
	int j = 0;
	
	
	traitementSon_init();
	traitementSVM_init();	
	affichage_init(); 			// L'affichage se fait en dernier car il est bloquant
	
	
	
	
	
	affichage_close();
	traitementSon_close();
	traitementSVM_close();
	
	
	return EXIT_SUCCESS;
}







void debug(char* string){
	#ifdef DEBUG__
		printf("%s\n", string);
	#endif	
}

