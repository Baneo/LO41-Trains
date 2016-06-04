#ifndef MAINTEST_H
#define MAINTEST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct elem{
    int train;
    struct elem* next;
}Element;

typedef struct {
    Element* head;
    int length;
}FileTrain;

/* Function createTrainLine
*
*  This Function creates and initializes an empty train waiting line, takes nothing and
*  returns a TrainLine.
*
*  F : a TrainLine variable corresponding to the TrainLine the user wants to
*  create.
*/

FileTrain createFileTrain()
{
FileTrain F;
F.head = NULL;
F.length = 0;
return F;
}

/* Function addTrain
*
*  This function adds the specified train to the specified TrainLine, it takes a *  TrainLine variable and a train variable (number) and returns the same TrainLine
*  containing the new train at the end.
*
*  h : a TrainLine variable corresponding to the TrainLine where the user wants to
*  add a train.
*  t : a train
*/
Polygon addTrain(FileTrain f, int t)
{
	 Element* newElem = (Element*)malloc(sizeof(Element));
	 newElem -> train = t;
    if (f.length == 0)
    {
	    newElem -> next = NULL;
	    f.head = newElem;
		  f.length = 1;
    }
    else
    {   newElem->next = h.head;
      for (size_t i = 1; i < length; i++) {
        newElem->next = newElem->next->next;
        }
    newElem->next->next = newElem;
    newElem->next = NULL;
		f.length = f.length + 1;
    }

return h;
}

/* Function removeTrain
*
*  This function removes the first train of the waiting line.

*  f : a TrainLine, the one we want to delete the first train in line.
*/
// PAS ENCORE TERMINE!!!!!!!!!!!!!!
Polygon removeTrain(TrainLine t)
{

    Element* m = h.head;
    if (t.length == 0)
    {
        return t;
    }
    else if (t.length == 1)
    {
        t.length = 0;
        free(t.head);
        t.head = NULL;
        return h;
    }
	 else
        {
		  m -> next -> next = m -> next;
		  free(m);
        h.length = h.length - 1;

    }
return h;
}
/* Function PrintTrainLine
*
*This function prints a TrainLine on the console terminal, it takes a TrainLine and return nothing.
*
*t : A TrainLine that we want to explore.
*/

void PrintTrainLine(TrainLine t)
{
  Element* m = t.head;
  for (size_t i = 0; i < t.length; i++) {
    printf("Le train numéro %d est le train %d",i,m.train);
    m=m.next;
  }
}


void maintest()
{


}

#endif
