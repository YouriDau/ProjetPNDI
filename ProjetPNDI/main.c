#define _CRT_SECURE_NO_WARNINGS

#include "Biblio.h"

// PROJET DE YOURI DAUTREBANDE, BRYCE MOLITOR

// PHASE 1
void createFilesSet(char fileName[]);
void createListingSubjects(Person subjects[]);
void browseFolder(char path[][LG_PATHS], Person subjects[]);
int browseFile(Person subjects[], int index, char folder[], FileSet fileSelected);
void writeLine(FileSet file, FILE* fi, bool lastLine);
// PHASE 3
void createFileModel(char fileName[]);
void readData(char file[], Model models[]);
void insertData(char file[], Model models[]);
// PHASE 4
void estimateTypeOfMovement(Model models[]);
double distanceEuclidienne(double data[], Model models[], int iClass);

void main(void) {
	Person subjects[NB_SUBJECTS];
	Model models[NB_CLASSES];

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
	createFilesSet("trainSet.csv");
	createFilesSet("testSet.csv");
	createListingSubjects(subjects);
	browseFolder(paths, subjects);
	

	// PHASE 3
	createFileModel("fiModel.csv"); 
	readData("trainSet.csv", models);
	insertData("fiModel.csv", models);

	// PHASE 4
	estimateTypeOfMovement(models);
	
}

// Phase 1

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

	for (int iSubject = 2; iSubject <= NB_SUBJECTS; iSubject++) {  // PASSER DANS LES 24 FICHIERS
		if (nbSubjectTest < NB_FILES_TEST) {
			strcpy_s(fileSelected.type, LG_FOLER_TYPE, "testSet.csv");
			nbSubjectTest++;
		}
		else {
			strcpy_s(fileSelected.type, LG_FOLER_TYPE, "trainSet.csv");
		}
		char file[LG_PATHS];

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
			
			FILE* fiTrainSet;
			FILE* fiTestSet;

			fopen_s(&fiTrainSet, "trainSet.csv", "a");
			fopen_s(&fiTestSet, "testSet.csv", "a");
			if (fiTrainSet == NULL || fiTestSet == NULL) {
				puts("Error opening trainSet or testSet of file!");
			}
			else {
				char line[800];
				int iLine = 0;
				Data data;
				double trash;
				bool fiIsTrain;

				fgets(line, sizeof(line), fiMovement);
				while (iLine < NB_DATA && !feof(fiMovement)) {
					fgets(line, sizeof(line), fiMovement);

					sscanf(line, "%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf", &trash, &trash, &trash, &trash, &trash, &trash, &trash, &trash, &trash, &trash, &data.x, &data.y, &data.z);

					fileSelected.vacc = sqrt(pow(data.x, 2) + pow(data.y, 2) + pow(data.z, 2));

					fiIsTrain = strcmp(fileSelected.type, "trainSet.csv") == 0;

					if (fiIsTrain) {
						writeLine(fileSelected, fiTrainSet, iLine == 0);
					} else {
						writeLine(fileSelected, fiTestSet, iLine == 0);
					}
					iLine++;
					fgets(line, sizeof(line), fiMovement);
				}
				if (fiIsTrain) {
					fprintf(fiTrainSet, "%s", "\n");
				} else {
					fprintf(fiTestSet, "%s", "\n");
				}

				iSubjectForTest += 4;
				if (iSubjectForTest >= NB_SUBJECTS) {
					iSubjectForTest = 0;
				}

				fclose(fiTestSet);
				fclose(fiTrainSet);
			}
			fclose(fiMovement);
		}
	
		index++;
	}
	return index;
}

void writeLine(FileSet file, FILE* fi, bool firstLine) {
	if (firstLine) {
		fprintf(fi, "%s,%d,%d,", file.movement, file.index, file.gender);
	}

	fprintf(fi, "%.5f,", file.vacc);
}

//phase 3

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

// Changement par rapport au da car le code ne permet peut-être pas de stocker toutes les valeurs dans un tableau
void readData(char file[], Model models[]) {
	FILE* fiSet;

	fopen_s(&fiSet, file, "r");
	if (fiSet == NULL) {
		printf("Error opening of file!");
	}
	else {
		char line[6000];
		double data[600];
		double sumTotal;
		int iMovement;
		int nbLines[NB_CLASSES];
		char movement[LG_MOVEMENT];


		// Init the arrays at 0
		for (int i = 0; i < NB_DATA; i++) {
			data[i] = 0;
		}
		for (int iMovement = 0; iMovement < NB_CLASSES; iMovement++) {
			nbLines[iMovement] = 0;

			for (int iData = 0; iData < NB_DATA; iData++) {
				models[iMovement].averages[iData] = 0;
				models[iMovement].standardDeviation[iData] = 0;
			}
			models[iMovement].average = 0;
		}

		fgets(line, sizeof(line), fiSet);	// Ne pas prendre en compte la première ligne
		while (fgets(line, sizeof(line), fiSet)) {
			char* token;
			int iData = 0;
			
			token = strtok(line, ",");
			// Récupérer le type de movements
			sscanf(token, "%s", &movement);
			//puts(movement);

			// passe 2 colonnes 
			token = strtok(NULL, ",");
			token = strtok(NULL, ",");
			// Récupérer les VACC
			token = strtok(NULL, ",");
			while (token != NULL) {
				sscanf(token, "%lf", &data[iData]);
				token = strtok(NULL, ",");
				iData++;
			}

			// trouver l'id du mouvement
			if (strcmp(movement, "downstairs") == 0) {
				iMovement = 0;
			}
			else if (strcmp(movement, "jogging") == 0) {
				iMovement = 1;
			}
			else if (strcmp(movement, "sitDown") == 0) {
				iMovement = 2;
			}
			else if (strcmp(movement, "standUp") == 0) {
				iMovement = 3;
			}
			else if (strcmp(movement, "upstairs") == 0) {
				iMovement = 4;
			}
			else if (strcmp(movement, "walking") == 0) {
				iMovement = 5;
			}
			
			nbLines[iMovement]++;

			// Faire les sommes pour la moyenne et std
			for (int iData = 0; iData < NB_DATA; iData++) {
				models[iMovement].averages[iData] += data[iData];
				models[iMovement].standardDeviation[iData] += pow(data[iData], 2);
			}
		}

		fclose(fiSet);
		for (int iMovement = 0; iMovement < NB_CLASSES; iMovement++) {
			sumTotal = 0;
			for (int iData = 0; iData < NB_DATA; iData++) {
				sumTotal += models[iMovement].averages[iData];
				models[iMovement].averages[iData] /= nbLines[iMovement];
				models[iMovement].standardDeviation[iData] = sqrt((models[iMovement].standardDeviation[iData] / nbLines[iMovement]) -
														pow(models[iMovement].averages[iData], 2));
			}
			models[iMovement].average = sumTotal / ((double)nbLines[iMovement] * NB_DATA);
		}
		
	}
}

//fonction qui à pour but de transférer les données du tableau models et de les écrire dans le fichier
void insertData(char file[], Model models[]) {
	FILE* fiModel;

	fopen_s(&fiModel, file, "a");
	if (fiModel == NULL) {
		printf("Error opening of file!");
	} else {
		char line[800];

		fgets(line, sizeof(line), fiModel); //on évitre la première ligne

		for (int iMovement = 0; iMovement < NB_CLASSES; iMovement++) {

			fprintf(fiModel, "%s,", MOVEMENTS[iMovement]);
			for (int i = 0; i < NB_DATA; i++) {
				fprintf(fiModel, "%.5f,", models[iMovement].averages[i]);
			}
			fprintf(fiModel, "%s", "\n");

			fprintf(fiModel, "%s,", MOVEMENTS[iMovement]);
			for (int i = 0; i < NB_DATA; i++) {
				fprintf(fiModel, "%.5f,", models[iMovement].standardDeviation[i]);
			}
			fprintf(fiModel, "%s", "\n");

			fprintf(fiModel, "%s,", MOVEMENTS[iMovement]);
			fprintf(fiModel, "%.5f,", models[iMovement].average);
			fprintf(fiModel, "%s", "\n");

			printf("Movement_%d    Writed\n", iMovement);
		}

		fclose(fiModel);
	}
}


//phase 4
void estimateTypeOfMovement(Model models[]) {
	int nbTests = 0;
	FILE* fiTest;

	fopen_s(&fiTest, "testSet.csv", "r");
	if (fiTest == NULL) {
		printf("Error opening of file!");
	} else {
		int realClasses[600];
		int estimatedClasses[600];
		char line[6000];
		int iData;

		fgets(line, sizeof(line), fiTest); //on évitre la première ligne
		while (fgets(line, sizeof(line), fiTest))
		{

			char* token;
			int iData = 0;
			char movement[LG_MOVEMENT];
			double data[600];

			token = strtok(line, ",");
			// Récupérer le type de movements
			sscanf(token, "%s", &movement);
			//puts(movement);

			// passe 2 colonnes 
			token = strtok(NULL, ",");
			token = strtok(NULL, ",");
			// Récupérer les VACC
			token = strtok(NULL, ",");
			while (token != NULL) {
				sscanf(token, "%lf", &data[iData]);
				token = strtok(NULL, ",");
				iData++;
			}

			double sum = 0;
			double correctAvg = 0;
			double indicators[3];

			if (strcmp(movement, "downstairs") == 0) {
				realClasses[nbTests] = 1;
			}
			else if (strcmp(movement, "jogging") == 0) {
				realClasses[nbTests] = 2;
			}
			else if (strcmp(movement, "sitDown") == 0) {
				realClasses[nbTests] = 3;
			}
			else if (strcmp(movement, "standUp") == 0) {
				realClasses[nbTests] = 4;
			}
			else if (strcmp(movement, "upstairs") == 0) {
				realClasses[nbTests] = 5;
			}
			else if (strcmp(movement, "walking") == 0) {
				realClasses[nbTests] = 6;
			}

			for (iData = 0; iData < NB_DATA; iData++) {
				sum += data[iData];
			}

			double average = sum / iData;
			double stadardDeviation[600];
			double minValueAvergage = 100;
			double minValueAvgDifference = 100;
			double minValueStd = 100;

			for (int iClass = 0; iClass < NB_CLASSES; iClass++) {
				double std[600];
				double result = distanceEuclidienne(data, models, iClass);

				if (result < minValueAvergage) {
					minValueAvergage = result;
					indicators[0] = iClass + 1;
				}

				for (iData = 0; iData < NB_DATA; iData++) {
					std[iData] = sqrt(pow(data[iData] - models[iClass].averages[iData], 2));
				}
				
				sum = 0;
				for (iData = 0; iData < NB_DATA; iData++) {
					sum += pow((data[iData] - models[iClass].standardDeviation[iData]), 2);
				}

				double resultStd = sqrt(sum);
				double valueMinStd = 100;
				if (resultStd < minValueStd) {
					valueMinStd = resultStd;
					indicators[1] = iClass + 1;
				}


				double differenceAvg;
				if (average < models[iClass].average) {
					differenceAvg = models[iClass].average - average;
				} else {
					differenceAvg = average - models[iClass].average;
				}

				if (differenceAvg < minValueAvgDifference) {
					minValueAvgDifference = differenceAvg;
					indicators[2] = iClass + 1;
				}
			}

			if (indicators[0] == indicators[1] || indicators[0] == indicators[2]) {
				estimatedClasses[nbTests] = indicators[0];
			} else {
				if (indicators[1] == indicators[2]) {
					estimatedClasses[nbTests] = indicators[1];
				} else {
					estimatedClasses[nbTests] = indicators[0];
				}
			}

			nbTests++;
		}

		fclose(fiTest);

		displayResultsByClass(realClasses, estimatedClasses, nbTests);
		displayAccuracy(realClasses, estimatedClasses, nbTests);
		displayConfusionMatrix(realClasses, estimatedClasses, nbTests);
	}
}

double distanceEuclidienne(double data[], Model models[], int iClass) {
	double result = 0;
	int iData = 0;

	while (iData < NB_DATA)
	{
		result += pow(data[iData] - models[iClass].averages[iData], 2);
		iData++;
	}
	return result = sqrt(result);
}
