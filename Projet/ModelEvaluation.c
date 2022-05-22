#include "ClassificationPerformances.h"

int minusDistanceStdClass(double data[], Model models[]) {
    double std[NB_TYPE][TIME_EVALUATED];
    int nearestIndicator[NB_TYPE + 1];

    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        nearestIndicator[NB_TYPE] = 0;
        for (int iTenthSecond = 0; iTenthSecond < TIME_EVALUATED; iTenthSecond++) {
            std[iModel][iTenthSecond] = sqrt(pow((data[iTenthSecond] - models[iModel].averages[iTenthSecond]), 2));
        }
    }

    for (int iTenthSecond = 0; iTenthSecond < TIME_EVALUATED; iTenthSecond++) {
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

int minusDistanceAverages(double data[], Model models[]) {
    int nearestIndicator[NB_TYPE + 1];

    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        nearestIndicator[NB_TYPE] = 0;
    }

    for (int iTenthSecond = 0; iTenthSecond < TIME_EVALUATED; iTenthSecond++) {
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
        printf("Erreur à l'ouverture du fichier !");

    }else {
        fgets(line, SIZE_LINE, fiModel); //get headline
        fgets(line, SIZE_LINE, fiModel);//get fiest line
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
    }
}

void initMovementsTestAndRealClasses(double movementsTested[NB_TESTS][TIME_EVALUATED], int realClasses[]) {
    FILE* fiTest;
    char line[SIZE_LINE];

    fopen_s(&fiTest, TESTSET, "r");

    if (fiTest == NULL) {
        printf("Erreur a l'ouverture du fichier !");
    }
    else {
        fgets(line, SIZE_LINE, fiTest); //get headline
        fgets(line, SIZE_LINE, fiTest);//get fiest line
        int iMov = 0;
        while (!feof(fiTest)) {
            realClasses[iMov] = decomposition(line, movementsTested[iMov]);
            fgets(line, SIZE_LINE, fiTest);
            iMov++;
        }
    }
}

void initEstimatedClasses(int estimatedClasses[], double movementsTested[][TIME_EVALUATED], Model models[]) {

    for (int iTest = 0; iTest < NB_TESTS; iTest++) {

        int ind1 = minusDistanceStdClass(movementsTested[iTest], models);
        int ind2 = minusDistanceAverages(movementsTested[iTest], models);

        double globalAverage = 0;
        for (int iTenthSecond = 0; iTenthSecond < TIME_EVALUATED; iTenthSecond++) {
            globalAverage += movementsTested[iTest][iTenthSecond];
        }
        globalAverage /= TIME_EVALUATED;
        int ind3 = minusDistanceGloballAverage(globalAverage, models);

        if (ind1 == ind2 || ind1 == ind3 ) {
            estimatedClasses[iTest] = ind1 + 1;
        }
        else {
            if (ind2 == ind3) {
                estimatedClasses[iTest] = ind2 + 1;
            }
            else {
                estimatedClasses[iTest] = ind1 + 1;
            }
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
}