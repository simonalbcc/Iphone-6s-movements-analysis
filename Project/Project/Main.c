#pragma region includes
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#pragma endregion 
#pragma region constantes
#define NB_DIR 15
#define TIME_EVALUATED 600
#define NB_SUBJECTS 24
#define SIZE_LINE 1000
#define TESTSET "testset.csv"
#define TRAINSET "trainset.csv"
#pragma endregion
#pragma region structures & enumerations
typedef struct someone Someone;
struct someone
{
	int code;
	int age;
	int weight;
	int height;
	int gender;
};

typedef struct movement Movement;
struct movement{
    int name;
    int gender;
    int index;
    float vAcc[TIME_EVALUATED];
};

typedef struct movementRead MovementRead;
struct movementRead {
	float attitudeRoll;
	float attitudePitch;
	float attitudeYaw;
	float gravityX;
	float gravityY;
	float gravityZ;
	float rotationRX;
	float rotationRY;
	float rotationRZ;
	float userAccX;
	float userAccY;
	float userAccZ;
};
#pragma endregion 


void gendersArray(int subjectsGender[]) {
	FILE* fi;
	Someone user;

	char line[SIZE_LINE];

	fopen_s(&fi, "data_subjects_info.csv", "r");

	if (fi == NULL) {
		printf("Erreur a l'ouverture du fichier !");
	}
	else {
		while (!feof(fi)) {
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
					printf("Erreur a la lecture des genres des sujets");
				}
			}
		}
		fclose(fi);
	}

}
void writeLineInFile(Movement movementToWrite, FILE* fi){
	fprintf(fi, "\n,%d,%d,%d", &movementToWrite.name,&movementToWrite.gender,&movementToWrite.index);
	int iVAcc = 0;
	while(movementToWrite.vAcc[iVAcc] != 0.0){
		fprintf(fi, "%f", &movementToWrite.vAcc[iVAcc]);
	}
}
void writeLine(Movement movementToWrite, int iTest, int iSub,FILE* fiTrain, FILE* fiTest) {
	if(iSub == iTest || iSub == iTest -1){
		writeLineInFile(movementToWrite, fiTest);
	}else{
		writeLineInFile(movementToWrite, fiTrain);
	}
}
void generationFile(Movement movementToWrite) {
	char paths[NB_DIR][7] = { "dw_1", "dw_2", "dw_11", "jog_9", "jog_16", "sit_5", "sit_13", "std_6",
								   "std_14", "ups_3", "ups_4", "ups_12", "wlk_7", "wlk_8", "wlk_15" };
	char pathFiSub[27];
	char pathFiMvt[33];

	int people[24];
	gendersArray(&people);

	FILE* fi;
	char line[SIZE_LINE];
	MovementRead movementRead;

	int iMovement = 0;
	int iSub = 0;
	int iMvt = 0;
	int iTest = 1;
	int iIndex = 0;
	int iDir = 0;

	// première boucle sur l'ensemble des dossiers 
	while (iDir < 15) {
		pathFiSub[27] = "A_DeviceMotionData/";
		strcat_s(pathFiSub, sizeof(pathFiSub), paths[iDir]);
		// vérifier si on est toujours dans le même mouvement 
		if (iDir < 14 && strcmp(paths[iDir][1], paths[iDir + 1][1]) != 0) {
			iMvt++;
		}
		movementToWrite.name = iMvt;
		// première boucle sur l'ensemble des fichiers d'un dossier  
		while (iSub < NB_SUBJECTS) {
			movementToWrite.gender = people[iSub];
			movementToWrite.index = iIndex;
			// crée le chemin pour le fichier à utiliser 
			pathFiMvt[33] = strcat_s(pathFiSub, sizeof(pathFiSub), "/sub_");
			pathFiMvt[33] = strcat_s(pathFiMvt, sizeof(pathFiMvt), iSub + 1);

			fopen_s(&fi, pathFiMvt, "r");
			if (fi == NULL) {
				printf("Erreur a l'ouverture du fichier !");
			}
			else {
				while (!feof(fi)) {
					//lire ligne d'entête
					fgets(line, SIZE_LINE, fi);
					//lecture du genre de chaque ligne
					movementRead = fgets(line, SIZE_LINE, fi);             // à refaire 
					while (&movementRead != NULL && iMovement < 600) {
						if (sscanf_s(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &movementRead.attitudeRoll, &movementRead.attitudePitch, &movementRead.attitudeYaw, &movementRead.gravityX, &movementRead.gravityY, &movementRead.gravityZ, &movementRead.rotationRX, &movementRead.rotationRY, &movementRead.rotationRZ, &movementRead.userAccX, &movementRead.userAccY, &movementRead.userAccZ == 12)) {
							movementToWrite.vAcc[iMovement] = sqrt((pow(movementRead.userAccX, 2) + pow(movementRead.userAccY, 2) + pow(movementRead.userAccZ, 2)));
							iMovement++;
							movementRead = fgets(line, SIZE_LINE, fi);
						}
					}
					// cas où le nombre de mouvements dispo est < que 600
					while (iMovement < 600) {
						movementToWrite.vAcc[iMovement] = 0;

					}
					// on ouvre où les deux fichiers??? #la galère 
					writeLine(movementToWrite, iTest, iSub, ); 
						iSub++;
					iIndex++;
				}

				if (iTest == 23) {
					iTest = 1;
				}
				else {
					iTest += 2;
				}

				iDir++;
			}
		}
	}
}



void main(void) {
  





}






























