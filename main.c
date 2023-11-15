#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* open_file(char * path){
    FILE *fp;
    // opening the file
    fp = fopen(path, "r");

    if (fp == NULL){
        printf("[INFO] Error opening file\n");
        return NULL;
    }
    printf("[INFO] File has been opened successfully.\n");
    return fp;
}

// This function takes a line from the csv file and a column number (preferency_num) and it returns the day chosen by the family
const char* getChoice(char* line, int column_number){
    const char* token; // Init token variable
    for (token = strtok(line, ";"); token && *token; token = strtok(NULL, ";\n"))
    {
        if (!--column_number)
            return token;
    }
    return NULL;
}

// Function that stores the file in a matrix format
int** getRows(int num_lines, int num_columns, char* PATH_CSV_2){
    FILE *fp; //File pointer variable
    char line[1000];
    // Allocate memory for the matrix A
    int** families_pref = (int **)malloc(num_lines * sizeof(int *));
    for (int i = 0; i < num_lines; i++) {
        families_pref[i] = (int *)malloc(num_columns * sizeof(int));
    }

    // opening the file
    fp = open_file(PATH_CSV_2);

    printf("[INFO] Filling up the matrix A.\n");

    // // Read each line of the file and store the values in the matrix A
    int row =0;
    while(fgets(line, 1000, fp)!= NULL){
        if (row == 0) {
            // Skip header row
            row++;
            continue;
        }
        char * value;
        char * duplicate_line;
        for (int column_number=1; column_number <7; column_number++){
            duplicate_line = strdup(line);
            value = getChoice(duplicate_line, column_number);
            printf("value : %s \n", value);
            families_pref[row-1][column_number-1] = atoi(value);
            free(duplicate_line);
        }
        row++;
    }
    fclose(fp);// close the file
    return families_pref;
}

//objective function
int fonction_objectif(int count[], int penalty[], int cost){

    int sum_penalties = 0;
    int fonction_objectif_value = 0;
    for(int i = 0 ; i< 6 ; i++){
        penalty[i]+= ((count[i] - 125)/400)*pow(count[i],(count[i]-count[i+1])/50);
    }

    //Condition about the 6st day
    penalty[6]+= ((count[6] - 125)/400);

    for(int i =0 ; i<7 ; i++){
        sum_penalties += penalty[i];
    }

    fonction_objectif_value = cost + sum_penalties;

    printf("[INFO] La fonction objectif = %d \n",fonction_objectif_value);

    return fonction_objectif_value;
}

int main()
{
    FILE *fp; // File pointer variable
    int MAX_ROWS = 300;
    int MAX_CHAR = 1000;
    char line[MAX_CHAR];
    int num_lines = 0;
    int num_columns = 6;
    int NUM_DAYS = 7;
    int count[6] = {0,0,0,0,0,0,0};
    int index_member[6] = {0,0,0,0,0,0,0}; //List that stores the number of people per day
    int cost = 0; //Calculate the cost of families
    int penalty[6] = {0,0,0,0,0,0,0}; //Calculate the penalty of days
    char* PATH_CSV_1 = "C:/Users/Oumaima/OneDrive - IMT MINES ALES/Documents/2IA/C/Projet/pb10.csv";
    char* PATH_CSV_2 = "C:/Users/Oumaima/OneDrive - IMT MINES ALES/Documents/2IA/C/Projet/pb20.csv";
    char* PATH_CSV_3 = "C:/Users/Oumaima/OneDrive - IMT MINES ALES/Documents/2IA/C/Projet/pb30.csv";
    char* PATH_CSV_4 = "C:/Users/Oumaima/OneDrive - IMT MINES ALES/Documents/2IA/C/Projet/pb40.csv";
    char* PATH_CSV_5 = "C:/Users/Oumaima/OneDrive - IMT MINES ALES/Documents/2IA/C/Projet/pb50.csv";

    printf("---------------------------------------------------------------\n");
    printf("[INFO] Start of program!\n");

    // opening the file
    fp = open_file(PATH_CSV_2);

    //Counting the lines in the file
    while(fgets(line,MAX_CHAR,fp)){
        num_lines++;
    }

    // Closing the file
    fclose(fp);

    //Initialize the matrix families_pref that will copy the file
    int** families_pref = getRows(num_lines, num_columns, PATH_CSV_2);

    //Initialize the matrix chosen_Days that will generate the days and families according to their previous choices
    int chosen_Days[MAX_ROWS][NUM_DAYS];

    // Initialize the matrix with zeros
    memset(chosen_Days, 0, sizeof(chosen_Days));

    //Number of lines
    printf("The file has %d lines.\n", num_lines);

    printf("---------------------------------------------------------------\n");

    // Fill the matrix chosenDays with data from the matrix families_pref by ascending preferencies
    for(int i = 0 ; i<num_lines ; i++){ //We fix a line of a family
        for(int j = 1 ; j<6 ; j++){ //We fix a column of a preferency
            for(int k = 0 ; k<7 ; k++){ //we search for the chosen day according to a preferency
                if(families_pref[i][j] == k && count[k]<300){
                    chosen_Days[index_member[k]][k] = families_pref[i][0];
                    index_member[k]+=1;
                    count[k] += families_pref[i][0];
                    printf("-----------------------------------------------------------\n");
                    switch(j) //According to the value of j which correspond to a preferency, we calculate the cost
                    {
                    case 1 :
                        break;
                    case 2 :
                        cost+= 50;
                        break;
                    case 3 :
                        cost+= 50 +9*families_pref[i][0];
                        break;
                    case 4 :
                        cost += 100 + 9*families_pref[i][0];
                        break;
                    case 5 :
                        cost += 200 +9*families_pref[i][0];
                        break;
                    }
                }
            }
        }
    }

    printf("---------------------------------------------------------------\n");

    printf("[INFO] Result of days \n");
    // Showing the output matrix
    for (int i = 0; i < MAX_ROWS ; i++){
        for (int j = 0; j < 7 ; j++) {
            printf("[INFO] chosen_Days[%d][%d] = %d \n", i,j,chosen_Days[i][j]);
        }
        printf("\n");
    }

    printf("---------------------------------------------------------------\n");

    // compute objective function
    fonction_objectif(count, penalty, cost);

    printf("---------------------------------------------------------------\n");

    // Free the allocated memory for the input matrix
    for (int i = 0; i < num_lines ; i++){
        free(families_pref[i]);
    }
    free(families_pref);

    // Free the allocated memory for the output matrix
    for (int i = 0; i < MAX_ROWS ; i++){
        free(chosen_Days[i]);
    }
    free(chosen_Days);

    //closing the file
    fclose(fp);
    printf("[INFO] End of program!");
    return 0;
}
