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
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

pthread_t tid[60];
pthread_mutex_t mutex;
pthread_cond_t LA,LC,LEST,ATGV,AM1,AM2,AGL,superviseur;

int shmid, i;
int print_full_info = 1;

typedef struct elem{
    int train;
    struct elem* next;
}Element;

typedef struct {
    Element* head;
    int length;
}FileTrain;

FileTrain *p;

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
FileTrain addTrain(FileTrain f, int t)
{
  int i;
	 Element* newElem = (Element*)malloc(sizeof(Element));
	 newElem -> train = t;
    if (f.length == 0)
    {
	    newElem -> next = NULL;
	    f.head = newElem;
		  f.length = 1;
    }
    else
    {   newElem->next = f.head;
      for (i = 1; i < f.length; i++) {
        newElem->next = newElem->next->next;
        }
    newElem->next->next = newElem;
    newElem->next = NULL;
		f.length = f.length + 1;
    }

return f;
}

/* Function removeTrain
*
*  This function removes the first train of the waiting line.

*  f : a TrainLine, the one we want to delete the first train in line.
*/
FileTrain removeTrain(FileTrain t)
{

    Element* m = t.head;
    if (t.length == 0)
    {
        return t;
    }
    else if (t.length == 1)
    {
        t.length = 0;
        free(t.head);
        t.head = NULL;
        return t;
    }
	 else
        {
		  m -> next -> next = m -> next;
		  free(m);
        t.length = t.length - 1;

    }
return t;
}
/* Function PrintTrainLine
*
*This function prints a TrainLine on the console terminal, it takes a TrainLine and return nothing.
*
*t : A TrainLine that we want to explore.
*/

void PrintTrainLine(FileTrain t)
{
  int i;
  Element* m = t.head;
  for (i = 0; i < t.length; i++) {
    printf("Le train numéro %d est le train %d",i+1,m->train);
    m=m->next;
  }
}


void* fonc_EST(void* arg)
{
  int *numero = (int*)arg;
  if(print_full_info == 1)
  {
    printf("valeur du tableau passé en argument: %d \n %d \n %d \n", numero[0], numero[1], numero[2]);
  }
  pthread_mutex_lock (&mutex);
  printf("Le Train Préviens le Chef qu'il est la.");
  addTrain(p[2],numero[0]);
  pthread_cond_signal(&superviseur);
  pthread_cond_wait(&LEST,&mutex);
  removeTrain(p[2]);
  printf("le train %d passe le Tunnel, et arrive dans la voie de Garage.", numero[0]);
  if (numero[4] == 0) /*TGV*/ {
    addTrain(p[3],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&ATGV, &mutex);
  }
  else if (numero[4] ==1) /*M*/ {
    addTrain(p[5],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AM1, &mutex);
  }
  else if (numero[4] == 2) /*GL*/ {
    addTrain(p[6],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AGL, &mutex);
  }
  printf("Le Train part vers sa destination finale.\n");
  pthread_mutex_unlock(&mutex);
  return 0;

}
void* fonc_A(void* arg)
{
  int *numero = (int*)arg;
  if(print_full_info == 1)
  {
    printf("valeur du tableau passé en argument: %d \n %d \n %d \n", numero[0], numero[1], numero[2]);

  }
  pthread_mutex_lock (&mutex);
  printf("Le Train Préviens le Chef qu'il est la.");
  addTrain(p[0],numero[0]);
  pthread_cond_signal(&superviseur);
  pthread_cond_wait(&LA,&mutex);
  removeTrain(p[0]);
  printf("le train %d l'aiguillage, et arrive dans la voie de Garage.", numero[0]);
  if (numero[4] == 0) /*TGV*/ {
    addTrain(p[3],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&ATGV, &mutex);
  }
  else if (numero[4] ==1) /*M*/ {
    addTrain(p[4],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AM1, &mutex);
  }
  else if (numero[4] == 2) /*GL*/ {
    addTrain(p[6],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AGL, &mutex);
  }
  printf("Le Train part vers sa destination finale.\n");
  pthread_mutex_unlock(&mutex);
  return 0;

}
void* fonc_C(void* arg)
{
  int *numero = (int*)arg;
  if(print_full_info == 1)
  {
    printf("valeur du tableau passé en argument: %d \n %d \n %d \n", numero[0], numero[1], numero[2]);
  }
  pthread_mutex_lock (&mutex);
  printf("Le Train Préviens le Chef qu'il est la.");
  addTrain(p[1],numero[0]);
  pthread_cond_signal(&superviseur);
  pthread_cond_wait(&LA,&mutex);
  removeTrain(p[0]);
  printf("le train %d l'aiguillage, et arrive dans la voie de Garage.", numero[0]);
  if (numero[4] == 0) /*TGV*/ {
    addTrain(p[3],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&ATGV, &mutex);
  }
  else if (numero[4] ==1) /*M*/ {
    addTrain(p[4],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AM1, &mutex);
  }
  else if (numero[4] == 2) /*GL*/ {
    addTrain(p[6],numero[0]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AGL, &mutex);
  }
  printf("Le Train part vers sa destination finale.\n");
  pthread_mutex_unlock(&mutex);
  return 0;
}

/*Fonctions ruling the Signals*/

void erreur(const char* msg) {

  fprintf(stderr,"%s\n",msg);

  }

void traitantSIGINT(int num) {
   if (num!=SIGTSTP){
     erreur("Pb sur SigInt...");
   }
  printf("\n---- Suppression des tables IPC-----\n");
  shmctl(shmid, IPC_RMID, NULL);
  signal(SIGINT,SIG_DFL);
  kill(getpid(),SIGINT);
  }

void traitantSIGTSTP(int num) {
 if (num!=SIGTSTP){
   erreur("Pb sur SigSTP...");
 }
    printf("\n---- Suppression des tables IPC-----\n");
    shmctl(shmid, IPC_RMID, NULL);
    signal(SIGTSTP,SIG_DFL);
    kill(getpid(),SIGTSTP);
  }


int main(void)
{
  /*variables utiles au développement du programme*/
  int i = 0,j = 0, number = 0, count = 0,x, Nb_A = 0, Nb_C = 0, Nb_EST = 0, size;
  /*variable utilisées pour les buffers de lecture du fichier*/
  char file_name[100];
  char line[128];
  /*variable d'entrée de la lecture du fichier et variable de sortie avec toutes les informations nécéssaires*/
  FILE* input;
  int ** output;
  /*tableau d'informations sur le train (depart, arrivée, type)*/
  int numero[3];

/*Handler signaux*/

signal(SIGINT,traitantSIGINT);
signal(SIGTSTP,traitantSIGTSTP);

  /*récupération du nom du fichier*/
  printf("Entrez le chemin du fichier a utiliser:\n");
  scanf("%s", file_name);
  printf("Fichier: %s\n", file_name);

  /*lecture du fichier*/
  if((input = fopen(file_name, "r")) == NULL)
  {
    perror("Wrong input");
    return 1;
  }

  fgets(line, sizeof line, input);
  /*acquisition de la taille du tableau output*/
  size = atoi(line);

  /*allocation de l'espace pour le tableau output*/
  output = malloc (size * sizeof (int *));
  for (x = 0;x < 3;x ++)
  {
    output[x] = malloc (3 * sizeof (int));
  }

  /*boucle de remplisage du tableau output*/
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

  /*vérification des informations*/
  if (print_full_info == 1)
  {
    for (i = 0; i < size; i++)
    {
      printf("i=%d\n", i);
      for(j = 0; j < 3; j++)
      {
        printf("j=%d\n",j);
        printf("MAIN tab output :%d\n", output[i][j]);
      }
    }
  }


  /*boucle de création du thread en fonction de la zone de départ*/
  for(number = 0;number < Nb_EST;number ++)

  {
    for (i = 0; i < size; i++)
    {
      if (output[i][1] == 5)
      {

        for(j = 0; j < 3; j++)
        {
          numero[j] = output[i][j];
        }
      }
    }
    /*verification du contenu avant de le passer au thread*/
    if(print_full_info == 1)
    {
      for (i = 0; i < 3 ; i++)
      {
        printf("contenu de numero, case %d :%d\n", i, numero[i]);
      }
    }
    /*creation du thread, envoi du tableau par cast en void* sur le pointeur du tableau, politique d'ordonnancement a faire ici ?*/

    if(!pthread_create(tid+number,0,(void*(*)())fonc_EST,(void*) numero))
    {
	     perror("pthread_create");
	     return EXIT_FAILURE;
    }

  }

  for(number = number;number < Nb_A + Nb_EST;number ++)
  {

    for (i = 0; i < size; i++)
    {
      if (output[i][1] == 1)
      {

        for(j = 0; j < 3; j++)
        {
          numero[j] = output[i][j];
        }
      }
    }
    /*verification du contenu avant de le passer au thread*/
    if(print_full_info == 1)
    {
      for (i = 0; i < 3 ; i++)
      {
        printf("contenu de numero, case %d :%d\n", i, numero[i]);
      }
    }
    /*creation du thread, envoi du tableau par cast en void* sur le pointeur du tableau, politique d'ordonnancement a faire ici ?*/

    if(!pthread_create(tid+number,0,(void*(*)())fonc_A,(void*) numero))
    {
	     perror("pthread_create");
	     return EXIT_FAILURE;
    }
  }

  for(number = number;number < Nb_C + Nb_A + Nb_EST;number ++)
  {

    for (i = 0; i < size; i++)
    {
      if (output[i][1] == 3)
      {

        for(j = 0; j < 3; j++)
        {
          numero[j] = output[i][j];
        }
      }
    }
    /*verification du contenu avant de le passer au thread*/
    if(print_full_info == 1)
    {
      for (i = 0; i < 3 ; i++)
      {
        printf("contenu de numero, case %d :%d\n", i, numero[i]);
      }
    }
    /*creation du thread, envoi du tableau par cast en void* sur le pointeur du tableau, politique d'ordonnancement a faire ici ?*/

    if(!pthread_create(tid+number,0,(void*(*)())fonc_C,(void*) numero))
    {
	     perror("pthread_create");
	     return EXIT_FAILURE;
    }
  }

  for(number = 0;number < Nb_EST + Nb_A + Nb_C;number ++)
  {
      pthread_join(tid[number],NULL);
  }

  /*
  shmid = shmget(IPC_PRIVATE, sizeof(FileTrain), 0666);
  p = (FileTrain *)shmat(shmid, NULL, 0);
  FT1 = createFileTrain();
  printf("FileTrain Crée");
  FT1 = addTrain(FT1, 12);
  FT1 = addTrain(FT1, 15);
  FT1 = addTrain(FT1, 18);
  PrintTrainLine(FT1);
  FT1 = removeTrain(FT1);
  PrintTrainLine(FT1);*/
  shmctl(shmid, IPC_RMID, NULL);
  return EXIT_SUCCESS;
}
