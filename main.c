#define _XOPEN_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>
#include "maintest.h"



pthread_t tid[60];

void fonc_EST()
{

}
void fonc_A()
{

}
void fonc_C()
{

}



int main(void)
{


  int i = 0,j = 0, number = 0, count = 0,x;
  int Nb_A = 0, Nb_C = 0, Nb_EST = 0;
  char file_name[100] = "";
  char line[128];
  int size;
  FILE* input;
  int ** output;
  printf("Entrez le chemin du fichier a utiliser:\n");
  gets(file_name);
  printf("Fichier: %s\n", file_name);


  if((input = fopen(file_name, "r")) == NULL)
  {
    perror("Wrong input");
    return 1;
  }
  fgets(line, sizeof line, input);
  size = atoi(line)+1;


  output = malloc (size * sizeof (int *));


  for (x = 0;x < 3;x ++)
  {
    output[x] = malloc (3 * sizeof (int));
  }


  while(fgets(line, sizeof line, input) != NULL)
  {
    if (strncmp(line, "TGV", 3) == 0)
    {
      output[number][count] = 1;
      count ++;
    }
    else if (strncmp(line, "GL", 2) == 0)
    {
      output[number][count] = 2;
      count ++;
    }
    else if (strncmp(line, "M", 1) == 0)
    {
      output[number][count] = 3;
      count ++;
    }
    else if (strncmp(line, "EST", 3) == 0)
    {
      output[number][count] = 5;
      if (count == 1)
      {
        Nb_EST ++;
      }
      count ++;
    }
    else if (strncmp(line, "A", 1) == 0)
    {
      output[number][count] = 1;
      if (count == 1)
      {
        Nb_A ++;
      }
      count ++;
    }
    else if (strncmp(line, "B", 1) == 0)
    {
      output[number][count] = 2;
      count ++;
    }
    else if (strncmp(line, "C", 1) == 0)
    {
      output[number][count] = 3;
      if(count == 1)
      {
        Nb_C ++;
      }
      count ++;
    }
    else if (strncmp(line, "D", 1) == 0)
    {
      output[number][count] = 4;
      count ++;
    }
    else
    {
      printf("Train number %d\n", number + 1);
    }
    if (count == 3)
    {
      count = 0;
      number ++;
    }

  }
  fclose(input);

  for (i = 0; i < 3; i++)
  {
    printf("i=%d\n", i);
    for(j = 0; j < 3; j++)
    {
      printf("j=%d\n",j);
      printf("MAIN tab output :%d\n", output[i][j]);
    }
  }





  for(number = 0;number < Nb_EST;number ++)
  {
    pthread_create(tid+number,0,void*(*)())fonc_EST,(void*)number);
  }

  for(number = 0;number < Nb_A;number ++)
  {
    pthread_create(tid+number,0,void* (*fonc_A()),(void*)number);
  }

  for(number = 0;number < Nb_C;number ++)
  {
    pthread_create(tid+number,0,void* (*fonc_C()),(void*)number);
  }

  for(number = 0;number < Nb_EST + Nb_A + Nb_C;number ++)
  {
      pthread_join(tid[number],NULL);
  }





  maintest();
  return EXIT_SUCCESS;
}
