#include "ClassificationPerformances.h" 


void gendersArray(int subjectsGender[]) {
	FILE* fiGender;                                                        
	Someone user;
	bool correct = true;

	char line[SIZE_LINE];

	fopen_s(&fiGender, "data_subjects_info.csv", "r");

	if (fiGender == NULL) {
		printf("Erreur a l'ouverture du fichier !");
	}else {
		//lire ligne d'entête
		fgets(line, SIZE_LINE, fiGender);
		int iSubject = 0;
		//lecture du genre de chaque ligne
		fgets(line, SIZE_LINE, fiGender);
		while (!feof(fiGender) && correct) {									                                                                                                                                
			if (sscanf_s(line, "%d,%d,%d,%d,%d", &user.code, &user.age, &user.weight, &user.height, &user.gender) == 5) {
				subjectsGender[iSubject] = user.gender;
				iSubject++;
			}else {
				correct = false;
				printf("Erreur a la lecture des genres des sujets");  
			}
			fgets(line, SIZE_LINE, fiGender);
		}
		fclose(fiGender);
	}

}
void writeLineInFile(Movement movementToWrite, FILE* fi) {
	fprintf(fi, "\n%d,%d,%d", movementToWrite.name, movementToWrite.gender, movementToWrite.index + 1);
	int iVAcc = 0;
	while (iVAcc < TIME_EVALUATED) {									
		fprintf(fi, ",%f", movementToWrite.vAcc[iVAcc]);							
		iVAcc++;																	 
	}
}
void writeLine(Movement movementToWrite, int iTest, int iSub, FILE* fiTrain, FILE* fiTest) {
	if (iSub == iTest || iSub == iTest - 1) {
		writeLineInFile(movementToWrite, fiTest);
	}
	else {
		writeLineInFile(movementToWrite, fiTrain);
	}
}
void generationFile() {

	FILE* fiTest;
	FILE* fiTrain;
	fopen_s(&fiTest, TESTSET, "w");				 
	fopen_s(&fiTrain, TRAINSET, "w");			

	if (fiTest == NULL || fiTrain == NULL) {
		printf("Probleme lors de l'ouverture d'un des 2 fichiers");
	} else {
		char paths[NB_DIR][CHAR_MAX_SIZE] = { "dws_1", "dws_2", "dws_11", "jog_9", "jog_16", "sit_5", "sit_13", "std_6",
										"std_14", "ups_3", "ups_4", "ups_12", "wlk_7", "wlk_8", "wlk_15" };
		char pathFiSub[PATH_SIZE];						

		int people[NB_SUBJECTS];
		gendersArray(people);

		FILE* fiCurrent;
		char line[SIZE_LINE];

		MovementRead movementRead;
		Movement movementToWrite;

		int iMvt = 0;
		int iTest = 1;
		int iIndex = 0;
		int iDir = 0;

		// première boucle sur l'ensemble des dossiers 
		while (iDir < NB_DIR) {
			int iSub = 0;
			movementToWrite.name = iMvt+1;
			// première boucle sur l'ensemble des fichiers d'un dossier  
			while (iSub < NB_SUBJECTS) {
				movementToWrite.gender = people[iSub];
				movementToWrite.index = iIndex;
				// crée le chemin pour le fichier à utiliser 
				sprintf_s(pathFiSub, sizeof(pathFiSub), "A_DeviceMotion_data/%s/sub_%d.csv", paths[iDir], iSub + 1); ;

				fopen_s(&fiCurrent, pathFiSub, "r");  				
				if (fiCurrent == NULL) {
					printf("Erreur a l'ouverture du fichier !");
				}
				else {
					//lire ligne d'entête
					fgets(line, SIZE_LINE, fiCurrent);
					int iMovement = 0;
					while (!feof(fiCurrent)) {
						//lecture du genre de chaque ligne
						fgets(line, SIZE_LINE, fiCurrent);			
						sscanf_s(line, "%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &movementRead.index, &movementRead.attitudeRoll, &movementRead.attitudePitch, &movementRead.attitudeYaw, &movementRead.gravityX, &movementRead.gravityY, &movementRead.gravityZ, &movementRead.rotationRX, &movementRead.rotationRY, &movementRead.rotationRZ, &movementRead.userAccX, &movementRead.userAccY, &movementRead.userAccZ);
						while (!feof(fiCurrent) && iMovement < TIME_EVALUATED) {	
							if (sscanf_s(line, "%d,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &movementRead.index, &movementRead.attitudeRoll, &movementRead.attitudePitch, &movementRead.attitudeYaw, &movementRead.gravityX, &movementRead.gravityY, &movementRead.gravityZ, &movementRead.rotationRX, &movementRead.rotationRY, &movementRead.rotationRZ, &movementRead.userAccX, &movementRead.userAccY, &movementRead.userAccZ) == 13) {
								movementToWrite.vAcc[iMovement] = sqrt((pow(movementRead.userAccX, 2) + pow(movementRead.userAccY, 2) + pow(movementRead.userAccZ, 2)));
								iMovement++;
							} else {			
								printf("Erreur, le document ne contient pas toutes les infos demandées");
								iMovement += TIME_EVALUATED - iMovement; 
							}
							fgets(line, SIZE_LINE, fiCurrent);
						}
						// cas où le nombre de mouvements dispo est < que 600
						while (iMovement < TIME_EVALUATED) {
							movementToWrite.vAcc[iMovement] = 0;
							iMovement++;
						}
					}
					writeLine(movementToWrite, iTest, iSub, fiTrain, fiTest);
					iSub++;
					iIndex++;
				}
			}
			if (iTest == 23) {
				iTest = 1;
			}
			else {
				iTest += 2;
			}
			// vérifier si on sera toujours dans le même mouvement 
			if (iDir < 13 && (paths[iDir][1] != paths[iDir + 1][1])) {
				iMvt++;
			}
			iDir++;
		}
		fclose(fiTrain);
		fclose(fiTest);
	}
}
