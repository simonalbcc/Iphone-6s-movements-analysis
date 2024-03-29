
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "Constantes.h" 

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
	double vAcc[TIME_EVALUATED];
};

typedef struct movementRead MovementRead;
struct movementRead {
	int index; 
	double attitudeRoll;
	double attitudePitch;
	double attitudeYaw;
	double gravityX;
	double gravityY;
	double gravityZ;
	double rotationRX;
	double rotationRY;
	double rotationRZ;
	double userAccX;
	double userAccY;
	double userAccZ;
};

typedef struct class Class;
struct class {
	int nbWellClassified;
	int nbTested;
	double accuracy;
	int remplacements[NB_CLASSES_MAX];
};


typedef struct movementType MovementType;
struct movementType {
	double averagePerTenthSecond[TIME_EVALUATED];
	double standardDeviation[TIME_EVALUATED];
	int nbVaccNotNull[TIME_EVALUATED];
	double generalAverage;
	int nbTotalNotNull;
};

typedef struct model Model;
struct model {
	int type;
	double averages[TIME_EVALUATED];
	double stds[TIME_EVALUATED];
	double globalAvg;
};

typedef struct indicators Indicators;
struct indicators {
	int indicator1;
	int indicator2;
	int indicator3;
};

// phase 1 
void gendersArray(int subjectsGender[]);
void writeLineInFile(Movement movementToWrite, FILE* fi); 
void writeLine(Movement movementToWrite, int iTest, int iSub, FILE* fiTrain, FILE* fiTest); 
void freeString(char string[], int length); 
void generationFile(); 
// phase 2 
void wellClassified(int realClasses[], int estimatedClasses[], Class classes[], int nbTests);
double globalAccuracy(Class classes[]);
void accuracy(Class classes[]); 
void displayResultsByClass(int realClasses[], int estimatedClasses[], int nbTests); 
void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests); 
void displayConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests); 
 // phase 3 
void initMovementTypeArray(MovementType movementType[]);
void modelCreation();
void modelCreation2();
int decomposition(char line[], double data[]);
void writeAllMovementTypeInFile(MovementType movementType[]);


//phase 4
Indicators indicatorsForAMovement(double data[], Model models[]);
void decompositionModel(char line[], double data[]);
double gapBetweenTwoNumbers(double number1, double number2);
void initModelsArray(Model models[]);
void initMovementsTestAndRealClasses(double movementsTested[NB_TESTS][TIME_EVALUATED], int realClasses[]);
int mostSimilarMovement(int nearestIndicator[]);
void modelEvaluation(void);
 
