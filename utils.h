#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

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
    JACOBI = 0,
    GAUSS_SEIDEL = 1
} Method;

typedef struct ListElement{
  Vector vector;
  struct ListElement* next;
  struct ListElement* last;
} ListElement;

// Functions
bool load(const char *filename, Matrix *A, Vector *b, Vector *x);
double vectorDistance(Vector v1, Vector v2);
void vectorPrint(Vector v);
int getListLength(ListElement* startElement);
void convertListToArray(Vector* vectorArray, ListElement* listStartElement,
                        int length);
ListElement* getLastListElement(ListElement* startElement);
