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

void modelCreation2(){
	
	
	FILE* fiTrain;
	FILE* fiMode;

	MovementType movementType[NB_TYPE];
	Movement move;
	
	int err = fopen_s(&fiTrain, TRAINSET, "r");
	if(err != 0){
		exit(100);
	}
	err = fopen_s(&fiTrain, MODEL, "w");
	if(err != 0){
		return;
	}

	char line[SIZE_LINE];
	move.name = 0; 
	int iMov = 0; 

	fgets(line, SIZE_LINE, fiTrain);

	char* token;
	char* nextToken = NULL;

	while(!feof(fiTrain) && iMov == move.name){
		initMovementTypeArray(movementType);


		token = strtok_s(line, ",", &nextToken);
		token = strtok_s(NULL, ",", &nextToken); // Mouvment
		move.name = atoi(token); 
		token = strtok_s(NULL, ",", &nextToken); // Gender

		int numMotion = move.name;
					puts(line); 
		while (!feof(fiTrain) && move.name == numMotion) {
			int iColum = 0;
			puts(line); 
			//Vacc
			token = strtok_s(NULL, ",", &nextToken);
			while (token != NULL && iColum < TIME_EVALUATED && atof(token) != 0) {
				printf("mov : %d valeur %lf\n", numMotion, atof(token)); 
				movementType[numMotion].averagePerTenthSecond[iColum] += atof(token);
				movementType[numMotion].standardDeviation[iColum] += atof(token) * atof(token);
				movementType[numMotion].nbVaccNotNull[iColum]++;
				token = strtok_s(NULL, ",", &nextToken);
				iColum++;
			}

			fgets(line, SIZE_LINE, fiTrain);
			if (!feof(fiTrain)) {
				token = strtok_s(line, ",", &nextToken); // Index
				token = strtok_s(NULL, ",", &nextToken); // Mouvment
				move.name = atoi(token);
				token = strtok_s(NULL, ",", &nextToken); // Gender
			}
		}
	}
	int iTenthSecond = 0; 
	while (iTenthSecond < TIME_EVALUATED && movementType[0].averagePerTenthSecond[iTenthSecond] != 0) {
		movementType[0].generalAverage += movementType[0].averagePerTenthSecond[iTenthSecond];
		movementType[0].averagePerTenthSecond[iTenthSecond] /= movementType[0].nbVaccNotNull[iTenthSecond];
		movementType[0].standardDeviation[iTenthSecond] = (movementType[0].standardDeviation[iTenthSecond] / movementType[0].nbVaccNotNull[iTenthSecond]) - sqrt(movementType[0].averagePerTenthSecond[iTenthSecond]);
		movementType[0].nbTotalNotNull += movementType[0].nbVaccNotNull[iTenthSecond];
		iTenthSecond++;
	}
	writeAllMovementTypeInFile(movementType); 
}

void modelCreation() {
	MovementType movementType[NB_TYPE];
	Movement currentMov;
	FILE* fiTrain; 
	int iMov = 0; 
	int iTenthSecond = 0; 
	char line[SIZE_LINE];

	initMovementTypeArray(movementType);
	
	fopen_s(&fiTrain, TRAINSET, "r");
	if (fiTrain == NULL) {
		printf("Probleme a l'ouverture de trainset"); 
	} else {
		fgets(line, SIZE_LINE, fiTrain); // get headline
		fgets(line, SIZE_LINE, fiTrain); // get first line 
		while (!feof(fiTrain) && iMov < NB_TYPE) {
			currentMov.name = decomposition(line, currentMov.vAcc);
			while (iMov == currentMov.name-1) {
				while (iTenthSecond < TIME_EVALUATED && currentMov.vAcc[iTenthSecond] != 0) {
					movementType[iMov].averagePerTenthSecond[iTenthSecond] += currentMov.vAcc[iTenthSecond];
					movementType[iMov].standardDeviation[iTenthSecond] += sqrt((double)currentMov.vAcc[iTenthSecond]);
					movementType[iMov].nbVaccNotNull[iTenthSecond] ++;
					iTenthSecond++;
				}
				fgets(line, SIZE_LINE, fiTrain);
				if (line != NULL) {
					currentMov.name = decomposition(line, currentMov.vAcc);
				}
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


}

