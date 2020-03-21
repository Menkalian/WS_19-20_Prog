#include "utils.h"

// Solving Methods
Vector *solve(Method method, Matrix *A, Vector *b, Vector *x, double e);

void solveJ(Matrix *A, Vector *b, Vector *newX, Vector *oldX);  // JACOBI
void solveGS(Matrix *A, Vector *b, Vector *newX, Vector *oldX); // GAUSS_SEIDEL
void solveLGS(Matrix *A, Vector *b, ListElement *x, double e, void (*f)(Matrix *, Vector *, Vector *, Vector *));

double calculateScalar(Vector *v1, Vector *v2, int indexToIgnore);

int main() {
    // Init Variables
    char *filename = malloc(260 * sizeof(char));    // maximum pathlength in windows is 260.
    // (we chose windows as limit since Linux's limit is higher as far as I know and the program should work on as many machines as possible.)
    Matrix *A = malloc(
            sizeof(Matrix));             // A is a pointer to the values of the SLE (System of linear equations)
    Vector *b = malloc(
            sizeof(Vector));             // b is a pointer to the right side of the SLE (System of linear equations)
    Vector *x = malloc(sizeof(Vector));             // x is a pointer to the start values
    Method method = 0;
    double margin = -1.0; // Margin of error
    bool ask = false;

    do { // Loop for retrying on error
        if (ask) {
            // Asking wether to start a new calculation
            char c = '0';
            while (c != 'j' && c != 'J' && c != 'n' && c != 'N') {
                printf("Soll eine neue Berechnung gestartet werden? (j/n) ");
                scanf("%c", &c);
                while (getchar() != '\n');

                if (c == 'j' || c == 'J') {
                    printf("Neue Berechnung wird gestartet...\n");
                    ask = false;
                } else if (c == 'n' || c == 'N') {
                    printf("Das Programm wird beendet.\n");
                } else
                    printf("Diese Eingabe ist ungültig.\n");
            }
            if (ask)
                break;
            else
                continue;
        } else {
            ask = true;
        }

        // Read in the values
        printf("Bitte den Namen der CSV-Datein angeben: ");
        scanf("%s", filename);
        while (getchar() != '\n'); // Clear the input buffer
        bool success = load(filename, A, b, x);
        printf("%s wurde %serfolgreich eingelesen!\n\n", filename, success ? "" : "nicht ");

        // Abort if not successfull
        if (!success) {
            printf("Die Berechung wird abgebrochen!\n");
            continue;
        }

        // Asking which method to use
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

        // Asking for the margin of error
        while (margin < 0) {
            printf("\nBei welcher Genauigkeit soll die Berechnung beendet werden? ");
            scanf("%lf", &margin);
            while (getchar() != '\n');
            if (margin < 0)
                printf("Diese Eingabe ist ungültig! Bitte eine Zahl > 0 eingeben.\n");
        }

        // Start the calculation
        printf("\nDie Berechnung für die Werte aus \"%s\" wird begonnen. Das Gewählte Verfahren ist das %s-Verfahren. "
               "Die Berechnung wird bei einem Fehlerwert von weniger als %.10lf beendet.\n",
               filename, method ? "Gauss-Seidel" : "Jacobi", margin);
        Vector *result = solve(method, A, b, x, margin);
        printf("\n***Die Berechnung wurde beendet!***\n\n\n");

        if (result[0].n < 1) {
            // error occured -> Abort
            printf("Ein Fehler ist während der Berechnung aufgetreten. Die Berechnung wird abgebrochen!\n");
            continue;
        }

        // Ausgabewunsch abfragen
        printf("Sollen alle Iterationsschritte angezeigt werden? (j/n) ");
        scanf("%c", &c);
        while (getchar() != '\n');

        int number = 0;
        // Count through the results until we hit an invalid Vector to determine the amount of steps
        while (result[number].n == b->n)
            number++;

        // When we hit an invalid vector we are to far, so go back a step.
        number--;

        if (c == 'j' || c == 'J') {
            // PRINT ALL
            printf("Die Iterationsschritte sind: \n");
            for (int i = 0; i <= number; ++i) {
                printf("%d)\t", i + 1);
                vectorPrint(result[number - i]);
            }

        } else {
            if (c != 'n' && c != 'N')
                printf("Keine Gültige Angabe. Es wird NEIN als Antwort angenommen.\n");

            // Just print the final result
            printf("Das Ergebnis ist: \n");
            vectorPrint(result[0]);
        }

        // Free all data pointer of Vector Variables
        for (int i = 0; i < number; i++)
            free(result[i].data);

        // Free all pointer
        for (int i = 0; i < A->n; i++)
            free(A->data[i]);

        free(result);

        break;
    } while (true);

    free(A);
    free(filename);
    free(b);

    return 0;
}

/*
 * This method determines the method to use and calls the solveLGS method with the correct parameters
 * @param method: One value of the Enum Method to determine the selected algorithm
 * @param *A: matrix with the left-hand-side values of the LGS
 * @param *b: vector with the right-hand-side values of the LGS
 * @param *x: vector with the starting values for the algorithm
 * @param e : margin of error
 * @return: Array of all iteration steps as vector. index 0 is the solution.
 */
Vector *solve(Method method, Matrix *A, Vector *b, Vector *x, double e) {
    ListElement* start = (ListElement*)calloc(1, sizeof(ListElement));
    start->vector = *x;
    start->last = NULL;
    start->next = NULL;

    if (method == JACOBI) { // == JACOBI
      solveLGS(A, b, start, e, solveJ);
    } else { // == GAUSS_SEIDEL
      solveLGS(A, b, start, e, solveGS);
    }

    int listLength = getListLength(start);
    Vector *toRet = (Vector*)calloc(1, listLength * sizeof(Vector));
    convertListToArray(toRet, start, listLength);

    //deleteChainedList(start);

    return toRet;
}

/*
 * This method calculates the solution for the LGS. calls itself in a recursion
 * @param *A: matrix with the left-hand-side values of the LGS
 * @param *b: vector with the right-hand-side values of the LGS
 * @param *x: vector with the starting values for the algorithm
 * @param e : margin of error
 * @param f : method to calculate the next iterationstep
 * @return: Array of all iteration steps as vector. index 0 is the solution.
 */
 void solveLGS(Matrix *A, Vector *b, ListElement *x, double e,
                  void (*f)(Matrix *, Vector *, Vector *, Vector *)) {
     // Create counter variable
     static int iterationsCounter = 0;

     // Add new element to list
     ListElement* newElement = (ListElement*)calloc(1, (sizeof(ListElement)));
     x->next = newElement;
     newElement->next = NULL;
     newElement->last = x;

     newElement->vector = x->vector;
     size_t dataLength = newElement->vector.n * sizeof(double);
     double* newData = (double*)calloc(1, dataLength);
     memcpy(newData, (x->vector).data, dataLength);
     (newElement->vector).data = newData;
     
     // Calculating new x vector
     f(A, b, &newElement->vector, &x->vector);

     // Increment counter and free last x vector
     iterationsCounter++;

     // Check vector distance
     if (!(vectorDistance(x->vector, newElement->vector) <= e
           || iterationsCounter > 100)) {
         solveLGS(A, b, newElement, e, f);
     } else if (iterationsCounter > 100) {
        printf("\n**Fehlerschranke wurde nach 100 Iterationen nicht unterschritten.**\nMöglicherweise konvergieren die Werte nicht. Beende Berechnung!\n");
     }
}

/*
 * This method calculates the next vector for a recursion-step of the algorithm
 * @param *A: matrix with the left-hand-side values of the LGS
 * @param *b: vector with the right-hand-side values of the LGS
 * @param @out *newX: pointer to memory-area to save the next x value into
 * @param *oldX: previous x value to calculate the next of
 */
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

/*
 * This method calculates the next vector for a recursion-step of the algorithm
 * @param *A: matrix with the left-hand-side values of the LGS
 * @param *b: vector with the right-hand-side values of the LGS
 * @param @out *newX: pointer to memory-area to save the next x value into
 * @param *oldX: previous x value to calculate the next of
 */
void solveGS(Matrix *A, Vector *b, Vector *newX, Vector *oldX) {
    // Calcualte new x vector
    for (int i = 0; i < b->n; i++) {
        // Generating vector to create scalar product
        Vector k = {b->n, A->data[i]};

        // Calculating new x vector element
        double newValue = 1 / A->data[i][i];
        newX->data[i] = newValue * (b->data[i]
                                    - calculateScalar(&k, newX, i));
    }
}
