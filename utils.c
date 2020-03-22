#include "utils.h"

/*
 * This method calculates the distance between two vectors.
 * @params: The vectors to calculate the distance between
 * @return: Returns the distance as double value. -1.0 if an error occurred
 */
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

/*
 * This method prints the given vector formatted to be readable.
 * @param: the vector to print
 */
void vectorPrint(Vector v) {
    printf("{");
    for (int i = 0; i < v.n; ++i) {
        printf(" %.10lf", v.data[i]);
        if (i + 1 < v.n)
            printf(",");
    }
    printf("}\n");
}

/*
 * This method calculates the scalarproduct of the two given vectors. But one row might be skipped.
 * @param: v1, v2: pointers to the vectors to calculate the scalar of
 * @param: indexToIgnore: the index to not add to the scalar
 * @return: scalarproduct as double value
 */
double calculateScalar(Vector *v1, Vector *v2, int indexToIgnore) {
    // calculation of scalarproduct
    double scalar = 0;
    for (int i = 0; i < v2->n; i++) {
        if (i != indexToIgnore) {
            scalar += v1->data[i] * v2->data[i];
        }
    }
    return scalar;
}

/*
 * This method count the number of list elements
 * @param: startElement: Head of a chained list
 */
int getListLength(ListElement* startElement) {
  static int length = 0;
  if (startElement->next != NULL) {
    length++;
    return getListLength(startElement->next);
  } else {
    return length;
  }
}

/*
 * This method returns the last element of a chained list
 * @param: startElement: Head of a chained list
 */
ListElement* getLastListElement(ListElement* startElement) {
  if (startElement->next != NULL) {
    return getLastListElement(startElement->next);
  } else {
    return startElement;
  }
}

/*
 * This method converts chained lists into arrays
 * @param: vectorArray: Initialized array
 * @param: listStartElement: Head of a chained list
 * @param: length: Length of array
 */
void convertListToArray(Vector* vectorArray, ListElement* listStartElement,
                        int length) {
  ListElement* lastElement = getLastListElement(listStartElement);
  for (int i = 0; i < length; i++) {
    vectorArray[i].n = lastElement->vector.n;
    vectorArray[i].data = (lastElement->vector).data;

    lastElement = lastElement->last;
    free(lastElement->next);
  }
}
