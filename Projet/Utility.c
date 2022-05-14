#include "ClassificationPerformances.h"


void decomposition(char line[], double data[]) {
    int i = 0;
    char* token = NULL;
    char* nextToken = NULL;
    char s[SIZE_CHAR_BETWEEN] = ",";
    char s2[SIZE_CHAR_BETWEEN] = "|";
    token = strtok_s(line, s, &nextToken);  // avoid type mvt 
    token = strtok_s(NULL, s, &nextToken);  // avoid gender
    token = strtok_s(NULL, s, &nextToken);  // avoid index

    while (token != NULL) {
        data[i] = atof(token);
        i++;
        token = strtok_s(NULL, s2, &nextToken);
    }
}

void writeAllMovementTypeInFile(MovementType movementType[]) {
    FILE* fiModel; 
    int iMov = 1;
    int iTenthSecond;

    fopen_s(&fiModel, MODEL, "w");

    while (iMov < 6) {
        fprintf(fiModel, "\n");
        fprintf(fiModel, "%d, ", iMov);
        iTenthSecond = 0; 
        while (iTenthSecond < TIME_EVALUATED) {
            fprintf(fiModel, ", %f", movementType[iMov].averagePerTenthSecond[iTenthSecond]);
            iTenthSecond++; 
        }
        fprintf(fiModel, "\n"); 
        fprintf(fiModel, "%d,", iMov);
        while (iTenthSecond > 0) {
            fprintf(fiModel, ",%f", movementType[iMov].standardDeviation[iTenthSecond]);
            iTenthSecond--; 
        }
        fprintf(fiModel, "\n");
        fprintf(fiModel, "%d,", iMov);
        fprintf(fiModel, ", %f", movementType[iMov].generalAverage);

        iMov++; 
    }
}

void freeString(char string[], int length) {
    for (int iChar = 0; iChar < length; iChar++) {
        string[iChar] = '\0';
    }
}