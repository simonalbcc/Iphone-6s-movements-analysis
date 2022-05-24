#include "ClassificationPerformances.h"

Indicators indicatorsForAMovement(double data[], Model models[]) {
    double std[NB_TYPE][TIME_EVALUATED];
    int nearestIndicatorStd[NB_TYPE];
    int nearestIndicatorAverage[NB_TYPE];
    Indicators indicators;

    double globalAverage = 0;
    int realTimeEvaluated = 0;

    while (realTimeEvaluated < TIME_EVALUATED && data[realTimeEvaluated] != 0) {
        globalAverage  += data[realTimeEvaluated];
        realTimeEvaluated++;
    }
    globalAverage /= realTimeEvaluated;
    double minGlobalAverage = LONG_MAX;
    int minIModelGlobalAverage = 0;
    double gapWithModelGlobalAverage;

    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        nearestIndicatorStd[iModel] = 0;
        nearestIndicatorAverage[iModel] = 0;

        for (int iTenthSecond = 0; iTenthSecond < realTimeEvaluated; iTenthSecond++) {
            std[iModel][iTenthSecond] = gapBetweenTwoNumbers(data[iTenthSecond],models[iModel].averages[iTenthSecond]);
        }

        gapWithModelGlobalAverage = gapBetweenTwoNumbers(globalAverage, models[iModel].globalAvg);
        if (minGlobalAverage > gapWithModelGlobalAverage) {
            minGlobalAverage = gapWithModelGlobalAverage;
            minIModelGlobalAverage = iModel;
        }
    }

    for (int iTenthSecond = 0; iTenthSecond < realTimeEvaluated; iTenthSecond++) {
        double minStd = LONG_MAX;
        double minAverage = LONG_MAX;
        int minIModelStd = 0;
        int minIModelAverage = 0;

        for (int iModel = 0; iModel < NB_TYPE; iModel++) {
            double gapWithModelStd = gapBetweenTwoNumbers(std[iModel][iTenthSecond], models[iModel].stds[iTenthSecond]);
            double gapWithModelAverage  = gapBetweenTwoNumbers(data[iTenthSecond], models[iModel].averages[iTenthSecond]);

            if (minStd > gapWithModelStd) {
                minStd = gapWithModelStd;
                minIModelStd = iModel;
            }

            if (minAverage > gapWithModelAverage) {
                minAverage = gapWithModelAverage;
                minIModelAverage = iModel;
            }
        }
        nearestIndicatorStd[minIModelStd]++;
        nearestIndicatorAverage[minIModelAverage]++;
    }
    
    indicators.indicator1 = mostSimilarMovement(nearestIndicatorAverage);
        indicators.indicator2 = mostSimilarMovement(nearestIndicatorStd);
        indicators.indicator3 = minIModelGlobalAverage;
        return indicators;
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
    Indicators indicators;

    for (int iTest = 0; iTest < NB_TESTS; iTest++) {

        indicators = indicatorsForAMovement(movementsTested[iTest], models);

        if (indicators.indicator2 == indicators.indicator3) {
            estimatedClasses[iTest] = indicators.indicator2 + 1;
        } else {
            estimatedClasses[iTest] = indicators.indicator1 + 1;
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