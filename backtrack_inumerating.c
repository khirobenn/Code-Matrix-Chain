/********************************************************************
 * solve_backtracking.c
 *
 * Usage:
 *   ./solve_backtracking <inputFile> <outputFile>
 *
 * Description:
 *   Reads a set of matrix-chain instances from <inputFile>,
 *   enumerates all parenthesizations using a stack-based backtracking
 *   approach, finds the optimal cost and parenthesization, and
 *   writes results to <outputFile> with lines of the form:
 *
 *       n timeSeconds bestCost bestParenthesization
 *
 *   Prints progress on the number of instances solved.
 ********************************************************************/
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

// We will define a list to add each node
typedef struct element{
    Node * node;
    struct element * next;
}
element;

typedef struct {
    element * head;
    int length;
}
list;

// functions of the list
static list * create_list(){
    list * l = malloc(sizeof(list));
    l->head = NULL;
    l->length = 0;
    return l;
}

// This will add the element at it place sorted bt node.cost
static void add_element_to_list(list *l, element *el){
    if(l->head == NULL){
        l->head = el;
        el->next = NULL;
    }
    else if(l->head->node->cost > el->node->cost){
        el->next = l->head;
        l->head = el;
    }
    else{
        element *tmp = l->head;
        element *tmp2 = tmp->next;
        while(tmp2 != NULL && tmp2->node->cost < el->node->cost){
            tmp = tmp2;
            tmp2 = tmp2->next;
        }

        if(tmp2 == NULL){
            el->next == NULL;
            tmp->next = el;
        }else{
            el->next = tmp2;
            tmp->next = el;
        }
    }
    l->length++;
}

static void free_list(list *l){
    element * t = l->head;
    while(t != NULL){
        element *tmp = t;
        t = t->next;
        tmp->next = NULL;
        free(tmp->node);
        free(tmp);
    }
    free(l);
}

static void display_cost_list(list *l){
    element * t = l->head;
    int i = 1;
    while(t != NULL){
        printf("*************************************************************\n");
        printf("Rank : %d\n", i++);
        printf("%s\n", t->node->repr);
        printf("%lld\n\n", t->node->cost);

        t = t->next;
    }
}

static int get_rank(list *l, long long cost){
    if(l == NULL) return -1;
    int i = 1;
    element * e = l->head;
    while(e != NULL && e->node->cost != cost){
        i++;
        e = e->next;
    }
    return i;
}

/* Global stack */
static Node stack[MAXSTACK];
static int top = 0; // stack pointer

/* Best solution for current instance */
static long long bestCost;
static char bestTree[MAXREPR];

/* We'll store the current dimensions in d[] for the chain */
static int d[MAXN+1];
static int n; // number of matrices

/* -------------- Stack operations -------------- */
static void push(Node nd)
{
    if (top >= MAXSTACK) {
        fprintf(stderr, "Stack overflow.\n");
        exit(1);
    }
    stack[top++] = nd;
}

static Node pop(void)
{
    if (top <= 0) {
        fprintf(stderr, "Stack underflow.\n");
        exit(1);
    }
    return stack[--top];
}

/* -------------- Leaf creation -------------- */
static Node makeLeaf(int i)
{
    Node leaf;
    leaf.rows = d[i-1];
    leaf.cols = d[i];
    leaf.cost = 0LL;
    snprintf(leaf.repr, sizeof(leaf.repr), "M%d", i);
    return leaf;
}

/* -------------- Combine two sub-products -------------- */
static Node combineNodes(const Node *A, const Node *B)
{
    Node C;
    long long combineCost = (long long)A->rows * A->cols * B->cols;
    C.rows = A->rows;
    C.cols = B->cols;
    C.cost = A->cost + B->cost + combineCost;
    snprintf(C.repr, sizeof(C.repr), "(%s %s)", A->repr, B->repr);
    return C;
}

/* -------------- Backtracking -------------- */
static void backtrack(int i, list *l)
{
    /* Case 1: SHIFT if i <= n */
    if (i <= n) {
        Node leaf = makeLeaf(i);
        push(leaf);
        backtrack(i + 1, l);
        pop(); // undo SHIFT
    }

    /* Case 2: REDUCE if top >= 2 */
    if (top >= 2) {
        Node B = pop();
        Node A = pop();
        Node C = combineNodes(&A, &B);

        push(C);
        backtrack(i, l);
        pop(); // undo REDUCE

        // restore
        push(A);
        push(B);
    }

    /* Case 3: check if complete parse => i>n & top==1 */
    if (i > n && top == 1) {
        Node candidate = stack[top-1];
        Node * n = malloc(sizeof(Node));
        n->cost = candidate.cost;
        strcpy(n->repr, candidate.repr);
        n->rows = candidate.rows;
        n->cols = candidate.cols;

        element * el = malloc(sizeof(element));
        el->node = n;
        el->next = NULL;

        add_element_to_list(l, el);
        if (candidate.cost < bestCost) {
            bestCost = candidate.cost;
            strcpy(bestTree, candidate.repr);
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage : %s matrix\n", argv[0]);
        exit(1);
    }
    const char *inFile  = argv[1];

    char *greedy[3] = {
        "greedy1",
        "greedy2",
        "my greedy"
    };

    FILE *files[3];
    files[0] = fopen("solutions/greedy1_solution", "r");
    files[1] = fopen("solutions/greedy2_solution", "r");
    files[2] = fopen("solutions/my_greedy_solution", "r");

    FILE *fin = fopen(inFile, "r");
    if (!fin) {
        fprintf(stderr, "Cannot open input file: %s\n", inFile);
        return 1;
    }

    // Read total number of instances T
    int T;
    fscanf(fin, "%d", &T);
    // We will read only 8 arrays of dimensions
    T = 8;

    // We'll solve each instance, measure time, and record results.
    for (int inst = 0; inst < T; inst++) {
        list * l = create_list();
        // read n
        fscanf(fin, "%d", &n);
        // read n+1 dimensions
        for (int i = 0; i < n+1; i++) {
            fscanf(fin, "%d", &d[i]);
        }

        // Reset global best
        bestCost = LLONG_MAX;
        strcpy(bestTree, "");

        // measure time
        clock_t start = clock();

        // Reset stack
        top = 0;
        // run backtracking
        backtrack(1, l);

        clock_t end = clock();
        double timeSec = (double)(end - start) / CLOCKS_PER_SEC;
        // display_cost_list(l);

        printf("RANK & GAP **********************************************************\n");

        // We will get the rank for each greedy here
        for(int i = 0; i<3; i++){
            int k;
            double time_sec;
            long long cost;
            char rep[MAXREPR];
            fscanf(files[i], "%d %lf %lld %[^\n]", &k, &time_sec, &cost, rep);
            int rank = get_rank(l, cost);
            printf("-----------------------------------------------------------------\n");
            printf("- %s :\n", greedy[i]);
            printf("Rep : %s\n", rep);
            printf("Cost = %d,\t Rank = %d,\t Rank Gap = %d,\t Gap Average : %lf%%\n", cost, rank, rank-1, ((rank-1)*1.0)/l->length);
        }
        printf("-----------------------------------------------------------------\n");
        
        free_list(l);
        // Print progress
        printf("[Progress] Solved instance %d / %d : n=%d, time=%.6f, bestCost=%lld\n",
            inst+1, T, n, timeSec, bestCost);
        printf("\n\n\n");
    }


    // close files opened
    for(int i = 0; i<3; i++){
        fclose(files[i]);
    }

    fclose(fin);
    
    return 0;
}
