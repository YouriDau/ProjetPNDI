#pragma once
// PHASE 2 BRYCE MOLITOR, YOURI DAUTREBANDE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define NB_CLASSES 6

typedef struct stats Stats;
struct stats {
	int iClass;
	int nbCorrects;
	int nbTotal;
};

void displayResultsByClass(int realClasses[], int estimatedClasses[], int nbTests) {
	Stats statsClasses[NB_CLASSES];
	double percentage;
	int iClass;

	for (iClass = 0; iClass < NB_CLASSES; iClass++) {
		statsClasses[iClass].iClass = iClass + 1;
	}

	for (iClass = 0; iClass < NB_CLASSES; iClass++) {
		statsClasses[iClass].nbCorrects = 0;
		statsClasses[iClass].nbTotal = 0;
	}

	iClass = 0;
	for (int iTest = 0; iTest < nbTests; iTest++) {
		iClass = realClasses[iTest] - 1;
		statsClasses[iClass].nbTotal++;

		if (realClasses[iTest] == estimatedClasses[iTest]) {
			statsClasses[iClass].nbCorrects++;
		}
	}

	printf("Classe\t|\tBien classes\t|\tTotal\t|\tPourcentage\n");
	for (iClass = 0; iClass < NB_CLASSES; iClass++) {
		if (statsClasses[iClass].nbTotal != 0) {
			percentage = (statsClasses[iClass].nbCorrects / (double)statsClasses[iClass].nbTotal) * 100;
		}
		else {
			percentage = 0.0;
		}
		printf("%d\t|\t%d\t\t|\t%d\t|\t%.2lf\n", statsClasses[iClass].iClass, statsClasses[iClass].nbCorrects, statsClasses[iClass].nbTotal, percentage);
	}
	printf("\n");
}


void displayAccuracy(int realClasses[], int estimatedClasses[], int nbTests) {
	int nbCorrects = 0;
	double accuracy;

	for (int iTest = 0; iTest < nbTests; iTest++) {
		if (realClasses[iTest] == estimatedClasses[iTest]) {
			nbCorrects++;
		}
	}

	accuracy = (nbCorrects / (double)nbTests) * 100;

	printf("The accuracy is %.2lf %%\n\n", accuracy);
}


void displayConfusionMatrix(int realClasses[], int estimatedClasses[], int nbTests) {
	int matrix[NB_CLASSES][NB_CLASSES];
	int iClass;
	int iEstimated;
	int iReal;

	for (iEstimated = 0; iEstimated < NB_CLASSES; iEstimated++) {
		for (iReal = 0; iReal < NB_CLASSES; iReal++) {
			matrix[iEstimated][iReal] = 0;
		}
	}

	for (iClass = 0; iClass < nbTests; iClass++) {
		iEstimated = estimatedClasses[iClass] - 1;
		iReal = realClasses[iClass] - 1;
		matrix[iEstimated][iReal]++;
	}

	// To display the matrix
	for (iClass = 0; iClass < NB_CLASSES; iClass++) {
		printf("\t    %d", iClass + 1);
	}
	puts("\n");
	for (iEstimated = 0; iEstimated < NB_CLASSES; iEstimated++) {
		printf(" %d", iEstimated + 1);
		for (iReal = 0; iReal < NB_CLASSES; iReal++) {
			printf("\t|   %d", matrix[iEstimated][iReal]);
		}
		printf("\n");
	}
	puts("");

	// To display (for each classes) The numbers of correct answers
	puts("Number of correct answers :");
	for (iClass = 0; iClass < NB_CLASSES; iClass++) {
		printf("\tclass %d : %d\n", iClass + 1, matrix[iClass][iClass]);
	}
}