#include "MiniProject1.h"

int variables;
int clauses;
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
    matrix.value = malloc(clauses*variables*sizeof(int));
    printf("clause: %d variable: %d\n", clauses, variables);

    // value[i][j] is access by value[i*variables + j]
    for(int i=0;i<clauses;i++)
    {
        for(int j=0;j<variables;j++)
        {
            matrix.value[i*variables + j] = 0;
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
                matrix.value[row*variables + (0-literal-1)] = -1;
            } else {
                matrix.value[row*variables + (literal-1)] = 1;
            }
        }
    }

    // for (int i = 0; i < matrix.clauses; ++i) {
    //     printf("(");
    //     for (int j = 0; j < matrix.variables; ++j) {
    //         if(matrix.value[i * variables + j] == 0 ) continue;
    //         else if (matrix.value[i * variables + j]==1)
    //         {
    //             printf("x%d", j+1);
    //         } else if (matrix.value[i * variables + j]==-1)
    //         {
    //             printf("¬x%d", j+1);
    //         }
    //         if(j<matrix.variables-1) {
    //             printf(" ∨ ");
    //         }
    //     }
    //     if(i<matrix.clauses-1){
    //         printf(") ∧ ");
    //     }else
    //     {
    //         printf(").\n");
    //     }
        
    // }
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
    int variables = problem.variables;
    for(clauseIndex = 0; clauseIndex < problem.clauses; clauseIndex++) {
        int check = 0;
        for(int i = 0; i < problem.variables; i ++) {
            if(problem.value[clauseIndex*variables + i]==0) continue;
            if(problem.value[clauseIndex*variables + i]==1) {
                if(affec.affec[i] == 1) {
                    check = 1;
                    break;
                }
            } else if (problem.value[clauseIndex*variables+ i] == -1) {
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
//  int verifySAT(struct Affectation problem, int b) {
//      printf("int verify %d %d \n", problem.affec[0], b);
//      return 1;
//  }

