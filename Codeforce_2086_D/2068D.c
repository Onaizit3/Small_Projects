
// Problem in https://codeforces.com/problemset/problem/2086/D
// Compiler: GCC 14.0


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//l'array con le quantita' di lettere
int array[26]; //array con le quantita' di lettere
int n = 0; //numero di lettere
int n_fract = 0; //numero di lettere diviso 2 approssimato per difetto
int max_index = 0; //indice dell'array se non avesse zeri
const long long MOD = 998244353;
int *factorial; //una puntatore ad un array che tiene in memoria i fattoriali

//index_array[i] l'array con booleani che indica se l'elemento i-esimo e' stato posto nel subarray di destra
//dx_int e' la somma attuale delle lettere usate nel primo subarray , serve per stoppare anticipatamente la coda
//sx_int e' la somma attuale delle lettere usate nel secondo subarray, serve per stoppare anticipatamente la coda

//array di test
int test_array_1[26] = {2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // deve risultare 4
int test_array_2[26] = {3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0}; //deve risultare 960
int test_array_3[26] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //deve risultare 0
int test_array_4[26] = {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //deve risultare 1
int test_array_5[26] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 233527, 233827}; //deve risultare 789493841


int getArray(){
    memcpy(array, test_array_5, sizeof(array));
    return 0;
}

int getMaxIndex(){
    for (int i = 1; i < 26; i++) {
        if (array[i] == 0) {
            return 0;
        }else{
            max_index = i;
        }
    }
}


//------------------- Sort Descending -------------------

// Struct to store the count and original index
typedef struct {
    int count;
    int index;
} CountIndex;

// Comparison function for qsort (descending order)
int compareDescending(const void *a, const void *b) {
    CountIndex *countA = (CountIndex *)a;
    CountIndex *countB = (CountIndex *)b;
    
    // Sort in descending order
    return countB->count - countA->count;
}

// Function to sort array in descending order while preserving letter-to-count mapping
void sortArrayDescending() {
    CountIndex temp[26];
    int sorted_array[26];
    int sorted_indices[26];
    
    // Create temporary array with count and index
    for (int i = 0; i < 26; i++) {
        temp[i].count = array[i];
        temp[i].index = i;
    }
    
    // Sort the temporary array
    qsort(temp, 26, sizeof(CountIndex), compareDescending);
    
    // Extract sorted array and indices
    for (int i = 0; i < 26; i++) {
        sorted_array[i] = temp[i].count;
        sorted_indices[i] = temp[i].index;
    }
    
    // Copy the sorted values back to the original array
    for (int i = 0; i < 26; i++) {
        array[i] = sorted_array[i];
    }
}


//------------------- Calculate Combination -------------------


// Calculate modular inverse of a number using Fermat's little theorem
long long modInverse(long long a, long long m) {
    long long result = 1;
    long long power = m - 2;
    a = a % m;
    
    while (power > 0) {
        if (power % 2 == 1) {
            result = (result * a) % m;
        }
        a = (a * a) % m;
        power /= 2;
    }
    
    return result;
}

void calculateFactorial() {
    // Allocate memory for the factorial array
    factorial = (int *)malloc((n + 2) * sizeof(long long));
    if (!factorial) {
        printf("Memory allocation failed\n");
        return;
    }
    
    // Calculate factorial values and store them in the array
    factorial[0] = 1;
    for (int i = 1; i <= n; i++) {
        // Use long long for intermediate calculation to prevent overflow
        factorial[i] = (1LL * factorial[i-1] * i) % MOD;
    }
}

// Calculate the multinomial coefficient modulo MOD
long long calculateCombinationsModulo(int* arr) {
    
    // Calculate total length
    int totalLength = 0;
    for (int i = 0; i < 26; i++) {
        totalLength += arr[i];
    }
    
    // Calculate multinomial coefficient
    long long result = factorial[totalLength];
    for (int i = 0; i < 26; i++) {
        if (arr[i] > 0) {
            result = (result * modInverse(factorial[arr[i]], MOD)) % MOD;
        }
    }
    
    return result;
}



long long calculateCombinations(int* index_array){
    int filtered_array_dx[26] = {0};
    int filtered_array_sx[26] = {0};     
    for (int i = 0; i < 26; i++) {
        if (index_array[i] == 1) {
            filtered_array_dx[i] = array[i];
        }else{
            filtered_array_sx[i] = array[i];
        }
    }
    long long combination = calculateCombinationsModulo(filtered_array_dx)* calculateCombinationsModulo(filtered_array_sx) % MOD;
    return combination;
}


//------------------- Compute Cases -------------------


long long processArray_for_n_even(int* index_array, int index, int dx_int, int sx_int) {

    if(dx_int > n_fract){ 
        return 0;
    } 
    if(sx_int > n_fract){ 
        return 0;
    }

    if(dx_int == n_fract){
        return calculateCombinations(index_array) ;}
    if(sx_int == n_fract){
        //@todo cambiare struttura per rendere piu comprensibile, 
        for (int i = index; i <= max_index; i++) {
            index_array[i] = 1;
        }
        long long result_1 =  calculateCombinations(index_array) ;
        for (int i = index; i <= max_index; i++) {
            index_array[i] = 0;
        }
        return result_1;
    }
    if(array[index] == 0){ 
        return 0;
    }
    if(index > 25){
        return 0;
    }

    index_array[index] = 1; // Indica che l'elemento i-esimo e' stato usato
    long long right_Result = processArray_for_n_even( index_array, index + 1, dx_int + array[index], sx_int) % MOD; //ramo destro
    index_array[index] = 0; // Indica che l'elemento i-esimo non e' stato usato 
    long long left_Result = processArray_for_n_even( index_array, index + 1, dx_int, sx_int + array[index] ) % MOD; //ramo sinistro

    long long result = (right_Result + left_Result) % MOD;

    return result;
}

long long processArray_for_n_odd(int* index_array, int index, int dx_int, int sx_int) {

    if(dx_int > n_fract +1){ return 0;} // cambiare il +1 per evitare doppioni
    if(sx_int > n_fract +1){ return 0;}

    if(dx_int == n_fract +1){ 
        return calculateCombinations(index_array);
    }
    if(sx_int == n_fract +1){
        for (int i = index; i <= max_index; i++) {
            index_array[i] = 1;
        }
        long long result_1 =  calculateCombinations(index_array) ;
        for (int i = index; i <= max_index; i++) {
            index_array[i] = 0;
        }
        return result_1;
    }
    if(dx_int == n_fract){
        return calculateCombinations(index_array);
    }
    if(sx_int == n_fract){
        for (int i = index; i <= max_index; i++) {
            index_array[i] = 1;
        }
        long long result_1 =  calculateCombinations(index_array) ;
        for (int i = index; i <= max_index; i++) {
            index_array[i] = 0;
        }
        return result_1;
    }
    if(array[index] == 0){ return 0;}
    if(index > 25){return 0;} 

    index_array[index] = 1; // Indica che l'elemento i-esimo e' stato usato
    long long right_Result = processArray_for_n_odd( index_array, index + 1, dx_int + array[index], sx_int) % MOD; //ramo destro
    index_array[index] = 0; // Indica che l'elemento i-esimo non e' stato usato
    long long left_Result = processArray_for_n_odd( index_array, index + 1, dx_int, sx_int + array[index] ) % MOD; //ramo sinistro 

    return (right_Result + left_Result) % MOD;
}


//------------------- Main Function -------------------


int main() {
    clock_t start_time = clock();

    //Definizione Array
    
    getArray();
    sortArrayDescending();
    getMaxIndex();
    // Calculate the total number of letters
    for (int i = 0; i < 26; i++) {
        n += array[i];
    }
    calculateFactorial();

    printf("n Vale:  %d\n", n);
    n_fract = n / 2;
    printf("n_frac Vale:  %d\n", n_fract);

    int index_array[26] = {0};
    index_array[0] = 1; // Per evitare doppioni si impone che le 'a' stiano tutte nello stesso subarray
    long long res;
    if (n % 2 == 0) {
        // n is even
        printf("n is even\n"); 
        res = 2 * processArray_for_n_even(index_array, 1, array[0], 0) % MOD; ;
    } else {
        // n is odd
        printf("n is odd\n");
        res = processArray_for_n_odd(index_array, 1, array[0], 0);
    }
    printf("Casi: %lld\n", res);
      
    // Calculate and print execution time
    clock_t end_time = clock();
    double execution_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.6f seconds\n", execution_time);
    return 0;
}





