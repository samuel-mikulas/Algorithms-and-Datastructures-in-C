

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
int solveEqDeg2(List lp, double *solution, double *solution2);

#endif
