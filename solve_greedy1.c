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

void print_array(long long *ok, int size){
    for(int i = 0; i<size; i++){
        printf("%d\t", ok[i]);
    }
    printf("\n");
}

// Shrinking 
void delete_the_iem_element(long long tab[], int index, int size){
    for(int i = index; i < size-1; i++){
        tab[i] = tab[i+1];
    }
}

long long greedy(int nb_of_matrices, long long ok[]){
    if(nb_of_matrices == 2){
        return ok[0] * ok[1] * ok[2];
    }
    
    int index_to_delete = 1;
    long long sum_of_greedy = ok[0] * ok[1] * ok[2];
    for(int i = 2; i < nb_of_matrices; i++){
        long long tmp_sum = ok[i-1] * ok[i] * ok[i+1];
        if(tmp_sum < sum_of_greedy){
            sum_of_greedy = tmp_sum;
            index_to_delete = i;
        } 
    }

    delete_the_iem_element(ok, index_to_delete, nb_of_matrices+1);
    print_array(ok, nb_of_matrices);
    return sum_of_greedy + greedy(nb_of_matrices-1, ok);
}

#define SIZE 5
#define nb_matrices (SIZE-1)

int main(int argc, char *argv[]){
    long long ok[] = {10, 100, 5, 50, 20};
    print_array(ok, SIZE);
    printf("%lld\n", greedy(nb_matrices, ok));

    return 0;
}