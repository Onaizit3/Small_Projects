#include <stdio.h>
#include <stdlib.h> // Per atoi
    #define min(a, b) ((a) < (b) ? (a) : (b))
    #define max(a, b) ((a) > (b) ? (a) : (b))

int k;
int a;
int b;
int x;
int y;

int main(int argc, char *argv[]) {
    
    if(argc < 6 ) {
        printf("Wrong input, the input must be 5 arguments\n");
        return 1;
    }else{
        k = atoi(argv[1]);
        a = atoi(argv[2]);
        b = atoi(argv[3]);
        x = atoi(argv[4]);
        y = atoi(argv[5]);
    }

    int z_min = min(x, y);
    int z_max = max(x, y);
    int c_min;
    int c_max;

    if (z_min == x)
    {
        c_min = a;
        c_max = b;
    }else if (z_min == y)
    {
        c_min = b;
        c_max = a;
    }else
    {
        printf("Error not recognized\n");
        return 1;
    }

    if (c_min > k || c_max > k)
    {
        int solution = 0;
        printf("Solution is: %d\n", solution);
        return 0;
    }

    int num1 = (k - c_min ) /z_min + 1;
    int residue = k - z_min*num1;
    printf("num1 is: %d\n", num1);
    printf("residue is: %d\n", residue);
    printf("c_max is: %d\n", c_max);
    if (residue < c_max)
    {
        int solution = num1;
        printf("Solution is: %d\n", solution);
        return 0;
    }
    int num2 = ( residue - c_max )/z_max + 1;

    int solution = num1 + num2;
    printf("Solution is: %d\n", solution);
}