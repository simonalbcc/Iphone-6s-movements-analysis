#include "ClassificationPerformances.h"


/*
int decomposition(char line[], float data[]) {
    int i = 0;
    char* token = NULL;
    char* nextToken = NULL;
    char s[2] = ",";
    int mvt;
    token = strtok_s(line, s, &nextToken);
    mvt = atoi(line);
    token = strtok_s(NULL, s, &nextToken); // avoid gender
    token = strtok_s(NULL, s, &nextToken); // avoid index 
    while (token != NULL) {
        data[i] = atof(token);
        i++;
        token = strtok_s(NULL, s, &nextToken);
    }
    return mvt;
}

void writeAllMovementTypeInFile(MovementType movementType[]) {
    FILE* fiModel; 
    int iMov = 1;
    int iTenthSecond;

    fopen_s(&fiModel, MODEL, "a+");

    while (iMov < 6) {
        fprintf(fiModel, "%d", iMov);
        iTenthSecond = 0; 
        while (iTenthSecond < TIME_EVALUATED) {
            fprintf(fiModel, "%f", movementType[iMov].averagePerTenthSecond[iTenthSecond]);
            iTenthSecond++; 
        }
        fprintf(fiModel, "\n"); 
        fprintf(fiModel, "%d", iMov);
        while (iTenthSecond > 0) {
            fprintf(fiModel, "%f", movementType[iMov].standardDeviation[iTenthSecond]);
            iTenthSecond--; 
        }
        fprintf(fiModel, "\n");
        fprintf(fiModel, "%d", iMov);
        fprintf(fiModel, "%f", movementType[iMov].generalAverage);

        iMov++; 
    }


}
*/