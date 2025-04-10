

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// Calculate how much water can be collected in an array of heights. The idea is to use local maximums and scan the array from the right with one index and from the left with another, stopping when the two are equal.


int* height_array; 
int n;
int max_height_left; 
int max_height_right;
int search_only_from_left = 0;
int search_only_from_right = 0;

typedef struct {
    int index;
    int water;
} CurrentStatus;


CurrentStatus partial_search_from_left( int bound_index, int start_index){

    int terrain_block = 0;
    CurrentStatus current_status;
    for ( int i = start_index + 1; i <= bound_index; i++ )
    {
        if(height_array[i] >= max_height_left)
        {
            current_status.index = i;
            current_status.water = max_height_left*(i - start_index - 1) - terrain_block;
            max_height_left = height_array[i];
            return current_status;
        }else{
            terrain_block += height_array[i];
        }
    }
    current_status.index = start_index;
    current_status.water = 0;
    return current_status;
}


CurrentStatus partial_search_from_right( int bound_index, int start_index){

    int terrain_block = 0;
    CurrentStatus current_status;
    for ( int i = start_index -1 ; i >= bound_index; i-- )
    {
        if(height_array[i] >= max_height_right)
        {
            current_status.index = i;
            current_status.water = max_height_right*( start_index -i - 1) - terrain_block;
            max_height_right = height_array[i];
            return current_status;
        }else{
            terrain_block += height_array[i];
        }
    }
    current_status.index = start_index;
    current_status.water = 0;
    return current_status;
}



int main()
{
    int rain = 0;

    printf("Inserisci il numero di elementi: ");
    scanf("%d", &n);

    height_array = (int*)malloc(n * sizeof(int));

    printf("Inserisci %d numeri interi:\n", n);
    for(int i = 0; i < n; i++) {
        printf("Elemento %d: ", i+1);
        scanf("%d", &height_array[i]);
    }

    CurrentStatus right = {n-1, 0};
    CurrentStatus left = {0, 0};
    
    max_height_left = height_array[0];
    max_height_right = height_array[n-1];

    int last_right_index = n-1;
    int last_left_index = 0;
    char side_method = 'L'; 

    while (left.index < right.index) {
        CurrentStatus *current;
        char current_side;
        
        // Determine which side to search from
        if (search_only_from_right) {
            current_side = 'R';
        } else if (search_only_from_left) {
            current_side = 'L';
        } else {
            current_side = side_method;
            side_method = (side_method == 'L') ? 'R' : 'L';
        }
        

        if (current_side == 'L') {
            left = partial_search_from_left(right.index, left.index);
            rain += left.water;
            printf("Water from left: %d\n", left.water);
        } else { 
            right = partial_search_from_right(left.index, right.index);
            rain += right.water;
            printf("Water from right: %d\n", right.water);
        }
    }

    printf("La quantità d'acqua raccolta è: %d\n", rain);

}