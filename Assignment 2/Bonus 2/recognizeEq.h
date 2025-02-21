/* recognizeExp.h, Gerard Renardel, 29 January 2014 */

#ifndef RECOGNIZEEXP_H
#define RECOGNIZEEXP_H

int acceptNumber(List *lp);
int acceptIdentifier(List *lp);
int acceptCharacter(List *lp, char c);
int acceptExpression(List *lp);
void recognizeEquation();
int getDegree(List lp);
int hasSingleVariable(List lp);
int solveEqDeg1(List lp, double *solution);
void scanExpression(List eq, double *a, double *b, double *c, char *var1);
int solveLinearEquations(List eq1, List eq2, double *x, double *y, char **var1, char **var2);


#endif