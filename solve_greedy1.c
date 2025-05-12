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
void delete_the_iem_element(long long tab[], int index, int size, char solution[][100]){
    for(int i = index; i < size-1; i++){
        tab[i] = tab[i+1];
        sprintf(solution[i], "%s", solution[i+1]);
    }
}

long long greedy(int nb_of_matrices, long long ok[], char solution[][100]){
    if(nb_of_matrices == 2){
        char tmp[100];
        sprintf(tmp, "(%s %s)", solution[0], solution[1]);
        sprintf(solution[0], "%s", tmp);
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
    char tmp[100];
    sprintf(tmp, "(%s %s)", solution[index_to_delete-1], solution[index_to_delete]);
    sprintf(solution[index_to_delete-1], "%s", tmp);
    delete_the_iem_element(ok, index_to_delete, nb_of_matrices+1, solution);
    return sum_of_greedy + greedy(nb_of_matrices-1, ok, solution);
}

#define SIZE 5
#define nb_matrices (SIZE-1)

int main(int argc, char *argv[]){
    long long ok[] = {10, 100, 5, 50, 20};
    print_array(ok, SIZE);
    char t[nb_matrices][100];
    for(int  i = 0; i<nb_matrices; i++){
        sprintf(t[i], "M%d", i+1);
    }
    
    printf("%lld\n", greedy(nb_matrices, ok, t));
    // t[0] will contain the solution after the call of greedy
    printf("%s\n", t[0]);

    return 0;
}