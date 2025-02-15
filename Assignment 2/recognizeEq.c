/* file:    */
/* author:  */
/* date:    */
/* version: */

/* Description:
    Program evaluates if the input is an equation of
    one variable and outputs its degree.
  */


 #include <stdio.h>  /* getchar, printf */
 #include <stdlib.h> /* NULL */
 #include <string.h> /*strcmp*/
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
       if (hasSingleVariable(tl)){
         printf("this is an equation in 1 variable of degree %d\n", getDegree(tl));
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
 