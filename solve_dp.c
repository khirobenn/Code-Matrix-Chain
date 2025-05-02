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
    for(int i = 0; i<4; i++){
        for(int j = 0; j<4; j++){
            tab[i][j] = 0;
        }
    }
}

int main(){
    // test of the solution
    int ok[5] = {10, 100, 5, 50, 20};
    int tab[4][4];
    int split[4][4];

    init_matrix(4, tab);
    init_matrix(4, split);

    solution(4, tab, split, ok);

    print_matrix(4, tab);
    printf("------------------\n");
    print_matrix(4, split);
    // it works ;)
    
    return 0;
}