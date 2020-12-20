#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct SATProblem  {
    int *value;
    int variables;
    int clauses;
};

struct Affectation {
    int * affec;
};

struct SATProblem readSATProblem(char * path);

struct Affectation readAffectation (char * path, int variables);

int verifySAT(struct SATProblem problem, struct Affectation affec);
//int verifySAT(struct Affectation problem, int b);