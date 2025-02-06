/* file :    planes.c */
/* author :  Samuel Mikulas (s.mikulas@student.rug.nl) */
/* date :    Tue 4 Feb 2024 */
/* version:  1.0 */

/* Description:
    Program takes n planes and a status for repair of each plain (y/n)
    and outputs the order in which planse leave the airport. It uses 
    stack to track hangair and queue to track runway. 
 */

#include <stdio.h>
#include <stdlib.h>

#include "LibStack.h"
#include "LibQueue.h"

void clearRunway(Queue *runway);
void clearHangair(Stack *hangair);

#define RUNWAY_CAPACITY  7
#define HANGAIR_CAPACITY 5

int main(int argc, char *argv[]) {
  
  int NumberOfPlanes;
  Queue runway = newQueue(RUNWAY_CAPACITY);
  Stack hangar = newStack(HANGAIR_CAPACITY);

  // Keep track of planes on runway and hangair
  int runwaySize = 0;
  int hangarSize = 0;

  scanf("%d", &NumberOfPlanes);
  for (int i = 0; i < NumberOfPlanes; i++){
    int curPlane;
    char status[4]; 
    scanf("%d %s", &curPlane, status);
    
    if (status[0] == 'n'){
      // Ready to leave
      enqueue(curPlane, &runway);
      runwaySize++;
    } else {
      // Needs repair
      push(curPlane, &hangar);
      hangarSize++;
    }

    // Hangar reached its full capacity
    if (hangarSize == HANGAIR_CAPACITY){
      clearRunway(&runway);
      clearHangair(&hangar);
      hangarSize = runwaySize = 0;
    }

    // Runway reached its full capacity
    if (runwaySize == RUNWAY_CAPACITY){
      clearRunway(&runway);
      runwaySize = 0;
    }
  }

  // Clear remaining planes
  clearRunway(&runway);
  clearHangair(&hangar);

  freeQueue(runway);
  freeStack(hangar);
  return 0;
}

// Function empties queue and outputs every plane
void clearRunway(Queue *runway){
  while (!isEmptyQueue(*runway)){
    printf("%d\n", dequeue(runway));
  }
}

// Function empties stack and outputs every plane
void clearHangair(Stack *hangair){
  while (!isEmptyStack(*hangair)){
    printf("%d\n", pop(hangair));
  }
} 