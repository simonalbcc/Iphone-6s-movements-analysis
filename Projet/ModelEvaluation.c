#include "ClassificationPerformances.h"

int minusDistanceStdClass(double data[], Model models[], int realTimeEvaluated) {
    double std[NB_TYPE][TIME_EVALUATED];
    int nearestIndicator[NB_TYPE];

    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        nearestIndicator[iModel] = 0;
        for (int iTenthSecond = 0; iTenthSecond < realTimeEvaluated; iTenthSecond++) {
            std[iModel][iTenthSecond] = sqrt(pow((data[iTenthSecond] - models[iModel].averages[iTenthSecond]), 2));
        }
    }

    for (int iTenthSecond = 0; iTenthSecond < realTimeEvaluated; iTenthSecond++) {
        double min = LONG_MAX;
        int minIModel = 0;
        for (int iModel = 0; iModel < NB_TYPE; iModel++) {
            double gapWithModel = gapBetweenTwoNumbers(std[iModel][iTenthSecond], models[iModel].stds[iTenthSecond]);
            if (min > gapWithModel) {
                min = gapWithModel;
                minIModel = iModel;
            }
        }
        nearestIndicator[minIModel]++;
    }

    return mostSimilarMovement(nearestIndicator);
}

int minusDistanceAverages(double data[], Model models[], int realTimeEvaluated) {
    int nearestIndicator[NB_TYPE];

    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        nearestIndicator[iModel] = 0;
    }

    for (int iTenthSecond = 0; iTenthSecond < realTimeEvaluated; iTenthSecond++) {
        double min = LONG_MAX;
        int minIModel = 0;
        for (int iModel = 0; iModel < NB_TYPE; iModel++) {
            double gapWithModel = gapBetweenTwoNumbers(data[iTenthSecond],models[iModel].averages[iTenthSecond]);
            if (min > gapWithModel) {
                min = gapWithModel;
                minIModel = iModel;
            }
        }
        nearestIndicator[minIModel]++;
    }

    return mostSimilarMovement(nearestIndicator);
}

int minusDistanceGloballAverage(double data, Model models[]) {
    double min = LONG_MAX;
    int minIModel = 0;
    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        double gapWithModel = gapBetweenTwoNumbers(data, models[iModel].globalAvg);
        if (min > gapWithModel) {
            min = gapWithModel;
            minIModel = iModel;
        }
    }
    return minIModel;
}

void initModelsArray(Model models[]) {
    FILE* fiModel;
    char line[SIZE_LINE];

       
    fopen_s(&fiModel, MODEL, "r");

    if (fiModel == NULL) {
        printf("Erreur à l'ouverture du fichier model!");

    }else {
        fgets(line, SIZE_LINE, fiModel); //get headline
        fgets(line, SIZE_LINE, fiModel); //get first line
        int iModel = 0;
        while (!feof(fiModel)) {
            decompositionModel(line, models[iModel].averages);
            fgets(line, SIZE_LINE, fiModel);
            decompositionModel(line, models[iModel].stds);
            fgets(line, SIZE_LINE, fiModel);
            sscanf_s(line, "%d,%lf", &models[iModel].type, &models[iModel].globalAvg);
            fgets(line, SIZE_LINE, fiModel);
            iModel++;
        }
        fclose(fiModel);
    }
}

void initMovementsTestAndRealClasses(double movementsTested[NB_TESTS][TIME_EVALUATED], int realClasses[]) {
    FILE* fiTest;
    char line[SIZE_LINE];

    fopen_s(&fiTest, TESTSET, "r");

    if (fiTest == NULL) {
        printf("Erreur a l'ouverture du fichier testset !");
    }
    else {
        fgets(line, SIZE_LINE, fiTest); //get headline
        fgets(line, SIZE_LINE, fiTest); //get first line
        int iMov = 0;
        while (!feof(fiTest)) {
            realClasses[iMov] = decomposition(line, movementsTested[iMov]);
            iMov++;
            fgets(line, SIZE_LINE, fiTest);
        }
        realClasses[iMov] = decomposition(line, movementsTested[iMov]); // for the last line 
        fclose(fiTest);
    }
}

void initEstimatedClasses(int estimatedClasses[], double movementsTested[][TIME_EVALUATED], Model models[]) {
    

    for (int iTest = 0; iTest < NB_TESTS; iTest++) {

        GlobalAverage globalAverage = generalAverageMovement(movementsTested[iTest]);
           
        globalAverage.sum /= globalAverage.realTimeEvaluated;

        int ind1 = minusDistanceStdClass(movementsTested[iTest], models, globalAverage.realTimeEvaluated);
        int ind2 = minusDistanceAverages(movementsTested[iTest], models, globalAverage.realTimeEvaluated);
        int ind3 = minusDistanceGloballAverage(globalAverage.sum, models);

        if (ind2 == ind3) {
            estimatedClasses[iTest] = ind2 + 1;
        } else {
            estimatedClasses[iTest] = ind1 + 1;
        }
    }
}

void modelEvaluation(void) {
    double movementsTested[NB_TESTS][TIME_EVALUATED];
    int realClasses[NB_TESTS];
    int estimatedClasses[NB_TESTS];
    Model models[NB_TYPE];

    initModelsArray(models);
    initMovementsTestAndRealClasses(movementsTested, realClasses);
    initEstimatedClasses(estimatedClasses, movementsTested, models);

    displayResultsByClass(realClasses, estimatedClasses, NB_TESTS);
    displayAccuracy(realClasses, estimatedClasses, NB_TESTS);
    displayConfusionMatrix(realClasses, estimatedClasses, NB_TESTS);
}