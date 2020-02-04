#include "utils.h"

// Solving Methods
Vector *solve(Method method, Matrix *A, Vector *b, Vector *x, double e);
void solveJ(Matrix *A, Vector *b, Vector *newX, Vector *oldX);  // JACOBI
void solveGS(Matrix *A, Vector *b, Vector *x, Vector *oldX); // GAUSS_SEIDEL
void calculateLGS(Matrix* A, Vector* b, Vector* x, double e, void (*f)(Matrix*, Vector*, Vector*, Vector*));
double calculateScalar(Vector* vector, Vector* x, int withOut);

int main() {
    // Init Variables
    char filename[260];// = malloc(260 * sizeof(char));    // Maximale Pfadlänge in Windows ist 260.
    bzero(&filename, 260);
    Matrix A;             // A is a pointer to the values of the SLE (System of linear equations)
    Vector b;             // b is a pointer to the right side of the SLE (System of linear equations)
    Vector x;             // x is a pointer to the start values
    Method method = 0;
    double margin = -1.0; // Margin of error

    // Read in the values
    printf("Bitte den Namen der CSV-Datein angeben: ");
    scanf("%s", filename);
    while (getchar() != '\n'); // Clear the input buffer
    bool success = load(filename, &A, &b, &x);
    printf("%s wurde %serfolgreich eingelesen!\n\n", filename, success ? "" : "nicht ");

    // Abort if not successfull
    if (!success) {
        printf("Das Programm wird jetzt beendet!\n");
        return 1;
    }

    char c = '0';
    while (c != 'j' && c != 'J' && c != 'g' && c != 'G') {
        printf("Soll das Gleichungssystem mit dem (J)acobi-Verfahren oder mit dem (G)auss-Seidel-Verfahren bearbeitet werden? ");
        scanf("%c", &c);
        while (getchar() != '\n');

        if (c == 'j' || c == 'J')
            method = JACOBI;
        else if (c == 'g' || c == 'G')
            method = GAUSS_SEIDEL;
        else
            printf("Diese Eingabe ist ungültig.\n");
    }

    while (margin < 0) {
        printf("\nBei welcher Genauigkeit soll die Berechnung beendet werden? ");
        scanf("%lf", &margin);
        while (getchar() != '\n');
        if (margin < 0)
            printf("Diese Eingabe ist ungültig! Bitte eine Zahl > 0 eingeben.\n");
    }

    printf("\nDie Berechnung für die Werte aus \"%s\" wird begonnen. Das Gewählte Verfahren ist das %s-Verfahren. "
           "Die Berechnung wird bei einem Fehlerwert von weniger als %.10lf beendet.\n",
           filename, method ? "Gauss-Seidel" : "Jacobi", margin);
    Vector *result = solve(method, &A, &b, &x, margin);
    printf("\n***Die Berechnung wurde beendet!***\n\n\n");

    // Ausgabewunsch abfragen
    printf("Sollen alle Iterationsschritte angezeigt werden? (j/n) ");
    scanf("%c", &c);
    while (getchar() != '\n');

    int counter = 0;
    while (result[counter].data != NULL)
      counter++;

    if (c == 'j' || c == 'J') {
        // PRINT ALL
        printf("Die Iterationsschritte sind: \n");

        for (int i = 0; i < counter; i++) {
          printf("%d)\t", i + 1);
          vectorPrint(result[i]);
        }
    } else {
        if (c != 'n' && c != 'N')
            printf("Keine Gültige Angabe. Es wird NEIN als Antwort angenommen.\n");

        // Just print the final result
        printf("Das Ergebnis ist: \n");
        vectorPrint(result[counter]);
    }


    for (int i = 0; i < 100; i++)
      free(result[i].data);
    free(result);

    for (int i = 0; i < A.n; i++)
    	free(A.data[i]);
    free(A.data);
    free(b.data);
    
    return 0;
}

Vector *solve(Method method, Matrix *A, Vector *b, Vector *x, double e) {
    Vector* toRet = calloc(100, sizeof(Vector));
    toRet[0] = *x;

    if (method == JACOBI) { // == JACOBI
        calculateLGS(A, b, toRet, e, solveJ);
    } else { // == GAUSS_SEIDEL
        calculateLGS(A, b, toRet, e, solveGS);
    }
    return toRet;
}

void solveJ(Matrix *A, Vector *b, Vector *newX, Vector *oldX) {
    // Transform "result" to get the next Vector
    for (int i = 0; i < A->n; ++i) {
      // Generating vector to create scalar product
      Vector k = {b->n, A->data[i]};

      // Calculating new x vector element
      double newValue = 1 / A->data[i][i];
      newX->data[i] = newValue * (b->data[i]
         - calculateScalar(&k, oldX, i));
    }
}

void solveGS(Matrix *A, Vector *b, Vector *x, Vector *oldX) {

    memcpy(x->data, oldX->data, sizeof(double) * b->n);

    // Calcualte new x vector
    for (int i = 0; i < b->n; i++) {
      // Generating vector to create scalar product
      Vector k = {b->n, A->data[i]};

      // Calculating new x vector element
      double newValue = 1 / A->data[i][i];
      x->data[i] = newValue * (b->data[i]
         - calculateScalar(&k, x, i));
    }
}

void calculateLGS(Matrix* A, Vector* b, Vector* x, double e,
                    void (*f)(Matrix*, Vector*, Vector*, Vector*)) {
  // Create counter variable
  static int iterationsCounter = 1;

  // Copy pointer
  x[iterationsCounter].n = b->n;
  x[iterationsCounter].data = (double*)malloc(sizeof(double) * b->n);

  // Calculating new x vector
  f(A, b, &x[iterationsCounter], &x[iterationsCounter - 1]);

  // Check vector distance
  if (vectorDistance(x[iterationsCounter-1], x[iterationsCounter]) >= e
        && (iterationsCounter < 100)) {
    // Increment counter
    iterationsCounter++;
    calculateLGS(A, b, x, e, f);
  } else {
    // Set print end condition
    if (iterationsCounter + 1 <= 99) {
    	x[iterationsCounter+1].n = 0;
    	x[iterationsCounter+1].data = NULL;
    }
  }
}
