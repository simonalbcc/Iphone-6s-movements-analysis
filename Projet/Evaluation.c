#include "ClassificationPerformances.h"


int highestClass(int realClasses[], int nbTests) {
	int highest = 0; 
	for (int iRealClass = 0; iRealClass < nbTests; iRealClass++) {
		if (highest < realClasses[iRealClass]) {
			highest = realClasses[iRealClass]; 
		}
	}
	return highest; 
}

void wellClassified(int realClasses[], int estimatedClasses[], Class classes[], int nbTests) {
	int iClass = 0; 
	while (iClass < NB_CLASSES_MAX) {
		classes[iClass].nbTested = 0; 
		classes[iClass].nbWellClassified = 0;
		for (int iRemplacement = 0; iRemplacement < NB_CLASSES_MAX; iRemplacement++) {
			classes[iClass].remplacements[iRemplacement] = 0; 
		}
		iClass++; 
	}

	for (int iTest = 0; iTest < nbTests; iTest++) {
		if (realClasses[iTest] == estimatedClasses[iTest]) {
			classes[realClasses[iTest] - 1].nbWellClassified++; 
		}
		classes[realClasses[iTest] - 1].nbTested++;
		classes[realClasses[iTest] - 1].remplacements[estimatedClasses[iTest] - 1]++;
	}

}
double globalAccuracy(Class classes[]) {
	int nbWellClassifiedTotal = 0; 
	int nbTestedTotal = 0;
	int iClass = 0; 
	while (iClass != NB_CLASSES_MAX && classes[iClass].nbTested != 0) {
		nbWellClassifiedTotal += classes[iClass].nbWellClassified; 
		nbTestedTotal += classes[iClass].nbTested;
		iClass++; 
	}

	return (double)nbWellClassifiedTotal / nbTestedTotal * 100; 
}
void accuracy(Class classes[]) {
	for (int iClass = 0; iClass < NB_CLASSES_MAX; iClass++) {
		classes[iClass].accuracy = classes[iClass].nbWellClassified / (double)classes[iClass].nbTested * 100;
	}
}
void displayResultsByClass(int realClasses[], int estimatedClasses[], int nbTests) {
	Class classes[NB_CLASSES_MAX]; // taille provisoire 

	wellClassified(realClasses, estimatedClasses, classes, nbTests);
	accuracy(classes); 
	int nbClassMax = highestClass(realClasses, nbTests);

	printf("Numero Class\t Bien classes\t      Total\t   Pourcentage\n");
	printf("_________________________________________________________________\n");
	for (int iClass = 0; iClass < nbClassMax; iClass++) {
		if (classes[iClass].nbTested != 0) {
			printf("\t%d\t|\t", iClass + 1);
			printf("%d\t|\t", classes[iClass].nbWellClassified);
			printf("  %d\t|  ", classes[iClass].nbTested);
			printf("\t%.2f\t|\t", classes[iClass].accuracy);
			printf("\n");
		}
	}

}
void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests) {
	Class classes[NB_CLASSES_MAX];
	wellClassified(realClasses, estimatedClasses, classes, nbTests);
	printf("The accuracy is %.2f%%, it means that %.2f%% of classes are well classified.\n", globalAccuracy(classes), globalAccuracy(classes));
}
void displayConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests) {
	int nbClassMax = highestClass(realClasses, nbTests);
	Class classes[NB_CLASSES_MAX];
	wellClassified(realClasses, estimatedClasses, classes, nbTests);

	for (int iTest = 0; iTest < nbClassMax; iTest++) {
		if (iTest == 0) {
			printf("/");
		} else {
			if (iTest == nbClassMax -1) {
				printf("\\");
			} else {
				printf("|");
			}
		}
		for (int iClassement = 0; iClassement < nbClassMax; iClassement++) {
			printf(" %d ", classes[iTest].remplacements[iClassement]);
		}

		if (iTest == 0) {
			printf("\\\n");
		} else {
			if (iTest == nbClassMax -1) {
				printf("/\n");
			} else {
				printf("|\n");
			}
		}
	}
}
