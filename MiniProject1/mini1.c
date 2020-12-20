#include "MiniProject1.h"

int main(int argc, char const *argv[])
{
    /* code */
    if(argc != 3) {
        printf("error: for using type: \n./mini1 dimacs.txt affectation.txt\n");
        return 0;
    }

    char * problemPath = argv[1];
    char * affectationPath = argv[2];

    struct SATProblem problem =
            readSATProblem(problemPath);
    int varialbes = problem.variables;
    struct Affectation affec =
            readAffectation(affectationPath, varialbes);

    printf("verify: %d", verifySAT(problem, affec));
    printf("\n");
    return 0;
}