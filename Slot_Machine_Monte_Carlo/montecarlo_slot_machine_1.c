
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NUM_SIMULATIONS 1000000
#define SLOT_MACHINE_PRICE 1

char get_random_symbol() {
    int r = rand() % 100;
    if (r < 30) return 'A';
    else if (r < 55) return 'B';
    else if (r < 75) return 'C';
    else if (r < 90) return 'D';
    else return 'E';
}

int get_payout(char s1, char s2, char s3) {
    if (s1 == s2 && s2 == s3) {
        switch (s1) {
            case 'A': return 100 - SLOT_MACHINE_PRICE; 
            case 'B': return 40 - SLOT_MACHINE_PRICE;
            case 'C': return 20 - SLOT_MACHINE_PRICE;
            case 'D': return 10 - SLOT_MACHINE_PRICE;
            case 'E': return 5 - SLOT_MACHINE_PRICE;
        }
    } else if (s1 == s2 || s1 == s3 || s2 == s3) {
        return 2;
    }
    return 0 - SLOT_MACHINE_PRICE;
}

int make_simulation(int number_attempts){
    int total_reslut = 0;
    int total_result_squared = 0;
    for( int i = 0; i < number_attempts; i++){
        int result_1 = get_random_symbol();
        int result_2 = get_random_symbol();
        int result_3 = get_random_symbol();
        int result = get_payout(result_1, result_2, result_3);
        total_reslut += result;
        total_result_squared += result * result;
    }
    double mean = (double)total_reslut / number_attempts;
    double variance = ((double)total_result_squared / number_attempts) - (mean * mean);
    double std_deviation = sqrt(variance);
    printf("Casino profit average: %f\n", mean);
    printf("Variance: %f\n", variance);
    printf("Standard deviation: %f\n", std_deviation);
}



int main() {
    srand(time(NULL));
    int number_of_simulations = 15000;
    make_simulation(number_of_simulations);

    return 0;
}
