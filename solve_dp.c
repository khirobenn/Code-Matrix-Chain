#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAXN 100
#define MAXSTACK 200
#define MAXREPR 500

/* Best solution for current instance */
static long long bestCost;
static char bestTree[MAXREPR];

/* We'll store the current dimensions in d[] for the chain */
static int d[MAXN+1];
static int n; // number of matrices

void solution(int size, long long (*dp)[size], int (*split)[size], const int* d){
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
            long long minimum = LONG_MAX; // init minimum
            int k_minimum = INT_MAX; // init k-split minimum
            for(int k = 1; k <= pas; k++){
                long long tmp = dp[i][column_index-k] + dp[i+pas-k+1][column_index] + d[i] * d[column_index-k+1] * d[column_index+1];
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
        char tmp[15];
        snprintf(tmp, 15, "M%d", i+1);
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

int main(int argc, char *argv[]){
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <inputFile> <outputFile>\n", argv[0]);
        return 1;
    }
    const char *inFile  = argv[1];
    const char *outFile = argv[2];

    FILE *fin = fopen(inFile, "r");
    if (!fin) {
        fprintf(stderr, "Cannot open input file: %s\n", inFile);
        return 1;
    }
    FILE *fout = fopen(outFile, "w");
    if (!fout) {
        fprintf(stderr, "Cannot open output file: %s\n", outFile);
        fclose(fin);
        return 1;
    }

    // Read total number of instances T
    int T;
    fscanf(fin, "%d", &T);

    // We'll solve each instance, measure time, and record results.
    for (int inst = 0; inst < T; inst++) {
        // read n
        fscanf(fin, "%d", &n);
        // read n+1 dimensions
        for (int i = 0; i < n+1; i++) {
            fscanf(fin, "%d", &d[i]);
        }

        bestCost = LLONG_MAX;
        bestTree[0] = '\0';

        // init dp and split matrices
        long long dp[n][n];
        int split[n][n];
        // init_matrix(n, dp);
        init_matrix(n, split);

        // measure time
        clock_t start = clock();
        
        // run dp solution and storing the best solution in bestTree
        solution(n, dp, split, d);
        print_solution(n, split, 0, n-1, bestTree);

        clock_t end = clock();

        bestCost = dp[0][n-1];


        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;

        // Write result: n time bestCost bestTree
        fprintf(fout, "%d %.6f %lld %s\n", n, timeSec, bestCost, bestTree);
        fflush(fout);

        // Print progress
        printf("[Progress] Solved instance %d / %d : n=%d, time=%.6f, bestCost=%lld\n",
               inst+1, T, n, timeSec, bestCost);
    }

    fclose(fin);
    fclose(fout);
    printf("Processed %d instances. Results saved to %s\n", T, outFile);
    return 0;
}