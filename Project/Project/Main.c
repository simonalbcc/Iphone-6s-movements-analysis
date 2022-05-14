#include "Biblio.h" 

void main(void) {

	//generationFile();
	int realClasses[8] = { 5, 2, 5, 3, 1, 3, 2, 4 };
	int estimateClasses[8] = { 5, 5, 1, 2, 1, 3, 2, 4 };
	displayResultsByClass(realClasses, estimateClasses, 8); 
	displayAccuracy(realClasses, estimateClasses, 8);
	displayConfusionMatrix(realClasses, estimateClasses, 8);
<<<<<<< HEAD
	
=======
>>>>>>> parent of 2b71b0d (Phase 3)
	
}






























