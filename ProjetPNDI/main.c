#define _CRT_SECURE_NO_WARNINGS

#include "Biblio.h"

// PROJET DE YOURI DAUTREBANDE, BRYCE MOLITOR

// PHASE 1
void createFilesSet(char fileName[]);
void createListingSubjects(Person subjects[]);
void browseFolder(char path[][LG_PATHS], Person subjects[]);
int browseFile(Person subjects[], int index, char folder[], FileSet fileSelected);
void writeLine(FileSet file, bool lastLine);
// PHASE 3
void createFileModel(char fileName[]);
void readData(char file[], Model model[]);

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

	// PHASE 1
	//createFilesSet("trainSet.csv");
	//createFilesSet("testSet.csv");
	//createListingSubjects(subjects);
	//browseFolder(paths, subjects);

	// PHASE 2
	int estimatedClasses[] = { 1, 3, 4, 2, 5, 6, 2, 1};
	int realClassesV[] = { 1, 3, 4, 2, 5, 6, 2, 1};
	int realClasses[] = { 1, 4, 5, 2, 3, 6, 1, 1 };
	//displayResultsByClass(realClasses, estimatedClasses, 8);
	//displayAccuracy(realClasses, estimatedClasses, 8);
	//displayConfusionMatrix(realClasses, estimatedClasses, 8);

	// PHASE 3
	createFileModel("fiModel.csv");
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

		for (int i = 0; i < NB_DATA - 1; i++) {
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
			double racine;
			char* token;
			int iLine = 0;
			int iInfo;
			Data data;

			fgets(line, sizeof(line), fiMovement);
			while (fgets(line, sizeof(line), fiMovement) && iLine < NB_DATA) {
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

				fileSelected.vacc = sqrt(pow(data.x, 2) + pow(data.y, 2) + pow(data.z, 2));
				//fileSelected.vacc = racine;

				writeLine(fileSelected, iLine == 0);

				iLine++;
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

void createFileModel(char fileName[]) {
	FILE* fiFile;

	fopen_s(&fiFile, fileName, "w+");
	if (fiFile == NULL) {
		puts("Error opening of file!");
	}
	else {
		fprintf(fiFile, "Movement,");

		for (int i = 0; i < NB_DATA - 1; i++) {
			fprintf(fiFile, "Vacc,");
		}
		fprintf(fiFile, "Vacc\n");

		fclose(fiFile);
	}
}

void readData(char file[], Model model[]) {
	FILE* fiSet;

	fopen_s(&fiSet, file, "r");
	if (fiSet == NULL) {
		printf("Error opening of file!");
	} else {
		char line[200];
		int iMovement;
		int iTotalColumn;
		int iColumn = 0;
		int nbLines = 0;
		double totalAverages = 0;
		double totalValues = 0;
		double totalColumns[NB_DATA];
		double totalColumnsSquare[NB_DATA];
		char movements[NB_CLASSES][LG_MOVEMENT] = { "downstairs", "jogging", "sitDown", "standUp", "upstairs", "walking" };

		fgets(line, sizeof(line), fiSet);	// Ne pas prendre en compte la première ligne
		while (fgets(line, sizeof(line), fiSet)) {
			iMovement = 0;
			iColumn = 3;

			if (iMovement < NB_CLASSES && line[0] != movements[iMovement]) {
				iTotalColumn = 0;
				while (iTotalColumn < NB_DATA) {
					model[iMovement].averages[iTotalColumn] += (totalColumns[iTotalColumn] / nbLines) * 100;
					totalAverages += model[iMovement].averages[iTotalColumn];
					model[iMovement].standardDeviation[iTotalColumn] = sqrt((totalColumnsSquare[iTotalColumn]/nbLines)
																		- pow(model[iMovement].averages[iTotalColumn], 2));

					iTotalColumn++;
				}
				model[iMovement].average = totalValues / (nbLines * NB_DATA);

				totalValues = 0;
				iMovement++;
			} else {
				iTotalColumn = 0;
				while (iTotalColumn < NB_DATA) {
					totalColumns[iTotalColumn] += line[iColumn];
					totalColumnsSquare[iTotalColumn] += pow(line[iColumn], 2);
					totalValues += line[iColumn];

					iTotalColumn++;
					iColumn++;
				}
				iTotalColumn = 0;
			}
		}

		fclose(&fiSet);
	}
}