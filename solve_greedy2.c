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

long long greedy(long long ok[], char solution[][100], int start, int end){
    if(end - start < 2){
        // càd y'a une seule matrice, y'a pas de coût à calculer.
        return 0;
    }
    else if(end - start == 2){
        // càd y'a deux matrices, y'a un coût à calculer.
        char tmp[100];
        sprintf(tmp, "(%s %s)", solution[start], solution[start+1]);
        sprintf(solution[start], "%s", tmp);
        return ok[start] * ok[start+1] * ok[end];
    }
    
    // On cherche le minimum entre start et end
    // Donc c'est pas la peine de faire la multiplication
    int index_to_delete = start+1;
    long long sum_of_greedy = ok[start+1];
    for(int i = start+2; i < end; i++){
        if(ok[i] < sum_of_greedy){
            sum_of_greedy = ok[i];
            index_to_delete = i;
        } 
    }
    sum_of_greedy *= ok[start];
    sum_of_greedy *= ok[end];
    // à gauche
    long long b = greedy(ok, solution, start, index_to_delete);
    // à droite
    long long c = greedy(ok, solution, index_to_delete, end);

    // On concate les deux solutions, 
    // on les mets dans solution[start] pour avoir la solution finale dans solution[0]
    char tmp[100];
    sprintf(tmp, "(%s %s)", solution[start], solution[index_to_delete]);
    sprintf(solution[start], "%s", tmp);
    printf("%s\n", solution[start]);
    return sum_of_greedy+b+c;
}

#define SIZE 5
#define nb_matrices (SIZE-1)

int main(int argc, char *argv[]){
    // long long ok[] = {10, 100, 5, 50, 20};
    long long ok[] = {5, 100, 3, 2, 50};
    print_array(ok, SIZE);
    char t[nb_matrices][100];
    for(int  i = 0; i<nb_matrices; i++){
        sprintf(t[i], "M%d", i+1);
    }
    
    printf("%lld\n", greedy(ok, t, 0, nb_matrices));
    // t[0] will contain the solution after the call of greedy
    printf("%s\n", t[0]);

    return 0;
}