#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int variables;
int clauses;
struct SATProblem  {
    int value[100][100];
    int variables;
    int clauses;
};

struct Affectation {
    int * affec;
};

/**
 *
 * @param path
 * @return Sat problem with n variables and m clause;
 * in clause if value[i][j] = 1 mean clause (i-1), variable (j-1) is positive
 * in clause if value[i][j] = -1 mean clause (i-1), variable (j-1) is negative
 * in clause if value[i][j] = 0 mean clause (i-1), variable (j-1) is not appear.
 */
struct SATProblem readSATProblem(char * path){
    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
    char ch;
    int count = 0;
    int match = fscanf(file, "p cnf %d %d", &variables, &clauses);
    struct SATProblem matrix;
    if (match < 2) {
        printf("The clauses and variables isn't true");
        return matrix;
    }
    matrix.variables = variables;
    matrix.clauses = clauses;
    printf("clause: %d variable: %d\n", clauses, variables);
    for(int i=0;i<clauses;i++)
    {
        for(int j=0;j<variables;j++)
        {
            matrix.value[i][j] = 0;
        }
    }

    int row = 0;
    for(;;){
        int literal;
        fscanf(file, "%d", &literal);
        if(literal==0) {
            char ch = getc(file);
            if(ch == '\n') row++;
            if(ch == EOF) {
                break;
            }
        } else {
            if(literal < 0) {
                matrix.value[row][0-literal-1] = -1;
            } else {
                matrix.value[row][literal-1] = 1;
            }
        }
    }

    for (int i = 0; i < matrix.clauses; ++i) {
        for (int j = 0; j < matrix.variables; ++j) {
            printf("%d ", matrix.value[i][j]);
        }
        printf("\n");
    }
    fclose(file);
    return matrix;
}

/**
 *
 * @param path
 * @param variables number of variables
 * @return a struct of Affecation.
 * affect[i] = 0 mean variable i assign with negative (0), 1 mean positive (1)
 */

struct Affectation readAffectation (char * path, int variables) {
    FILE* fp = fopen(path, "r");
    int value;
    struct Affectation affec;
    affec.affec = malloc(variables*sizeof(int));
    for (int i = 0; i < variables; i++) {
        fscanf(fp, "%d", &value);
        if(value > 0) {
            affec.affec[value - 1] = 1;
        } else {
            affec.affec[0 - value - 1] = 0;
        }
    }
    fclose(fp);
    printf("affec: ");
    for (int i = 0; i < variables; ++i) {
        printf("%d ", affec.affec[i]);
    }
    printf("\n");
    return affec;
}

/**
 *
 * @param problem
 * @param affec
 * @return
 */
int verifySAT(struct SATProblem problem, struct Affectation affec) {
    int clauseIndex = 0;
    for(clauseIndex = 0; clauseIndex < problem.clauses; clauseIndex++) {
        int check = 0;
        for(int i = 0; i < problem.variables; i ++) {
            if(problem.value[clauseIndex][i]==0) continue;
            if(problem.value[clauseIndex][i]==1) {
                if(affec.affec[i] == 1) {
                    check = 1;
                    break;
                }
            } else if (problem.value[clauseIndex][i] == -1) {
                if(affec.affec[i] == 0) {
                    check = 1;
                    break;
                }
            }
        }
        if(check==0) return 0;
    }
    return 1;
}

int main() {
    struct SATProblem problem =
            readSATProblem("./test/mini1/test1.txt");
    int varialbes = problem.variables;
    struct Affectation affec =
            readAffectation("./test/mini1/affect.txt", varialbes);

    printf("verify: %d", verifySAT(problem, affec));
    printf("\n");
    return 0;
}