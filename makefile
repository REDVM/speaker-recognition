# Plutôt qu'ajouter svm.cpp et svm.h a mon projet et le recompiler a chaque fois, j'utilise svm.h
# dans le dossier /usr/include/libsvm et avec -lsvm j'utilise la bibliothèque déja compilée dans /usr/lib (libsvm.so.....)
CFLAGS = -w -g -std=c99 -pedantic -I/usr/local/include/aubio -I/usr/include/aubio 

OPTIMISATION = -O3 -fomit-frame-pointer 
EXE = sr 

LINKERS = -laubio -lsvm -liup

MAIN = main.o helpers.o
AFFICHAGE = affichage.o
TRAITEMENT = traitementSon.o traitementSVM.o
sr: $(MAIN) $(AFFICHAGE) $(TRAITEMENT) 
	gcc $(OPTIMISATION) $^ $(CFLAGS) $(LINKERS) -o $(EXE) 

%.o : %.cpp
	gcc $(OPTIMISATION) $(CFLAGS) $(LINKERS) -c $<

	
clean:
	rm -f *.o
	
clean_all:
	rm -f *.o $(EXE)
	
exec:
	#make clean
	make
	./sr
	
play:
	make
	./sr




#-D__LITTLE_ENDIAN__
