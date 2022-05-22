#include "ClassificationPerformances.h"

void initMovementTypeArray (MovementType movementType[]) {
	for (int iType = 0; iType < NB_TYPE; iType++) {
		movementType[iType].generalAverage = 0; 
		movementType[iType].nbTotalNotNull = 0;
		for (int iMove = 0; iMove < TIME_EVALUATED; iMove++) {
			movementType[iType].averagePerTenthSecond[iMove] = 0;
			movementType[iType].standardDeviation[iMove] = 0;
			movementType[iType].nbVaccNotNull[iMove] = 0;

		}
	}
}

void modelCreation() {
	MovementType movementType[NB_TYPE];
	Movement currentMov;
	FILE* fiTrain; 
	int iMov = 0; 
	int iTenthSecond; 
	char line[SIZE_LINE];

	fopen_s(&fiTrain, TRAINSET, "r");

	if (fiTrain == NULL) {
		printf("Probleme a l'ouverture de trainset"); 
	} else {
		initMovementTypeArray(movementType);
		fgets(line, SIZE_LINE, fiTrain); // get headline
		fgets(line, SIZE_LINE, fiTrain); // get first line 
		while (!feof(fiTrain)) {
			currentMov.name = decomposition(line, currentMov.vAcc);

			while (!feof(fiTrain) && iMov == currentMov.name - 1) {
				iTenthSecond = 0;
				while (iTenthSecond < TIME_EVALUATED && currentMov.vAcc[iTenthSecond] != 0) {
					movementType[iMov].averagePerTenthSecond[iTenthSecond] += currentMov.vAcc[iTenthSecond];
					movementType[iMov].standardDeviation[iTenthSecond] += pow(currentMov.vAcc[iTenthSecond],2);
					movementType[iMov].nbVaccNotNull[iTenthSecond] ++;
					iTenthSecond++;
				}
				fgets(line, SIZE_LINE, fiTrain);
				if (!feof(fiTrain)) {
					currentMov.name = decomposition(line, currentMov.vAcc);
				}
			}
			iTenthSecond = 0;
			while (iTenthSecond < TIME_EVALUATED) {
				movementType[iMov].generalAverage += movementType[iMov].averagePerTenthSecond[iTenthSecond];
				movementType[iMov].averagePerTenthSecond[iTenthSecond] /= movementType[iMov].nbVaccNotNull[iTenthSecond];
				movementType[iMov].nbTotalNotNull += movementType[iMov].nbVaccNotNull[iTenthSecond];
				movementType[iMov].standardDeviation[iTenthSecond] = sqrt((movementType[iMov].standardDeviation[iTenthSecond] / movementType[iMov].nbVaccNotNull[iTenthSecond]) - pow(movementType[iMov].averagePerTenthSecond[iTenthSecond],2));
				iTenthSecond++;
			}
			movementType[iMov].generalAverage /= movementType[iMov].nbTotalNotNull;
			iMov++;
		}
		fclose(fiTrain);
		
		writeAllMovementTypeInFile(movementType);
	}


}

