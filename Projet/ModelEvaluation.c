#include "ClassificationPerformances.h"

int minusDistanceStdClass(double data[], Model models[]) {
    double std[NB_TYPE][TIME_EVALUATED];
    int lookLike[NB_TYPE + 1];

    for (int iModel = 0; iModel < NB_TYPE; iModel++) {
        lookLike[NB_TYPE] = 0;
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
        lookLike[minIModel]++;
    }

    int min = 0;
    for (int iModel = 1; iModel < NB_TYPE; iModel++) {
        if (lookLike[min] > lookLike[iModel]) {
            min = iModel;
        }
    }
    return min + 1;
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

void initMovementsTest(MovementTested movementsTested[]) {
    FILE* fiTest;
    char line[SIZE_LINE];

    fopen_s(&fiTest, TESTSET, "r");

    if (fiTest == NULL) {
        printf("Erreur à l'ouverture du fichier !");
    }else {
        fgets(line, SIZE_LINE, fiTest); //get headline
        fgets(line, SIZE_LINE, fiTest);//get fiest line
        int iMov = 0;
        while (!feof(fiTest)){
            movementsTested[iMov].type = decomposition(line, movementsTested[iMov].vAcc);
            fgets(line, SIZE_LINE, fiTest);
            iMov++;
        }
    }


}