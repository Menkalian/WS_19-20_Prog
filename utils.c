#include "utils.h"

bool loadTest(const char *filename, Matrix *A, Vector *b, Vector *x) {
    // Fill in Test Values
    /*
     * TEST DATA:
     *  4,2,1,7
     *  2,4,1,7
     *  1,1,3,5
     *
     *  A = {{4,2,1},{2,4,1},{1,1,3}}
     *  b = {7,7,5}
     *  x = {0,0,0}
     *
     *  Lösung: {1,1,1}
     */
    double **mat = malloc(3 * sizeof(double *));
    double *r1 = malloc(3 * sizeof(double));
    double *r2 = malloc(3 * sizeof(double));
    double *r3 = malloc(3 * sizeof(double));
    mat[0] = r1;
    mat[1] = r2;
    mat[2] = r3;

    double *sol = malloc(3 * sizeof(double));
    double *start = malloc(3 * sizeof(double));

    // LGS
    mat[0][0] = 4;
    mat[0][1] = 2;
    mat[0][2] = 1;
    mat[1][0] = 2;
    mat[1][1] = 4;
    mat[1][2] = 1;
    mat[2][0] = 1;
    mat[2][1] = 1;
    mat[2][2] = 3;

    // rightside
    sol[0] = 7.0;
    sol[1] = 7.0;
    sol[2] = 5.0;

    // Start
    start[0] = 0.0;
    start[1] = 0.0;
    start[2] = 0.0;

    // Assign the values
    *A = (Matrix) {.n = 3, .data =  mat};
    *b = (Vector) {.n = 3, .data = sol};
    *x = (Vector) {.n = 3, .data = start};
    return true;
}

double vectorDistance(Vector v1, Vector v2) {
    if (v1.n != v2.n) {
        printf("Vektoren ungleicher Dimensionen erkannt! Entfernung kann nicht berechnet werden. Breche Berechnung ab!\n");
        return -1.0;
    }

    double distance = 0.0;
    // Calculate the Magnitude of the connecting vector
    for (int i = 0; i < v1.n; ++i) {
        double d = v1.data[i] - v2.data[i];
        distance += d * d;
    }

    return sqrt(distance);
}

bool vectorCompare(Vector* v1, Vector* v2){
    if(v1->n != v2->n)
        return false;
    bool toRet = true;
    for(int i = 0; i<v1->n; ++i){
        toRet = toRet && v1->data[i] == v2->data[i];
        if(!toRet) // Abort if already not equal
            return toRet;
    }
    return toRet;
}

void vectorPrint(Vector v) {
    printf("{");
    for (int i = 0; i < v.n; ++i) {
        printf(" %.8lf", v.data[i]);
        if (i + 1 < v.n )
            printf(",");
    }
    printf("}\n");
}
