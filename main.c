#include <stdio.h>
#include <stdbool.h>

// Define everything needed
// Structs
typedef struct {
    int n;
    double **data;
} Matrix;

typedef struct {
    int n;
    double *data;
} Vector;

// Enum
typedef enum {
    JACOBI = 0, GAUSS_SEIDEL = 1
} Method;

// Methods
bool load (const char *filename, Matrix *A, Vector *b, Vector *x);
Vector* solve (Method method, Matrix *A, Vector* b, Vector *x, double e);

int main() {
    printf("Hello, World!\n");
    return 0;
}

bool load (const char *filename, Matrix *A, Vector *b, Vector *x){
    return false;
}

Vector* solve (Method method, Matrix *A, Vector* b, Vector *x, double e){
    return NULL;
}