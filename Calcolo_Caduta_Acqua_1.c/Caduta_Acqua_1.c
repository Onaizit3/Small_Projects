
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


//Calcolare quanta aqua può essere raccolta in un array di altezze


int main()
{
    int n; // Numero effettivo di elementi
    int rain = 0;

    printf("Inserisci il numero di elementi: ");
    scanf("%d", &n);

    int X[n]; 

    printf("Inserisci %d numeri interi:\n", n);
    for(int i = 0; i < n; i++) {
        printf("Elemento %d: ", i+1);
        scanf("%d", &X[i]);
    }

    int max_value = X[0];
    for(int i = 1; i < n; i++) {
        if(X[i] > max_value) {
            max_value = X[i];
        }
    }

    int height = max_value;

    while (height > 0)
    {
        int flag = 0;
        int last_position = 0;
        int current_position = 0;

        while (current_position < n)
        {
            if (X[current_position] >= height)
            {
                if (flag == 1)
                {
                    rain += current_position - last_position - 1;
                }
                last_position = current_position;
                flag = 1;
            }
            current_position ++;
        }

        height --;
    }
    printf("La quantità d'acqua raccolta è: %d\n", rain);
    return 0;
}


