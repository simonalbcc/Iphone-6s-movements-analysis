#include "Biblio.h"

// phase 2 
void wellClassified(int realClasses[], int estimatedClasses[], Classe classes[], int nbTests) {
	int iClasse = 0; 
	while (iClasse < NB_CLASSE_MAX) {
		classes[iClasse].nbTested = 0; 
		classes[iClasse].nbWellClassified = 0;
		for (int iRemplacement = 0; iRemplacement < NB_CLASSE_MAX; iRemplacement++) {
			classes[iClasse].remplacements[iRemplacement] = 0; 
		}
		iClasse++; 
	}

	for (int iTest = 0; iTest < nbTests; iTest++) {
		if (realClasses[iTest] == estimatedClasses[iTest]) {
			classes[realClasses[iTest] - 1].nbWellClassified++; 
		}
		classes[realClasses[iTest] - 1].nbTested++;
		classes[realClasses[iTest] - 1].remplacements[estimatedClasses[iTest] - 1];
	}
}

double averagePercents(Classe classes[]) {
	int nbWellClassifiedTotal = 0; 
	int nbTestedTotal = 0;
	int iClasse = 0; 
	while (iClasse != NB_CLASSE_MAX && classes[iClasse].nbTested != 0) {
		nbWellClassifiedTotal += classes[iClasse].nbWellClassified; 
		nbTestedTotal += classes[iClasse].nbTested;
		iClasse++; 
	}

	return (double)nbWellClassifiedTotal / nbTestedTotal * 100; 
}
void displayResultsByClass(int realClasses[], int estimatedClasses[], int nbTests) {
	Classe classes[NB_CLASSE_MAX]; // taille provisoire 

	wellClassified(realClasses, estimatedClasses, classes, nbTests);

	printf("Numero classe\t Bien classes\t      Total\t   Pourcentage\n");
	printf("_________________________________________________________________\n");
	for (int iClasse = 0; iClasse < NB_CLASSE_MAX; iClasse++) {
		if (classes[iClasse].nbTested != 0) {
			printf("\t%d\t|\t", iClasse + 1);
			printf("%d\t|\t", classes[iClasse].nbWellClassified);
			printf("  %d\t|  ", classes[iClasse].nbTested);
			printf("\t%.2f\t|\t", (classes[iClasse].nbWellClassified / (double)classes[iClasse].nbTested) * 100);
			printf("\n");
		}
	}

}
void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests) {
	Classe classes[NB_CLASSE_MAX]; // taille provisoire 
	wellClassified(realClasses, estimatedClasses, classes, nbTests);
	printf("The accuracy is %.2f%%, it means that %.2f%% of classes are well classified.", averagePercents(classes), averagePercents(classes));
}
void displayConfusionMatrix() {

}
