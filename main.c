#include "utils.h"

// Solving Methods
Vector *solve(Method method, Matrix *A, Vector *b, Vector *x, double e);
Vector *solveJ(Matrix *A, Vector *b, Vector *x, double e, int n);  // JACOBI
Vector *solveGS(Matrix *A, Vector *b, Vector *x, double e); // GAUSS_SEIDEL
double calculateScalar(Vector* vector, Vector* x);

int main() {
    // Init Variables
    char *filename = malloc(260 * sizeof(char));    // Maximale Pfadlänge in Windows ist 260.
    Matrix *A = malloc(sizeof(Matrix));             // A is a pointer to the values of the SLE (System of linear equations)
    Vector *b = malloc(sizeof(Vector));             // b is a pointer to the right side of the SLE (System of linear equations)
    Vector *x = malloc(sizeof(Vector));             // x is a pointer to the start values
    Method method = 0;
    double margin = -1.0; // Margin of error

    // Read in the values
    printf("Bitte den Namen der CSV-Datein angeben: ");
    scanf("%s", filename);
    while (getchar() != '\n'); // Clear the input buffer
    bool success = load(filename, A, b, x);
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
    Vector *result = solve(method, A, b, x, margin);
    printf("\n***Die Berechnung wurde beendet!***\n\n\n");

    // Ausgabewunsch abfragen
    printf("Sollen alle Iterationsschritte angezeigt werden? (j/n) ");
    scanf("%c", &c);
    while (getchar() != '\n');

    if (c == 'j' || c == 'J') {
        // Count through the results until we hit the starting Vector to determine the amount of steps
        int number = 0;
        while (!vectorCompare(x, &result[number]))
            number++;

        // PRINT ALL
        printf("Die Iterationsschritte sind: \n");
        for (int i = 0; i <= number; ++i) {
            printf("%d)\t", i);
            vectorPrint(result[number - i]);
        }
    } else {
        if (c != 'n' && c != 'N')
            printf("Keine Gültige Angabe. Es wird NEIN als Antwort angenommen.\n");

        // Just print the final result
        printf("Das Ergebnis ist: \n");
        vectorPrint(result[0]);
    }
    return 0;
}

Vector *solve(Method method, Matrix *A, Vector *b, Vector *x, double e) {
    Vector *toRet;
    if (method == JACOBI) {
        toRet = solveJ(A, b, x, e, 0);
    } else { // == GAUSS_SEIDEL
        toRet = solveGS(A, b, x, e);
    }
    return toRet;
}

Vector *solveJ(Matrix *A, Vector *b, Vector *x, double e, int n) {
    if (n >= 100) // End Rekursion
        return x;

    Vector *toRet = malloc((n + 1) * sizeof(Vector));

    Vector *start = x; // Index 0 is the latest Result, so Array Indexing is not needed

    // Jacobi uses b as base to calculate the next Iteration. So b gets copied to the result first.
    Vector res;
    res.data = malloc(b->n * sizeof(double));
    for (int i = 0; i < b->n; ++i) { // copy the vector. NOT only the pointer
        res.data[i] = b->data[i];
    }
    res.n = b->n;
    Vector *result = &res;

    // Transform "result" to get the next Vector
    for (int i = 0; i < A->n; ++i) {
        for (int j = 0; j < A->n; ++j) {
            if (j != i) {
                result->data[i] -= A->data[i][j] * start->data[j];
            }
        }
        result->data[i] /= A->data[i][i];
    }

    // Build Array toRet
    toRet[0] = *result;
    for (int i = 1; i <= n + 1; ++i)
        toRet[i] = x[i - 1];

    // Continue Calculation or return
    if (vectorDistance(*start, *result) <= e) {
        return toRet;
    } else {
        return solveJ(A, b, toRet, e, n + 1);
    }
}

Vector *solveGS(Matrix *A, Vector *b, Vector *x, double e) {

    static int iterationsCounter = 0;
    Vector lastX = x[0];
    Vector xNew;
    xNew.n = (*b).n;
    xNew.data = malloc((*b).n * sizeof(double));

    // Copy pointer
    Vector* newX = (Vector*) malloc((iterationsCounter + 1)*sizeof(Vector));
    for (int i = 1; i < (iterationsCounter + 1); i++) {
        newX[i] = x[i - 1];
    }

    // Calcualte new x vector
    for (int i = 0; i < x->n; i++) {
        // Transform row of matrix into vector
        Vector matrixLine = {x->n, A->data[i]};
        // Calculate new vector values
        double newComponent = lastX.data[i] - (1 / A->data[i][i])
              * (calculateScalar(&matrixLine, &lastX) - b->data[i]);
        xNew.data[i] = newComponent;
    }

    // Set new vector as first vectors
    newX[0] = xNew;

    // Ermitteln der Abweichung
    bool fehlerWertIstKleiner = vectorDistance(lastX, xNew) > e;

    if (fehlerWertIstKleiner && iterationsCounter < 100) {
        free(x); // Free the last x pointer on RAM
        x = newX; // Set newX as x for recursive call
        iterationsCounter++;
        solveGS(A, b, newX, e); // Recursive call
    } else {
        // Return results
        return newX;
    }
}
