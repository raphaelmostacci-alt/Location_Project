repertoire: repertoire.o fonctions_principales.o fonctions_utiles.o
	gcc repertoire.o fonctions_principales.o fonctions_utiles.o -o repertoire

repertoire.o: repertoire.c prototypes_rep.h prototypes_utiles.h librairies_rep.h prototypes_principales.h
	gcc -c repertoire.c -Wall

fonctions_principales.o: fonctions_principales.c librairies_rep.h prototypes_principales.h
	gcc -c fonctions_principales.c -Wall

fonctions_utiles.o: fonctions_utiles.c librairies_rep.h prototypes_utiles.h
	gcc -c fonctions_utiles.c -Wall