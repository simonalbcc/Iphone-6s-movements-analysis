#include "classificationPerformances.h" 

// phase 1 
void gendersArray(int subjectsGender[]) {
	FILE* fi;                                                        // donner un nom plus significatif
	Someone user;

	char line[SIZE_LINE];

	fopen_s(&fi, "data_subjects_info.csv", "r");

	if (fi == NULL) {
		printf("Erreur a l'ouverture du fichier !");
	}
	else {
		while (!feof(fi)) {							// je ne comprends pas à quoi sert cette boucle ? De plus, ça ne correspond pas à votre DA
			int iSubject = 0;
			//lire ligne d'entête
			fgets(line, SIZE_LINE, fi);
			//lecture du genre de chaque ligne
			while (fgets(line, SIZE_LINE, fi) != NULL) {
				if (sscanf_s(line, "%d,%d,%d,%d,%d", &user.code, &user.age, &user.weight, &user.height, &user.gender) == 5) {
					subjectsGender[iSubject] = user.gender;
					iSubject++;
				}
				else {
					printf("Erreur a la lecture des genres des sujets");   // et peut-être sortir directement de la boucle si erreur
				}
			}
		}
		fclose(fi);
	}

}
void writeLineInFile(Movement movementToWrite, FILE* fi) {
	fprintf(fi, "\n%d,%d,%d,", movementToWrite.name, movementToWrite.gender, movementToWrite.index + 1);
	int iVAcc = 0;
	while (movementToWrite.vAcc[iVAcc] != 0 && iVAcc < 600) {			// inverser les conditions
		fprintf(fi, "%f", movementToWrite.vAcc[iVAcc]);				// c'est un peu bizarre : vous avez rajouté des 0 pour arriver jusqu'à 600
		iVAcc++;								// et là vous ne les mettez pas dans le fichier 
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
void freeString(char string[], int length) {
	for (int iChar = 0; iChar < length; iChar++) {
		string[iChar] = '\0';
	}
}
void generationFile() {

	FILE* fiTest;
	FILE* fiTrain;
	fopen_s(&fiTest, TESTSET, "a+");				// pourquoi pas mode w?
	fopen_s(&fiTrain, TRAINSET, "a+");				// tests si problème?


	/*
		fprintf(fiTest, "%f", &movementToWrite.vAcc[iVAcc]);
		fprintf(fiTrain, "%f", &movementToWrite.vAcc[iVAcc]);
	*/


	char paths[NB_DIR][7] = { "dws_1", "dws_2", "dws_11", "jog_9", "jog_16", "sit_5", "sit_13", "std_6",
								   "std_14", "ups_3", "ups_4", "ups_12", "wlk_7", "wlk_8", "wlk_15" };
	char pathFiSub[40];						// grandeurs de tableaux en # define (partout)

	int people[24];
	gendersArray(people);

	FILE* fi;
	char line[SIZE_LINE];
	char number[10];

	MovementRead movementRead;
	Movement movementToWrite;

	int iMvt = 0;
	int iTest = 1;
	int iIndex = 0;
	int iDir = 0;

	// première boucle sur l'ensemble des dossiers 
	while (iDir < 15) {
		int iSub = 0;
		printf("Je suis dans le numero %d\n", iDir + 1);
		// vérifier si on est toujours dans le même mouvement 
		if (iDir < 13 && (paths[iDir][1] != paths[iDir + 1][1])) {		
			iMvt++;
		}
		movementToWrite.name = iMvt;
		// première boucle sur l'ensemble des fichiers d'un dossier  
		while (iSub < NB_SUBJECTS) {
			printf("\t- Je suis dans le sub %d\n", iSub + 1);
			movementToWrite.gender = people[iSub];
			movementToWrite.index = iIndex;
			// crée le chemin pour le fichier à utiliser 
			sprintf_s(pathFiSub, sizeof(pathFiSub), "A_DeviceMotion_data/%s/sub_%d.csv", paths[iDir], iSub + 1); ;

			fopen_s(&fi, pathFiSub, "r+");  					// pourquoi r+?
			if (fi == NULL) {
				printf("Erreur a l'ouverture du fichier ! ici");
			}
			else {
				//lire ligne d'entête
				fgets(line, SIZE_LINE, fi);
				int iMovement = 0;
				while (!feof(fi)) {
					//lecture du genre de chaque ligne
					fgets(line, SIZE_LINE, fi);			// les scanf pour des double c'est lf
					sscanf_s(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", &movementRead.attitudeRoll, &movementRead.attitudePitch, &movementRead.attitudeYaw, &movementRead.gravityX, &movementRead.gravityY, &movementRead.gravityZ, &movementRead.rotationRX, &movementRead.rotationRY, &movementRead.rotationRZ, &movementRead.userAccX, &movementRead.userAccY, &movementRead.userAccZ);
					while (&movementRead != NULL && iMovement < 600) {		// drôle de condition pour tester une fin de fichier !!
						if (sscanf_s(line, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", &movementRead.attitudeRoll, &movementRead.attitudePitch, &movementRead.attitudeYaw, &movementRead.gravityX, &movementRead.gravityY, &movementRead.gravityZ, &movementRead.rotationRX, &movementRead.rotationRY, &movementRead.rotationRZ, &movementRead.userAccX, &movementRead.userAccY, &movementRead.userAccZ) == 12) {
							movementToWrite.vAcc[iMovement] = sqrt((pow(movementRead.userAccX, 2) + pow(movementRead.userAccY, 2) + pow(movementRead.userAccZ, 2)));
							iMovement++;
						}				// mettre un else au cas où
						fgets(line, SIZE_LINE, fi);
					}
					// cas où le nombre de mouvements dispo est < que 600
					while (iMovement < 600) {
						movementToWrite.vAcc[iMovement] = 0;
						iMovement++;
					}
					iMovement = 0;			//inutile tu le refais après
				}
				writeLine(movementToWrite, iTest, iSub, fiTrain, fiTest);
				freeString(pathFiSub, sizeof(pathFiSub));       // est-ce utile??
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
		iDir++;
	}
	fclose(fiTrain);
	fclose(fiTest);
}
