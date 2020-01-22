#include "utils.h"

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
        printf(" %.10lf", v.data[i]);
        if (i + 1 < v.n )
            printf(",");
    }
    printf("}\n");
}

double calculateScalar(Vector* vector, Vector* x, int withOut) {
  // Berechnung des Skalarprodukts
  double scalar = 0;
  for (int i = 0; i < x->n; i++) {
    if (i != withOut) {
      scalar += vector->data[i] * x->data[i];
    }
  }
  return scalar;
}
