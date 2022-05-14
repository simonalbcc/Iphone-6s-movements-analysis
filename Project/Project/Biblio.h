#pragma region includes
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Constantes.h" 
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
struct movement {
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

typedef struct class Class;
struct class {
	int nbWellClassified;
	int nbTested;
	double accuracy;
	int remplacements[NB_CLASSES_MAX];
};

#pragma endregion 


#pragma prototypes 

void gendersArray(int subjectsGender[]); 
void writeLineInFile(Movement movementToWrite, FILE* fi); 
void writeLine(Movement movementToWrite, int iTest, int iSub, FILE* fiTrain, FILE* fiTest); 
void freeString(char string[], int length); 
void generationFile(); 
void wellClassified(int realClasses[], int estimatedClasses[], Class classes[], int nbTests);
double averagePercents(Class classes[]);
void displayResultsByClass(int realClasses[], int estimatedClasses[], int nbTests); 
void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests); 
void displayConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests); 



#pragma endregion