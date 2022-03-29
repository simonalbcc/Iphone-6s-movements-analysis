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

typedef struct classe Classe;
struct classe {
	int nbWellClassified;
	int nbTested;
	double accuracy;
	int remplacements[NB_CLASSE_MAX];
};

#pragma endregion 
