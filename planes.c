/* file:    */
/* author:  */
/* date:    */
/* version: */

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

int main(int argc, char *argv[]) {
  
  int NumberOfPlanes;
  Queue runway = newQueue(7);
  Stack hangar = newStack(5);

  scanf("%d", &NumberOfPlanes);
  for (int i = 0; i < NumberOfPlanes; i++){
    int curPlane;
    char status[4]; 
    scanf("%d %s", &curPlane, status);
    
    if (status[0] == 'n'){
      // Ready to leave
      enqueue(curPlane, &runway);
    } else {
      // Needs repair
      push(curPlane, &hangar);
    }

    // Hangar reached its full capacity
    if (hangar.size == hangar.top){
      clearRunway(&runway);
      clearHangair(&hangar);
    }

    // Runway reached its full capacity
    if (runway.front == runway.size){
      clearRunway(&runway);
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
  while (runway->front != runway->back){
    printf("%d\n", dequeue(runway));
  }
}

// Function empties stack and outputs every plane
void clearHangair(Stack *hangair){
  while (hangair->top > 0){
    printf("%d\n", pop(hangair));
  }
} 