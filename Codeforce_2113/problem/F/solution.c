#include <stdio.h>
#include <stdlib.h>



int array_lenght;
int max_value; // Maximum value in the arrays, as per the problem statement
int *array1; 
int *array2; 
int score = 0; 

int *array1_description; // the number of each value in array1
int *array2_description;

// generate and count the number of each value in array1 and array2
void describe_arrays() {
    array1_description = (int *)malloc(max_value * sizeof(int));
    array2_description = (int *)malloc(max_value * sizeof(int));

    for (int i = 0; i < max_value; i++) {
        array1_description[i] = 0;
        array2_description[i] = 0;
    }

    for (int i = 0; i < array_lenght; i++) {
        array1_description[array1[i]-1]++;
        array2_description[array2[i]-1]++;
    }
}

// Get the improvment of score if a change is aplied
int get_change_score(int int_1, int int_2) {

    if(int_1 == int_2) {
        return 0;
    }

    int gain1 = 0;
    int gain2 = 0;

    int count1_1 = array1_description[int_1-1];
    int count2_2 = array2_description[int_2-1];

    int count1_2 = array1_description[int_2-1];
    int count2_1 = array2_description[int_1-1];

    if( count1_1 > 1 && count2_1 == 0){
        gain1 = 1;
    } else if ( count1_1 == 1 && count2_1 > 0)
    {
        gain1 = -1;
    }

    if( count2_2 > 1 && count1_2 == 0){
        gain2 = 1;
    } else if ( count2_2 == 1 && count1_2 > 0)
    {
        gain2 = -1;
    }
    
    return gain1 + gain2;
}


void build_final_arrays(){
    int i = 0;
    int continue_flag = 1;

    while(continue_flag == 1 && score < max_value*2) {
        continue_flag = 0;
        for(int j = 0; j< array_lenght; j++){
            int local_score = get_change_score(array1[j], array2[j]);
            if (local_score > 0) {
                score += local_score;
                int temp = array1[j];
                array1[j] = array2[j];
                array2[j] = temp;
                continue_flag = 1;
                array1_description[array1[j]-1]++;
                array2_description[array2[j]-1]++;
                array1_description[array2[j]-1]--;
                array2_description[array1[j]-1]--;
            }
        }
    }
}


void calculate_initial_score() {
    int nonzero_count1 = 0;
    int nonzero_count2 = 0;
    for (int i = 0; i < max_value; i++) {
        if (array1_description[i] != 0) {
            nonzero_count1++;
        }
        if (array2_description[i] != 0) {
            nonzero_count2++;
        }
    }
    score = nonzero_count1 + nonzero_count2;
}


int main(int argc, char *argv[]) {
        if(argc < 2 ) {
           return 1; 
        }else{
            array_lenght = atoi(argv[1]);
            max_value = 2* array_lenght;
        }

        array1 = (int *)malloc(array_lenght * sizeof(int)); // instead of sizeof(int) one can use less memory using max_value
        array2 = (int *)malloc(array_lenght * sizeof(int));
        if (array1 == NULL || array2 == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }

        for (int i = 0; i < array_lenght; i++) {
            array1[i] = atoi(argv[i + 2]);
        }

        for (int i = 0; i < array_lenght; i++) {
            array2[i] = atoi(argv[i + 2 + array_lenght]);
        }

        describe_arrays();
        calculate_initial_score();
        build_final_arrays();
        
        printf("Final Scole: %d\n", score);

        printf("array1: ");
        for (int i = 0; i < array_lenght && i < 20; i++) {
            printf("%d ", array1[i]);
        }
        printf("\n");

        printf("array2: ");
        for (int i = 0; i < array_lenght && i < 20; i++) {
            printf("%d ", array2[i]);
        }
        printf("\n");
}