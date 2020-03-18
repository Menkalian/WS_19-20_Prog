#include "utils.h"

/*
 * This method calculates the solution for the LGS. calls itself in a recursion
 * @param *filename: name/path of the .csv file to load
 * @param @out *A: matrix with the left-hand-side values of the LGS
 * @param @out *b: vector with the right-hand-side values of the LGS
 * @param @out *x: vector with the start values for the LGS
 * @return: true if successful, false if any error occurred
 */
bool load(const char *filename, Matrix *A, Vector *b, Vector *x) {

    //convert Windows data path
    char *c = strchr(filename, 92);
    char fileExtension[] = ".csv";

    while (c != NULL) {
        *c = '/';
        c = strchr(filename, 92);
    }

    //check file extension (.csv)
    c = strstr(filename, fileExtension);
    if (c == NULL) {
        printf("%s ist keine CSV-Datei\n", filename);
        return false;
    }

    //open file
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Datei konnte nicht geoeffnet werden\n");
        return false;
    }

    bool startVectExists;
    int lines;

    bool countSuccess = coVal(filename, &lines, &startVectExists);

    if (!countSuccess) return false;

    //init matrix
    double **matrix = malloc(lines * sizeof(double *));

    for (int i = 0; i < lines; i++) {

        double *xCoefficients = malloc(lines * sizeof(double));
        matrix[i] = xCoefficients;
    }

    double *solutions = malloc(lines * sizeof(double));
    double *startVect = malloc(lines * sizeof(double));

    /*
     * load data
     */

    //go to beginning of file
    //fseek(fp, 0, SEEK_SET);

    //go through file
    for (int i1 = 0; i1 < lines; i1++) {
        double tempData = 0;

        //save xCoefficients
        for (int i2 = 0; i2 < lines; i2++) {

            fscanf(fp, "%lf", &tempData);
            matrix[i1][i2] = tempData;
            tempData = 0;

            nextSepChar(fp);
        }//end for

        //save solutions
        fscanf(fp, "%lf", &tempData);
        solutions[i1] = tempData;
        tempData = 0;

        nextSepChar(fp);

        //save starting Vectors if existing
        if (startVectExists) {

            fscanf(fp, "%lf", &tempData);
            startVect[i1] = tempData;
            tempData = 0;

            nextSepChar(fp);
        } else startVect[i1] = 0;//end if
    }//end for

    //assign data
    *A = (Matrix) {.n = lines, .data =  matrix};
    *b = (Vector) {.n = lines, .data = solutions};
    *x = (Vector) {.n = lines, .data = startVect};

    fclose(fp);

    return true;
}

/*
 * This method counts the lines and columns of the LGS.
 * Empty lines are ignored.
 * It also tests if the LGS has a correct form.
 * @param *filename: name/path of the .csv file to load
 * @param *columns: number of columns of the LGS
 * @param *startVectExists: true if the start values are specified, false if not
 * @return: true if successful, false if any error occurred
 */
bool coVal (const char *filename, int *lines, bool *startVectExists) {

    FILE *fpc;
    fpc = fopen(filename, "r");

    if (fpc == NULL) {
        return false;
    }

    int cLines = 0, cColumns = 0, prevColumns = 0;
    int temp = 0;
    bool firstLine = true, firstCommaInLine = true;

    while (temp != EOF) {

        temp = fgetc(fpc);

        if (temp == ',') {

            cColumns++;
            //only count line if there is at least one comma in it
            if (firstCommaInLine == true) {
                cLines++;
                firstCommaInLine = false;
            }//end if

        } else if ( (temp == '\n' || temp == EOF) && cColumns != 0) {

            firstCommaInLine = true;
            //there is one more column then commas
            cColumns++;
            if (firstLine == true) {

                prevColumns = cColumns;
                cColumns = 0;
                firstLine = false;
            } else if (prevColumns != cColumns) {

                printf("Daten ungueltig\n");
                return false;
            } else cColumns = 0; //end if
        }//end if
    }//end while

    fclose(fpc);

    if (cLines + 1 == prevColumns) *startVectExists = false;
    else if (cLines + 2 == prevColumns) *startVectExists = true;
    else return false;

    *lines = cLines;
    return true;
}

/*
 * This method gets the next seperating char (',' ; '\n' ; EOF) in the file.
 * @param *fpn: FILE Pointer to the file containing the LGS
 * @return void
 */
void nextSepChar(FILE *fpn) {
    char temp;

    do {
        temp = fgetc(fpn);
    } while (temp != ',' && temp != '\n' && temp != EOF);

    return;
}
