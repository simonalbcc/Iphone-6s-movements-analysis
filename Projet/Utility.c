#include "ClassificationPerformances.h"


int decomposition(char line[], double data[]) {
    int i = 0;
    char* token = NULL;
    char* nextToken = NULL;
    char s[SIZE_CHAR_BETWEEN] = ",";
    int mvt; 

    token = strtok_s(line, s, &nextToken);  // avoid type mvt 
    mvt = atoi(line); 
    token = strtok_s(NULL, s, &nextToken);  // avoid gender
    token = strtok_s(NULL, s, &nextToken);  // avoid index

    token = strtok_s(NULL, s, &nextToken); // take first number
    while (token != NULL) {
        data[i] = atof(token);
        i++;
        token = strtok_s(NULL, s, &nextToken);
    }
    return mvt; 

}

void writeAllMovementTypeInFile(MovementType movementType[]) {
    FILE* fiModel; 
    int iMov = 0;
    int iTenthSecond;

    fopen_s(&fiModel, MODEL, "w");

    if (fiModel == NULL) {
        printf("Erreur a l'ouverture du fichier modele");
    } else {
        while (iMov < 6) {
            printf("J'ai bien fait ca %dx\n", iMov+1);
            fprintf(fiModel, "\n");
            fprintf(fiModel, "%d", iMov + 1);
            iTenthSecond = 0;
            while (iTenthSecond < TIME_EVALUATED) {
                fprintf(fiModel, ", %f", movementType[iMov].averagePerTenthSecond[iTenthSecond]);
                iTenthSecond++;
            }
            fprintf(fiModel, "\n");
            fprintf(fiModel, "%d", iMov + 1);
            printf("%d", iTenthSecond);
            iTenthSecond = 0;
            while (iTenthSecond < TIME_EVALUATED) {
                fprintf(fiModel, ",%f", movementType[iMov].standardDeviation[iTenthSecond]);
                iTenthSecond++;
            }
            fprintf(fiModel, "\n");
            fprintf(fiModel, "%d", iMov + 1);
            fprintf(fiModel, ", %f", movementType[iMov].generalAverage);

            iMov++;
        }
    }
}

int minusDistanceStdClass(double data[], Model models[]) {
    double std[NB_TYPE][TIME_EVALUATED];
    int lookLike[NB_TYPE] = {0,0,0,0,0,0};

    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        for (int iTenthSecond = 0; iTenthSecond < TIME_EVALUATED; iTenthSecond++) {
            std[iModel][iTenthSecond] = sqrt(pow((data[iTenthSecond] - models[iModel].averages[iTenthSecond]), 2));
        }
    }

    for (int iTenthSecond = 0; iTenthSecond < TIME_EVALUATED; iTenthSecond++) {
        int min = INT_MAX;
        int minIModel;
        for (int iModel = 0; iModel < NB_TYPE; iModel++) {
            double gapWithModel = gapBetweenTwoNumbers(std[iModel][iTenthSecond], models[iModel].stds[iTenthSecond]);
            if (min > gapWithModel) {
                min = gapWithModel;
                minIModel = iModel;
            }
            lookLike[minIModel]++;
        }
    }

    int min = 0;
    for (int iModel = 1; iModel < NB_TYPE; iModel++) {
        if (lookLike[min] > lookLike[iModel]) {
            min = iModel;
        }
    }
    return min + 1;
}

double gapBetweenTwoNumbers(double number1, double number2) {
    return sqrt(pow((number1 - number2), 2));
}
