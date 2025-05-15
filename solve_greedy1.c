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

// Shrinking 
void delete_the_iem_element(int tab[], int index, int size, char solution[][MAXREPR]){
    for(int i = index; i < size-1; i++){
        tab[i] = tab[i+1];
        sprintf(solution[i], "%s", solution[i+1]);
    }
}

long long greedy(int nb_of_matrices, int dimensions[], char solution[][MAXREPR]){
    if(nb_of_matrices == 2){
        char tmp[MAXREPR];
        sprintf(tmp, "(%s %s)", solution[0], solution[1]);
        sprintf(solution[0], "%s", tmp);
        return dimensions[0] * dimensions[1] * dimensions[2];
    }
    
    int index_to_delete = 1;
    long long sum_of_greedy = dimensions[0] * dimensions[1] * dimensions[2];
    for(int i = 2; i < nb_of_matrices; i++){
        long long tmp_sum = dimensions[i-1] * dimensions[i] * dimensions[i+1];
        if(tmp_sum < sum_of_greedy){
            sum_of_greedy = tmp_sum;
            index_to_delete = i;
        } 
    }
    char tmp[MAXREPR];
    sprintf(tmp, "(%s %s)", solution[index_to_delete-1], solution[index_to_delete]);
    sprintf(solution[index_to_delete-1], "%s", tmp);
    delete_the_iem_element(dimensions, index_to_delete, nb_of_matrices+1, solution);
    return sum_of_greedy + greedy(nb_of_matrices-1, dimensions, solution);
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
        bestCost = greedy(n, d, solution);

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