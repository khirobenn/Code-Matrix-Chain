#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXSTACK 200
#define MAXREPR 500

typedef struct {
    int rows, cols;
    long long cost;    // 64-bit for safety
    char repr[MAXREPR];
} Node;

/* We'll store the current dimensions in d[] for the chain */
static int d[MAXN+1];
static int n; // number of matrices

void solution(int size, int (*dp)[size], int (*split)[size], const int* d){
    // initialise dp[i][i] to 0
    for(int i = 0; i<size; i++){
        dp[i][i] = 0;
    }

    // We will fill the matrix like this (diagonally) "x" is the box we fill for each iteration
    //--------------------
    // 0    x   .   .   .
    // .    0   x   .   .
    // .    .   0   x   .
    // .    .   .   0   x
    // .    .   .   .   0
    //--------------------
    /*
        Why we did that ? Because we can see that forAll i, j such that:
        i < j, we will need the values dp[i+k][j] and dp[i][j-k] to calculate dp[i][j]
    */
    for(int j = 1; j<size; j++){
        int pas = j;
        for(int i = 0; i<size-j; i++){
            int column_index = j + i; // to go through the diagonal
            int minimum = INT_MAX; // init minimum
            int k_minimum = INT_MAX; // init k-split minimum
            for(int k = 1; k <= pas; k++){
                int tmp = dp[i][column_index-k] + dp[i+pas-k+1][column_index] + d[i] * d[column_index-k+1] * d[column_index+1];
                if(tmp < minimum){
                    minimum = tmp;
                    k_minimum = column_index-k+1;
                }
            }
            dp[i][column_index] = minimum;
            split[i][column_index] = k_minimum;
        }
    }
}

void print_matrix(int size, int (*tab)[size]){
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            printf("%d\t", tab[i][j]);
        }
        printf("\n");
    }
}

void init_matrix(int size, int (*tab)[size]){
    for(int i = 0; i<size; i++){
        for(int j = 0; j<size; j++){
            tab[i][j] = 0;
        }
    }
}

void print_solution(int size, int (*split)[size], int i, int j, char *buff){
    if(i < 0 || j < 0 || j < i) return;
    if(i == j){
        char tmp[10];
        snprintf(tmp, 10, "M%d", i+1);
        strcat(buff, tmp);
    }
    else{
        strcat(buff, "(");
        print_solution(size, split, i, split[i][j]-1, buff);
        strcat(buff, " ");
        print_solution(size, split, split[i][j], j, buff);
        strcat(buff, ")");
    }
}

#define N 16
#define SIZE (N-1)

int main(){
    // test of the solution
    int dimensions[N] = {64, 51, 20, 39, 33, 98, 52, 76, 68, 14, 97, 62, 42, 81, 72, 47};
    int tab[SIZE][SIZE];
    int split[SIZE][SIZE];

    // Initialize tab and split matrices
    init_matrix(SIZE, tab);
    init_matrix(SIZE, split);

    // Fill tab with the minimal cost and split with the K where we split
    solution(SIZE, tab, split, dimensions);

    // Printing the solutions we get
    print_matrix(SIZE, tab);
    printf("------------------\n");
    print_matrix(SIZE, split);

    // We will store the solution here
    char sol[512];
    sol[0] = '\0';
    printf("%s\n", sol);

    // Always start the call at the index [0, size-1] it means we will get
    // where we have to split M1*M2*...*M(SIZE)
    print_solution(SIZE, split, 0, SIZE-1, sol);

    // printing the solution
    printf("%s\n", sol);
    return 0;
}