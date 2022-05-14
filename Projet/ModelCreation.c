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
	int iMov = 1; 
	int iTenthSecond; 
	char line[SIZE_LINE];

	initMovementTypeArray(movementType);
	
	fopen_s(&fiTrain, TRAINSET, "r");

	while (!feof(fiTrain) && iMov < 6) {
		fgets(line, SIZE_LINE, fiTrain);

		fgets(line, SIZE_LINE, fiTrain);
		decomposition(line, currentMov.vAcc); 
		sscanf_s(line, "%d, %d, %d", &currentMov.name, &currentMov.gender, &currentMov.index);

		while (iMov == currentMov.name) {
			iTenthSecond = 0; 
			while (iTenthSecond < TIME_EVALUATED && currentMov.vAcc[iTenthSecond] != 0) {
				movementType[iMov].averagePerTenthSecond[iTenthSecond] += currentMov.vAcc[iTenthSecond];
				movementType[iMov].standardDeviation[iTenthSecond] += sqrt((double)currentMov.vAcc[iTenthSecond]);
				movementType[iMov].nbVaccNotNull[iTenthSecond] ++;
				iTenthSecond++; 
			}
			fgets(line, SIZE_LINE, fiTrain);
			decomposition(line, currentMov.vAcc);
			sscanf_s(line, "%d, %d, %d", &currentMov.name, &currentMov.gender, &currentMov.index);
		}
		while (iTenthSecond > 0 && movementType[iMov].averagePerTenthSecond[iTenthSecond] != 0) {
			movementType[iMov].generalAverage += movementType[iMov].averagePerTenthSecond[iTenthSecond]; 
			movementType[iMov].averagePerTenthSecond[iTenthSecond] /= movementType[iMov].nbVaccNotNull[iTenthSecond]; 
			movementType[iMov].standardDeviation[iTenthSecond] = (movementType[iMov].standardDeviation[iTenthSecond] / movementType[iMov].nbVaccNotNull[iTenthSecond]) - sqrt(movementType[iMov].averagePerTenthSecond[iTenthSecond]); 
			movementType[iMov].nbTotalNotNull += movementType[iMov].nbVaccNotNull[iTenthSecond]; 
			iTenthSecond--; 
		}
		movementType[iMov].generalAverage /= movementType[iMov].nbTotalNotNull; 
		iMov++; 
	}
	writeAllMovementTypeInFile(movementType);


}

