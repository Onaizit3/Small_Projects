// Problem in https://codeforces.com/problemset/problem/2086/D
// Compiler: GCC 14.0

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ALPHABET_SIZE 26
#define TEST_ID        5  // choose which test array to load (1..5)

typedef long long ll;
static const ll MOD = 998244353;

// --- global variables ---
static int letterCounts[ALPHABET_SIZE];  // how many times each letter appears
static int maxIndex;                     // highest index with count > 0
static int totalLetters;                 // sum of all letterCounts[]
static int halfLetters;                  // floor(totalLetters / 2)
static ll *factorial;                    // factorial[i] = i! % MOD

// --- test arrays (expected results in comments) ---
static const int testArray1[ALPHABET_SIZE] = {
    2,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
}; // should result in 4

static const int testArray2[ALPHABET_SIZE] = {
    3,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0
}; // should result in 960

static const int testArray3[ALPHABET_SIZE] = {
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,3,0,0,0,0,0,0,0,0,0,0,0
}; // should result in 0

static const int testArray4[ALPHABET_SIZE] = {
    1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
}; // should result in 1

static const int testArray5[ALPHABET_SIZE] = {
    1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,233527,233827
}; // should result in 789493841

// --- function prototypes ---
void    loadTestData(void);
void    sortDescending(int *arr, int size);
int     findMaxIndex(const int *arr, int size);
ll      modPow(ll base, ll exp);
ll      modInverse(ll a);
void    computeFactorials(void);
ll      computeMultinomial(const int *arr);
ll      computeSplitWays(const bool selected[]);
ll      countEven(int idx, int sumA, int sumB, bool selected[]);
ll      countOdd (int idx, int sumA, int sumB, bool selected[]);

int main(void) {
    clock_t start = clock();

    loadTestData();                                   // load chosen test data
    sortDescending(letterCounts, ALPHABET_SIZE);      // sort counts descending
    maxIndex     = findMaxIndex(letterCounts, ALPHABET_SIZE);

    // compute totalLetters and prepare factorial[]
    totalLetters = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        totalLetters += letterCounts[i];
    }
    factorial = malloc((totalLetters + 1) * sizeof(ll));
    if (!factorial) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    computeFactorials();

    halfLetters = totalLetters / 2;

    // selection mask for group A (true) / B (false)
    bool selected[ALPHABET_SIZE] = { false };
    selected[0] = true;  // fix the first letter in A to avoid symmetric duplicates

    ll result;
    if (totalLetters % 2 == 0) {
        // even totalLetters: both halves of length halfLetters
        result = (2 * countEven(1, letterCounts[0], 0, selected)) % MOD;
    } else {
        // odd totalLetters: halves of length floor(n/2) and ceil(n/2)
        result = countOdd(1, letterCounts[0], 0, selected);
    }

    printf("%lld\n", result);

    clock_t end = clock();
    fprintf(stderr, "CPU time: %.6f s\n",
            (double)(end - start) / CLOCKS_PER_SEC);

    free(factorial);
    return EXIT_SUCCESS;
}

// ------------------------------------------------------------
// Load one of the predefined test arrays into letterCounts[].
// Replace with reading from stdin if needed.
void loadTestData(void) {
    switch (TEST_ID) {
        case 1:
            memcpy(letterCounts, testArray1, sizeof testArray1);
            break;
        case 2:
            memcpy(letterCounts, testArray2, sizeof testArray2);
            break;
        case 3:
            memcpy(letterCounts, testArray3, sizeof testArray3);
            break;
        case 4:
            memcpy(letterCounts, testArray4, sizeof testArray4);
            break;
        case 5:
            memcpy(letterCounts, testArray5, sizeof testArray5);
            break;
        default:
            fprintf(stderr, "Invalid TEST_ID %d\n", TEST_ID);
            exit(EXIT_FAILURE);
    }
}

// compare function for qsort (descending order)
int compareDesc(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

// sort arr[0..size) in-place in descending order
void sortDescending(int *arr, int size) {
    qsort(arr, size, sizeof(int), compareDesc);
}

// return the highest index i where arr[i] > 0
int findMaxIndex(const int *arr, int size) {
    for (int i = size - 1; i >= 0; i--) {
        if (arr[i] > 0) return i;
    }
    return 0;
}

// fast power mod: base^exp % MOD
ll modPow(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return res;
}

// modular inverse via Fermat's little theorem
ll modInverse(ll a) {
    return modPow(a, MOD - 2);
}

// fill factorial[i] = i! % MOD for i = 0..totalLetters
void computeFactorials(void) {
    factorial[0] = 1;
    for (int i = 1; i <= totalLetters; i++) {
        factorial[i] = (factorial[i - 1] * i) % MOD;
    }
}

// multinomial coefficient: (sum arr)! / product(arr[i]!) mod MOD
ll computeMultinomial(const int *arr) {
    int sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sum += arr[i];
    }
    ll res = factorial[sum];
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (arr[i] > 1) {
            res = (res * modInverse(factorial[arr[i]])) % MOD;
        }
    }
    return res;
}

// given selection mask, compute (#permutations in A) * (#permutations in B) % MOD
ll computeSplitWays(const bool selected[]) {
    int groupA[ALPHABET_SIZE] = {0}, groupB[ALPHABET_SIZE] = {0};
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (selected[i]) groupA[i] = letterCounts[i];
        else             groupB[i] = letterCounts[i];
    }
    return (computeMultinomial(groupA) * computeMultinomial(groupB)) % MOD;
}

// recursive counting when totalLetters is even
ll countEven(int idx, int sumA, int sumB, bool selected[]) {
    if (sumA > halfLetters || sumB > halfLetters) return 0;

    if (sumA == halfLetters) {
        return computeSplitWays(selected);
    }
    if (sumB == halfLetters) {
        for (int i = idx; i <= maxIndex; i++) selected[i] = true;
        ll res = computeSplitWays(selected);
        for (int i = idx; i <= maxIndex; i++) selected[i] = false;
        return res;
    }
    if (idx > maxIndex) return 0;

    selected[idx] = true;
    ll res = countEven(idx + 1, sumA + letterCounts[idx], sumB, selected);
    selected[idx] = false;

    res = (res + countEven(idx + 1, sumA, sumB + letterCounts[idx], selected)) % MOD;
    return res;
}

// recursive counting when totalLetters is odd
ll countOdd(int idx, int sumA, int sumB, bool selected[]) {
    int low  = halfLetters;      // floor(n/2)
    int high = halfLetters + 1;  // ceil(n/2)

    if (sumA > high || sumB > high) return 0;

    if (sumA == high) {
        return computeSplitWays(selected);
    }
    if (sumB == high) {
        for (int i = idx; i <= maxIndex; i++) selected[i] = true;
        ll res = computeSplitWays(selected);
        for (int i = idx; i <= maxIndex; i++) selected[i] = false;
        return res;
    }
    if (sumA == low) {
        return computeSplitWays(selected);
    }
    if (sumB == low) {
        for (int i = idx; i <= maxIndex; i++) selected[i] = true;
        ll res = computeSplitWays(selected);
        for (int i = idx; i <= maxIndex; i++) selected[i] = false;
        return res;
    }
    if (idx > maxIndex) return 0;

    selected[idx] = true;
    ll res = countOdd(idx + 1, sumA + letterCounts[idx], sumB, selected);
    selected[idx] = false;

    res = (res + countOdd(idx + 1, sumA, sumB + letterCounts[idx], selected)) % MOD;
    return res;
}