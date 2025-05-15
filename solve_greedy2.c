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

long long greedy(int dimensions[], char solution[][MAXREPR], int start, int end){
    if(end - start < 2){
        // it means there is only one matrix, there is no cost to calculate.
        return 0;
    }
    else if(end - start == 2){
        // it means there is two matrices, so, there is a cost to calculate.
        char tmp[MAXREPR];
        sprintf(tmp, "(%s %s)", solution[start], solution[start+1]);
        sprintf(solution[start], "%s", tmp);
        return dimensions[start] * dimensions[start+1] * dimensions[end];
    }
    // We are searching for a min between start and end
    // We don't need a multiplication for that
    // for example if a*x*c < a*y*c <=> x < y, such that numbers are greater than 0
    int index_to_delete = start+1;
    long long sum_of_greedy = dimensions[start+1];
    for(int i = start+2; i < end; i++){
        if(dimensions[i] < sum_of_greedy){
            sum_of_greedy = dimensions[i];
            index_to_delete = i;
        } 
    }
    sum_of_greedy *= dimensions[start];
    sum_of_greedy *= dimensions[end];
    // left call
    long long b = greedy(dimensions, solution, start, index_to_delete);
    // right call
    long long c = greedy(dimensions, solution, index_to_delete, end);

    // We concatenate the two solutions of left and right
    // We give it to solution[start] to have our final solution in solution[0]
    char tmp[MAXREPR];
    sprintf(tmp, "(%s %s)", solution[start], solution[index_to_delete]);
    sprintf(solution[start], "%s", tmp);
    return sum_of_greedy+b+c;
}

#define SIZE 5
#define nb_matrices (SIZE-1)

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
        // solution is an array of strings to write M1, M2, ...
        char solution[n][MAXREPR];
        for(int  i = 0; i<n; i++){
            sprintf(solution[i], "M%d", i+1);
        }

        bestCost = LLONG_MAX;
        bestTree[0] = '\0';

        // measure time
        clock_t start = clock();
        
        // call greedy function that will write the final solution in solution[0]
        bestCost = greedy(d, solution, 0, n);

        clock_t end = clock();
        
        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;

        // Write result: n time bestCost bestTree
        fprintf(fout, "%d %.6f %lld %s\n", n, timeSec, bestCost, solution[0]);
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