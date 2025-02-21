/* file :    recognizeEq.c */
/* author :  Samuel Mikulas (s.mikulas@student.rug.nl) */
/* date :    Tue 19 Feb 2024 */
/* version:  1.0 */

/* Description:
    Program evaluates scans system of two linear equations
    solves it if possible.
  */

  #include <math.h>   /* sqrt() */
  #include <stdio.h>  /* getchar, printf */
  #include <stdlib.h> /* NULL */
  #include <string.h> /* strcmp */
  
  #include "scanner.h"
  #include "recognizeEq.h"
  
  int acceptNumber(List *lp) {
     if (*lp != NULL && (*lp)->tt == Number) {
        *lp = (*lp)->next;
        return 1;
     }
     return 0;
  }
  
  int acceptIdentifier(List *lp) {
     if (*lp != NULL && (*lp)->tt == Identifier) {
        *lp = (*lp)->next;
        return 1;
     }
     return 0;
  }
  
  int acceptCharacter(List *lp, char c) {
     if (*lp != NULL && (*lp)->tt == Symbol && ((*lp)->t).symbol == c) {
        *lp = (*lp)->next;
        return 1;
     }
     return 0;
  }
  
  int acceptTerm(List *lp) {
     if (acceptNumber(lp)) {
        if (acceptIdentifier(lp)) {
           if (acceptCharacter(lp, '^'))
              return acceptNumber(lp);
           return 1;
        }
        return 1;
     } else if (acceptIdentifier(lp)) {
        if (acceptCharacter(lp, '^'))
           return acceptNumber(lp);
        return 1;
     }
     return 0;
  }
  
  int acceptExpression(List *lp) {
     acceptCharacter(lp, '-');
     if (!acceptTerm(lp))
        return 0;
     while (acceptCharacter(lp, '+') || acceptCharacter(lp, '-')) {
        if (!acceptTerm(lp))
           return 0;
     }
     return 1;
  }
  
  int acceptEquation(List *lp) {
     return acceptExpression(lp) && acceptCharacter(lp, '=') && acceptExpression(lp);
  }
  
  /* Function transforms token list to form a*(var1) = b*var2 + c */
  void scanExpression(List eq, double *a, double *b, double *c, char *var1) {
     /* Scan left side of equation */
     int coeficient;
     while (!acceptCharacter(&eq, '=')) {
        /* gets sign of coeficent*/
        int coeficient = acceptCharacter(&eq, '-') ? -1 : 1;
        acceptCharacter(&eq, '+');
  
        if (eq->tt == Number) {
           coeficient *= eq->t.number;
           acceptNumber(&eq);
        }
  
        if (eq->tt == Identifier) {
           if (!strcmp(var1, eq->t.identifier)) {
              (*a) += coeficient; /* Identifier is var1 */
           } else {
              (*b) -= coeficient; /* Identifier is var2 */
           }
           acceptIdentifier(&eq);
        } else {
           (*c) -= coeficient; /* Term is number */
        }
     }
  
     /* Scan right side of equation */
     while (eq != NULL) {
        /* gets sign of coeficent*/
        int coeficient = acceptCharacter(&eq, '-') ? 1 : -1;
        acceptCharacter(&eq, '+');
  
        if (eq->tt == Number) {
           coeficient *= eq->t.number;
           acceptNumber(&eq);
        }
  
        if (eq != NULL && eq->tt == Identifier) {
           if (!strcmp(var1, eq->t.identifier)) {
              (*a) += coeficient;  /* identifier is var1 */
           } else {
              (*b) -= coeficient; /* identifier is var2 */
           }
           acceptIdentifier(&eq);
        } else {
           (*c) -= coeficient;  /* Term is Number */
        }
     }
  }
  
  /* Function revrites two equations to the form:
      (a)*var1 = (b)*var2 + c
      (d)*var1 = (e)*var2 + f
      then evaluates if systems is solvable or not.
      */
  int solveLinearEquations(List eq1, List eq2, double *x, double *y, char **var1, char **var2) {
     double a = 0, b = 0, c = 0;
     double d = 0, e = 0, f = 0;
  
     /* Extracts variables */
     List temp = eq1;
     while (temp != NULL) {
        if (temp->tt == Identifier) {
           if (*var1 == NULL) {
              *var1 = temp->t.identifier;
           } else if (strcmp(*var1, temp->t.identifier)) {
              *var2 = temp->t.identifier;
           }
        }
        temp = temp->next;
     }
  
     /* First equation does not conatin both variables */
     if (*var2 == NULL) {
        temp = eq2;
        while (temp != NULL) {
           if (temp->tt == Identifier) {
              if (strcmp(*var1, temp->t.identifier)) {
                 *var2 = temp->t.identifier;
              }
           }
           temp = temp->next;
        }
     }
  
     scanExpression(eq1, &a, &b, &c, *var1);
     scanExpression(eq2, &d, &e, &f, *var1);
  
     if (a == 0 || d == 0) {
        return 0;
     }
  
     (*y) = (f - (d * c) / a) / ((d * b) / a - e);
     (*x) = (b * (*y)) / a + (c / a);
     return 1;
  }
  
  void recognizeEquation() {
     char *ar1, *ar2;
     List tl, tl1;
     List tl2, tl3;
     printf("give an equation: ");
     ar1 = readInput();
  
     while (ar1[0] != '!') {
        tl = tokenList(ar1);
        printList(tl);
  
        printf("give an equation: ");
        ar2 = readInput();
        tl2 = tokenList(ar2);
        printList(tl2);

        tl1 = tl;
        tl3 = tl2;
        double x, y;
        char *var1 = NULL, *var2 = NULL;
        if (acceptEquation(&tl1) && tl1 == NULL &&
            acceptEquation(&tl3) && tl3 == NULL) {
           if (solveLinearEquations(tl, tl2, &x, &y, &var1, &var2)) {
              printf("solution: %s = %.3f and %s = %.3f\n", var1, x, var2, y);
           } else {
              printf("not solvable\n");
           }
        } else {
           printf("this is not an equation\n");
        }
  
        free(ar1);
        free(ar2);
        freeTokenList(tl);
        freeTokenList(tl2);
  
        printf("\ngive an equation: ");
        ar1 = readInput();
     }
     free(ar1);
     printf("good bye\n");
  }
  