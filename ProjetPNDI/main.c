#define _CRT_SECURE_NO_WARNINGS

#include "Biblio.h"

// PROJET DE YOURI DAUTREBANDE, BRYCE MOLITOR

#define NB_VACC_MAX 1000
#define NB_FOLDERS_MAX 15
#define NB_FILES_TEST 3
#define NB_SUBJECTS 24

#define LG_PATHS 20

#define LG_MOVEMENT 11
#define LG_FOLER_TYPE 13

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


void createFilesSet(char fileName[]);
void createListingSubjects(Person subjects[]);
void browseFolder(char path[][LG_PATHS], Person subjects[]);
int browseFile(Person subjects[], int index, char folder[], FileSet fileSelected);
void writeLine(FileSet file, bool lastLine);

void main(void) {
	Person subjects[NB_SUBJECTS];

	char paths[NB_FOLDERS_MAX][LG_PATHS] = {
		"dws_1/sub_",
		"dws_2/sub_",
		"dws_11/sub_",
		"jog_9/sub_",
		"jog_16/sub_",
		"sit_5/sub_",
		"sit_13/sub_",
		"std_6/sub_",
		"std_14/sub_",
		"ups_3/sub_",
		"ups_4/sub_",
		"ups_12/sub_",
		"wlk_7/sub_",
		"wlk_8/sub_",
		"wlk_15/sub_",
	};

	createFilesSet("trainSet.csv");
	createFilesSet("testSet.csv");
	createListingSubjects(subjects);
	browseFolder(paths, subjects);
	

	int estimatedClasses[] = { 1, 3, 4, 2, 5, 6, 2, 1};
	int realClassesV[] = { 1, 3, 4, 2, 5, 6, 2, 1};
	int realClasses[] = { 1, 4, 5, 2, 3, 6, 1, 1 };
	displayResultsByClass(realClasses, estimatedClasses, 8);
	displayAccuracy(realClasses, estimatedClasses, 8);
	displayConfusionMatrix(realClasses, estimatedClasses, 8);
}

void createFilesSet(char fileName[]) {
	FILE* fiFile;

	fopen_s(&fiFile, fileName, "w+");
	if (fiFile == NULL) {
		puts("Error opening of file!");
	}
	else {
		fprintf(fiFile, "Movement,");
		fprintf(fiFile, "Index,");
		fprintf(fiFile, "Gender,");

		for (int i = 0; i < NB_VACC_MAX - 1; i++) {
			fprintf(fiFile, "Vacc,");
		}
		fprintf(fiFile, "Vacc\n");

		fclose(fiFile);
	}
}

void createListingSubjects(Person subjects[]) {
	FILE* fiSubjects;

	fopen_s(&fiSubjects, "data_subjects_info.csv", "r+");
	if (fiSubjects == NULL) {
		printf("Error opening of file!");
	}
	else {
		int iSubject = 0;
		char line[200];
		char* token;
		int infos[5];
		int iInfo;

		fgets(line, sizeof(line), fiSubjects);
		while (fgets(line, sizeof(line), fiSubjects)) {
			iInfo = 0;
			token = strtok(line, ",");
			while (token != NULL) {
				sscanf(token, "%d", &infos[iInfo]);
				token = strtok(NULL, ",");
				iInfo++;
			}

			subjects[iSubject].code = infos[0];
			subjects[iSubject].weight = infos[1];
			subjects[iSubject].height = infos[2];
			subjects[iSubject].age = infos[3];
			subjects[iSubject].gender = infos[4];

			iSubject++;
		}

		fclose(fiSubjects);
	}
}

void browseFolder(char paths[][LG_PATHS], Person subjects[]) {
	char folder[LG_PATHS];
	char typeMovement[4] = "";
	char movement[LG_MOVEMENT];
	FileSet fileSelected;
	int index = 0;

	for (int iFolder = 0; iFolder < NB_FOLDERS_MAX; iFolder++) {     // PASSER DANS LES 15 DOSSIERS
		strcpy_s(folder, LG_PATHS, paths[iFolder]);

		for (int i = 0; i < 3; i++) {
			typeMovement[i] = folder[i];
		}

		if (strcmp(typeMovement, "dws") == 0) {
			strcpy_s(fileSelected.movement, LG_MOVEMENT, "downstairs");
		}
		else if (strcmp(typeMovement, "ups") == 0) {
			strcpy_s(fileSelected.movement, LG_MOVEMENT, "upstairs");
		}
		else if (strcmp(typeMovement, "sit") == 0) {
			strcpy_s(fileSelected.movement, LG_MOVEMENT, "sitDown");
		}
		else if (strcmp(typeMovement, "std") == 0) {
			strcpy_s(fileSelected.movement, LG_MOVEMENT, "standUp");
		}
		else if (strcmp(typeMovement, "wlk") == 0) {
			strcpy_s(fileSelected.movement, LG_MOVEMENT, "walking");
		}
		else {
			strcpy_s(fileSelected.movement, LG_MOVEMENT, "jogging");
		}

		index = browseFile(subjects, index, folder, fileSelected);
	}
}

int browseFile(Person subjects[], int index, char folder[], FileSet fileSelected) {
	int nbSubjectTest = 0;
	int iSubjectForTest = 0;
	char iSubjectStr[3];

	for (int iSubject = 1; iSubject <= NB_SUBJECTS; iSubject++) {  // PASSER DANS LES 24 FICHIERS
		char file[LG_PATHS];

		if (nbSubjectTest < NB_FILES_TEST) {
			strcpy_s(fileSelected.type, LG_FOLER_TYPE, "testSet.csv");
			nbSubjectTest++;
		}
		else {
			strcpy_s(fileSelected.type, LG_FOLER_TYPE, "trainSet.csv");
		}

		strcpy_s(file, LG_PATHS, folder);
		sprintf(iSubjectStr, "%d", iSubject);
		strcat(file, iSubjectStr);
		strcat(file, ".csv");
		puts(file);

		fileSelected.gender = subjects[iSubject - 1].gender;
		fileSelected.index = index;

		FILE* fiMovement;
		fopen_s(&fiMovement, file, "r");
		if (fiMovement == NULL) {
			puts("Error opening of file!");
		}
		else {
			char line[800];
			double infos[13];
			char* token;
			int iLine = 0;
			int iInfo;
			Data data;
			double racine;
			double squaresSum;

			fgets(line, sizeof(line), fiMovement);
			fgets(line, sizeof(line), fiMovement);
			while (!feof(fiMovement) && iLine < NB_VACC_MAX) {
				iInfo = 0;

				token = strtok(line, ",");
				while (token != NULL) {
					sscanf(token, "%lf", &infos[iInfo]);

					token = strtok(NULL, ",");
					iInfo++;
				}

				data.x = infos[10];
				data.y = infos[11];
				data.z = infos[12];

				squaresSum = pow(data.x, 2) + pow(data.y, 2) + pow(data.z, 2);
				racine = sqrt(squaresSum);
				fileSelected.vacc = racine;

				writeLine(fileSelected, iLine == 0);

				iLine++;
				fgets(line, sizeof(line), fiMovement);
			}

			FILE* fiFile;
			fopen_s(&fiFile, fileSelected.type, "a");
			if (fiFile == NULL) {
				puts("Error opening of file!");
			}
			else {
				fprintf(fiFile, "\n");

				fclose(fiFile);
			}

			iSubjectForTest += 4;
			if (iSubjectForTest >= NB_SUBJECTS) {
				iSubjectForTest = 0;
			}

			fclose(fiMovement);
		}
		index++;
	}
	return index;
}

void writeLine(FileSet file, bool firstLine) {
	FILE* fiFile;
	fopen_s(&fiFile, file.type, "a");
	if (fiFile == NULL) {
		puts("Error opening of file!");
	}
	else {
		if (firstLine) {
			fprintf(fiFile, "%s,%d,%d,", file.movement, file.index, file.gender);
		}
		fprintf(fiFile, "%.5lf,", file.vacc);

		fclose(fiFile);
	}
}