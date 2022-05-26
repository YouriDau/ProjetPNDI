#pragma once
// PHASE 2 BRYCE MOLITOR, YOURI DAUTREBANDE
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define NB_CLASSES 6
#define NB_FOLDERS_MAX 15
#define NB_FILES_TEST 3
#define NB_SUBJECTS 24
#define NB_DATA 600

#define LG_PATHS 20
#define LG_MOVEMENT 13
#define LG_FOLER_TYPE 13

char MOVEMENTS[NB_CLASSES][LG_MOVEMENT] = { "downstairs", "jogging", "sitDown", "standUp", "upstairs", "walking" };

// PHASE 1
typedef struct person Person;
struct person {
	int code;
	int weight;
	int height;
	int age;
	int gender; // 0 femme | 1 homme
};

typedef struct fileSet FileSet;
struct fileSet {
	char type[LG_FOLER_TYPE]; // Train, Test
	char movement[LG_MOVEMENT];
	int gender;
	int index;
	double vacc;
};

typedef struct data Data;
struct data {
	double x;
	double y;
	double z;
};

// PHASE 2
typedef struct stats Stats;
struct stats {
	int iClass;
	int nbCorrects;
	int nbTotal;
};

// PHASE 3
typedef struct model Model;
struct model {
	char movement[LG_MOVEMENT];
	double average;
	double averages[NB_DATA];
	double standardDeviation[NB_DATA];
};

// PHASE 2
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
