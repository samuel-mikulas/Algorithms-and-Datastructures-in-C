
/* file :    */
/* author :  */
/* date :    */
/* version:  */

/* Description:
    Program evaluates if the input is an equation of
    one variable and outputs its degree. If the degree of equation is 1, 
    program also solves (detects that equation is not solvable) the equation.
  */


  #include <stdio.h>  /* getchar, printf */
  #include <stdlib.h> /* NULL */
  #include <string.h> /*strcmp*/
  
  #include "scanner.h"
  #include "evalEq.h"
  
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
  
  int getDegree(List lp){
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
 
  int hasSingleVariable(List lp){
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
  int solveEq(List lp, double *solution){
 
   // Keeps track of left and right hand side of equation
   // (left)*identifier = (right)
   int left = 0;
   int right = 0;
   int temp = 0;
   while (!acceptCharacter(&lp, '=')){
 
     // Multply by negative when - in front of term
     int mult = acceptCharacter(&lp, '-') ? -1 : 1;
     acceptCharacter(&lp, '+');  /*skip plus*/
 
 
     if (acceptIdentifier(&lp)){
       if (acceptCharacter(&lp, '^')){
         /* (identifier)^(1/0) */
         if ((lp)->t.number == 0){
           right -= mult;
         } else {
           left += mult*(lp)->t.number;
         }
         acceptNumber(&lp);
       } else {
         /* (identifier) */
         left += mult;
       }
     } else {
       temp = (lp)->t.number * mult;
       acceptNumber(&lp);
       if (acceptIdentifier(&lp)){
         if (acceptCharacter(&lp, '^')){
           /* (number)(identifier)^(1/0)*/
           if ((lp)->t.number == 0){
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
  
   while ((lp) != NULL){
 
     int mult = acceptCharacter(&lp, '-') ? -1 : 1;
     acceptCharacter(&lp, '+');
 
     if (acceptIdentifier(&lp)){
       if (acceptCharacter(&lp, '^')){
         /* (identifier)^(1/0) */
         if (lp->t.number == 0){
           right += mult;
         } else {
           left -= mult*(lp)->t.number;
         }
         acceptNumber(&lp);
       } else {
         left -= mult;
       }
     } else {
       temp = (lp)->t.number * mult;
       acceptNumber(&lp);
       if (acceptIdentifier(&lp)){
         if (acceptCharacter(&lp, '^')){
           /* (identifier) ^ (1/0) */
           if ((lp)->t.number == 0){
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
   if (left == 0){
     return 0;
   }
 
   /* Converts solutions in range [-0.0005, 0] to 0*/
   *solution = right / (double) left;
   if (-0.0005 <= *solution && *solution <= 0){
    *solution = 0.0;
   }
   return 1;
  }
 
  void recognizeEquation(){
    char *ar;
    List tl, tl1;
    printf("give an equation: ");
    ar = readInput();
    while (ar[0] != '!') {
      tl = tokenList(ar);
      printList(tl);
      tl1 = tl;
      if (acceptEquation(&tl1) && tl1 == NULL) {
        if (hasSingleVariable(tl)){
          int degree = getDegree(tl);
          double solution;
          printf("this is an equation in 1 variable of degree %d\n", degree);
 
          if (degree == 1){
            if (solveEq(tl, &solution)){
              printf("solution: %.3f\n", solution);
            } else {
             printf("not solvable\n");
            }
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
  