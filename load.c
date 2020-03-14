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

    /*
     * count lines and columns
     * verify that data is valid
     */

    int lines = 0;
    int columns = 0;
    int prevcolumns = 0;
    int temp = 0;
    bool firstLine = true;
    bool lineWithData = false;

    while (temp != EOF) {

        temp = fgetc(fp);

        if (temp == ',') {

            columns++;
            lineWithData = true;

        } else if (temp == '\n' && lineWithData == true) {

            lines++;
            lineWithData = false;

            if (firstLine == true) {

                prevcolumns = columns;
                columns = 0;
                firstLine = false;
            } else if (prevcolumns != columns) {

                printf("Daten ungueltig\n");
                return false;
            } else columns = 0; //end if
        }//end if
    }//end while

    //necessary if there is no word wrap at end of file
    if (lineWithData == true) {

        lines++;

        if (prevcolumns != columns) {

            printf("Daten ungueltig\n");
            return false;
        } //end if
    }//end if

    columns = prevcolumns + 1;
    if (lines + 1 != columns && lines + 2 != columns) {

        printf("Daten ungueltig\n");
        return false;
    }

    /*
     * init matrix
     */

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
    fseek(fp, 0, SEEK_SET);

    //go through file
    for (int z = 0; z < lines; z++) {
        double tempData = 0;
        int nextChar = 0;

        //save xCoefficients
        for (int s = 0; s < lines; s++) {

            fscanf(fp, "%lf", &tempData);

            //ignore whitespaces
            do {
                nextChar = fgetc(fp);
            } while (nextChar != ',');

            matrix[z][s] = tempData;
            tempData = 0;
        }

        //save solutions
        fscanf(fp, "%lf", &tempData);
        solutions[z] = tempData;
        tempData = 0;

        //save starting Vectors if existing
        if (columns == lines + 2) {

            //ignore whitespaces and word wrap
            do {
                nextChar = fgetc(fp);
            } while (nextChar != ',' && nextChar != '\n' && nextChar != EOF);

            fscanf(fp, "%lf", &tempData);
            startVect[z] = tempData;
            tempData = 0;

        } else {

            startVect[z] = 0;
        }

        //ignore whitespaces and word wrap
        do {
            nextChar = fgetc(fp);
        } while (nextChar != ',' && nextChar != '\n' && nextChar != EOF);
    }

    /*
     * Assign the values
     */

    *A = (Matrix) {.n = lines, .data =  matrix};
    *b = (Vector) {.n = lines, .data = solutions};
    *x = (Vector) {.n = lines, .data = startVect};

    fclose(fp);

    return true;
}
