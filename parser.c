#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int** parser(char file_name[])
{
  char line[128];
  int size;
  bool first = true;
  FILE* input;
  printf("File in parser: %s\n", file_name);


  if((input = fopen(file_name, "r")) == NULL)
  {
    perror("Wrong input");
    return 1;
  }
  fgets(line, sizeof line, input);
  size = atoi(line);
  int output[size + 1][3];
  int number = 0, count = 0;
  while(fgets(line, sizeof line, input) != NULL)
  {
    printf("Line: %s\n", line);
    if (strncmp(line, "TGV", 3) == 0)
    {

      printf("tgv found\n");
      printf("number = %d count = %d\n", number, count);
      output[number][count] = 1;
      count ++;
    }
    else if (strncmp(line, "GL", 2) == 0)
    {
      printf("gl found\n");
      printf("number = %d count = %d\n", number, count);
      output[number][count] = 2;
      count ++;
    }
    else if (strncmp(line, "M", 1) == 0)
    {
      printf("m found\n");
      printf("number = %d count = %d\n", number, count);
      output[number][count] = 3;
      count ++;
    }
    else if (strncmp(line, "EST", 3) == 0)
    {
      printf("est found\n");
      printf("number = %d count = %d\n", number, count);
      output[number][count] = 5;
      count ++;
    }
    else if (strncmp(line, "A", 1) == 0)
    {
      printf("a  found\n");
      printf("number = %d count = %d\n", number, count);
      output[number][count] = 1;
      count ++;
    }
    else if (strncmp(line, "B", 1) == 0)
    {
      printf("b found\n");
      printf("number = %d count = %d\n", number, count);
      output[number][count] = 2;
      count ++;
    }
    else if (strncmp(line, "C", 1) == 0)
    {
      printf("c found\n");
      printf("number = %d count = %d\n", number, count);
      output[number][count] = 3;
      count ++;
    }
    else if (strncmp(line, "D", 1) == 0)
    {
      printf("d found\n");
      printf("number = %d count = %d\n", number, count);
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

    int i = 0,j = 0;
    for (i = 0; i < size; i++)
    {
      printf("i=%d\n", i);
      for(j = 0; j < 3; j++)
      {
        printf("j=%d\n",j);
        printf("tab output :%d\n", output[i][j]);
      }
    }




  fclose(input);
  return output;
}

#endif
