/* file :     */
/* author :   */
/* date :     */
/* version:   */

/* Description:
    Program evaluates if the input is an equation of
    one variable and outputs its degree. If the degree of equation is 1,
    program also solves (detects that equation is not solvable) the equation.
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
  
  int getDegree(List lp) {
     int degree = 0;
     while (lp != NULL) {
        if (lp->tt == Identifier && lp->next != NULL &&
            lp->next->tt == Symbol && lp->next->t.symbol == '^') {
           int exponent = 0;
           List temp = lp->next->next;
           if (temp != NULL && temp->tt == Number) {
              exponent = temp->t.number;
              if (exponent > degree)
                 degree = exponent;
           }
  
        } else if (lp->tt == Identifier) {
           if (degree < 1)
              degree = 1;
        }
  
        lp = lp->next;
     }
     return degree;
  }
  
  int hasSingleVariable(List lp) {
     char *var = NULL;
     while (lp != NULL) {
        if (lp->tt == Identifier) {
           if (var == NULL)
              var = lp->t.identifier;
           else if (strcmp(var, lp->t.identifier) != 0)
              return 0;
        }
        lp = lp->next;
     }
     return var != NULL;
  }
  
  /* Function solves the equation of degree one or outputs
    if the equation is not solvable. */
  int solveEqDeg1(List lp, double *solution) {
     // Keeps track of left and right hand side of equation
     // (left)*identifier = (right)
     int left = 0;
     int right = 0;
     int temp = 0;
     while (!acceptCharacter(&lp, '=')) {
        // Multply by negative when - in front of term
        int mult = acceptCharacter(&lp, '-') ? -1 : 1;
        acceptCharacter(&lp, '+'); /*skip plus*/
  
        if (acceptIdentifier(&lp)) {
           if (acceptCharacter(&lp, '^')) {
              /* (identifier)^(1/0) */
              if ((lp)->t.number == 0) {
                 right -= mult;
              } else {
                 left += mult * (lp)->t.number;
              }
              acceptNumber(&lp);
           } else {
              /* (identifier) */
              left += mult;
           }
        } else {
           temp = (lp)->t.number * mult;
           acceptNumber(&lp);
           if (acceptIdentifier(&lp)) {
              if (acceptCharacter(&lp, '^')) {
                 /* (number)(identifier)^(1/0)*/
                 if ((lp)->t.number == 0) {
                    right -= temp;
                 } else {
                    left += temp;
                 }
                 acceptNumber(&lp);
              } else {
                 /* (number)(identifier) */
                 left += temp;
              }
           } else {
              right -= temp;
           }
        }
     }
  
     while ((lp) != NULL) {
        int mult = acceptCharacter(&lp, '-') ? -1 : 1;
        acceptCharacter(&lp, '+');
  
        if (acceptIdentifier(&lp)) {
           if (acceptCharacter(&lp, '^')) {
              /* (identifier)^(1/0) */
              if (lp->t.number == 0) {
                 right += mult;
              } else {
                 left -= mult * (lp)->t.number;
              }
              acceptNumber(&lp);
           } else {
              left -= mult;
           }
        } else {
           temp = (lp)->t.number * mult;
           acceptNumber(&lp);
           if (acceptIdentifier(&lp)) {
              if (acceptCharacter(&lp, '^')) {
                 /* (identifier) ^ (1/0) */
                 if ((lp)->t.number == 0) {
                    right += temp;
                 } else {
                    left -= temp;
                 }
                 acceptNumber(&lp);
              } else {
                 /* (identifier) */
                 left -= temp;
              }
           } else {
              /* (number) */
              right += temp;
           }
        }
     }
  
     /* Equation can not be solved since denominator
       would become zero */
     if (left == 0) {
        return 0;
     }
  
     /* Converts solutions in range [-0.0005, 0] to 0*/
     *solution = right / (double)left;
     if (-0.0005 <= *solution && *solution <= 0) {
        *solution = 0.0;
     }
     return 1;
  }
  
  /* Function solves the equation of degree one or outputs
     if the equation is not solvable. */
  int solveEqDeg2(List lp, double *solution1, double *solution2) {
     // Keeps track of a, b, c of equation in form:
     // (a)*x^2 + (b)*x + c = 0
  
     // give an equation: 1 + x - 3 x = - x ˆ 2
     int a = 0;
     int b = 0;
     int c = 0;
  
     int temp = 0;
     // Evaluates left side of equation
     while (!acceptCharacter(&lp, '=')) {
        // Multply by negative when - in front of term
        int mult = acceptCharacter(&lp, '-') ? -1 : 1;
        acceptCharacter(&lp, '+'); /*skip plus*/
  
        // 2x^2 + 3x + 7 = 2x^2
        if (acceptIdentifier(&lp)) {
           if (acceptCharacter(&lp, '^')) {
              /* (identifier)^(2/1/0) */
              if ((lp)->t.number == 0) {
                 c += mult;
              } else if ((lp)->t.number == 1) {
                 b += mult;
              } else {
                 a += mult;
              }
              acceptNumber(&lp);
           } else {
              /* (identifier) */
              b++;
           }
        } else {
           temp = (lp)->t.number * mult;
           acceptNumber(&lp);
           if (acceptIdentifier(&lp)) {
              if (acceptCharacter(&lp, '^')) {
                 /* (number)(identifier)^(2/1/0)*/
                 if ((lp)->t.number == 0) {
                    c += temp;
                 } else if ((lp)->t.number == 1) {
                    b += temp;
                 } else {
                    a += temp;
                 }
                 acceptNumber(&lp);
              } else {
                 /* (number)(identifier) */
                 b += temp;
              }
           } else {
              /* (number) */
              c += temp;
           }
        }
     }

     while ((lp) != NULL) {
        // Multply by negative when - in front of term
        int mult = acceptCharacter(&lp, '-') ? -1 : 1;
        acceptCharacter(&lp, '+'); /*skip plus*/
  
        if (acceptIdentifier(&lp)) {
           if (acceptCharacter(&lp, '^')) {
              /* (identifier)^(2/1/0) */
              if ((lp)->t.number == 0) {
                 c -= mult;
              } else if ((lp)->t.number == 1) {
                 b -= mult;
              } else {
                 a -= mult;
              }
              acceptNumber(&lp);
           } else {
              /* (identifier) */
              b--;
           }
        } else {
           temp = (lp)->t.number * mult;
           acceptNumber(&lp);
           if (acceptIdentifier(&lp)) {
              if (acceptCharacter(&lp, '^')) {
                 /* (number)(identifier)^(2/1/0)*/
                 if ((lp)->t.number == 0) {
                    c -= temp;
                 } else if ((lp)->t.number == 1) {
                    b -= temp;
                 } else {
                    a -= temp;
                 }
                 acceptNumber(&lp);
              } else {
                 /* (number)(identifier) */
                 b -= temp;
              }
           } else {
              /* (number) */
              c -= temp;
           }
        }
     }
     if (a == 0){
      *solution1 = *solution2 = -c / (double) b;
      return 1;
     }
     // (a)*x^2 + (b)*x + c = 0
     if (b == 0) {
        if (c == 0) {
           *solution1 = *solution2 = 0;
        } else if (c < 0) {
           *solution1 = sqrt(-c);
           *solution2 = -(*solution1);
        } else {
           return 0;
        }
        return 1;
     }
     double discriminant = b * b - 4 * a * c;
     if (discriminant < 0) {
        return 0;
     } else if (discriminant == 0) {
        *solution1 = *solution2 = -b / (double)2 * a;
     } else {
        *solution1 = ((-b) + sqrt(discriminant)) / (2 * a);
        *solution2 = ((-b) - sqrt(discriminant)) / (2 * a);
     }
     return 1;
  }
  
  void recognizeEquation() {
     char *ar;
     List tl, tl1;
     printf("give an equation: ");
     ar = readInput();
     while (ar[0] != '!') {
        tl = tokenList(ar);
        printList(tl);
        tl1 = tl;
        if (acceptEquation(&tl1) && tl1 == NULL) {
           if (hasSingleVariable(tl)) {
              int degree = getDegree(tl);
              double solution1, solution2;
              printf("this is an equation in 1 variable of degree %d\n", degree);
              if (degree == 1) {
                 if (solveEqDeg1(tl, &solution1)) {
                    printf("solution: %.3f\n", solution1);
                 } else {
                    printf("not solvable\n");
                 }
  
              } else if (degree == 2) {
                 if (solveEqDeg2(tl, &solution1, &solution2)) {
                    if (solution1 == solution2){
                     printf("solution: %.3f\n", solution1);
                    } else {
                     printf("solution: %.3f and %.3f\n", solution1, solution2);
                    }

                 } else {
                    printf("not solvable\n");
                 }
              } else {
                 printf("\n");
              }
           } else {
              printf("this is an equation, but not in 1 variable\n");
           }
  
        } else {
           printf("this is not an equation\n");
        }
  
        free(ar);
        freeTokenList(tl);
        printf("\ngive an equation: ");
        ar = readInput();
     }
     free(ar);
     printf("good bye\n");
  }
  
