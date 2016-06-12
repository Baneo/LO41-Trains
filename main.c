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

#define nbThreads 15

pthread_t tid[nbThreads];
pthread_mutex_t mutex;
pthread_cond_t LA,LC,LEST,ATGV,AM1,AM2,AGL,superviseur;



int shmid;

int print_full_info = 0;

typedef struct elem{
    int train;
    struct elem* next;
}Element;

typedef struct {
    Element* head;
    int length;
}FileTrain;

FileTrain p[7];

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
        t.head = t.head ->next;
        t.length = t.length - 1;
        free(m);
        m=NULL;
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
  if (t.length != 0){
  for (i = 0; i < t.length; i++) {
    printf("Le Train numero %d est le train %d\n",i+1,m->train);
    m=m->next;
  }
}
  else
  { printf("File de Trains vide\n");
}
}




void* fonc_EST(int* arg)
{
  int *numero = (int*)arg;
  if(print_full_info == 1)
  {
    printf("valeur du tableau passé en argument(EST):no%d\n type:%d \n dpt:%d \n arr:%d\n",numero[3], numero[0], numero[1], numero[2]);
  }

  pthread_mutex_lock (&mutex);
  p[2] = addTrain(p[2],numero[3]);
  pthread_cond_wait(&LEST,&mutex);
  p[2] = removeTrain(p[2]);
  printf("le Train %d passe le tunnel, et arrive dans la voie de garage.\n", numero[3]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();
  if (numero[0] == 1)
  {
    p[3] = addTrain(p[3],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&ATGV, &mutex);
    p[3] = removeTrain(p[3]);
  }
  else if (numero[0] == 3)
  {
    p[4] = addTrain(p[4],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AM1, &mutex);
    p[4] = removeTrain(p[4]);
  }
  else if (numero[0] == 2)
  {
    p[6] = addTrain(p[6],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AGL, &mutex);
    p[6] = removeTrain(p[6]);
  }

  printf("Le Train %d part vers sa destination finale.\n", numero[3]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();

  pthread_mutex_unlock(&mutex);
  pthread_cond_signal(&superviseur);

  return 0;

}




void* fonc_A(void* arg)
{
  int *numero = (int*)arg;
  if(print_full_info == 1)
  {
    printf("valeur du tableau passé en argument:no%d\n type:%d \n dpt:%d \n arr:%d\n",numero[3], numero[0], numero[1], numero[2]);
  }

  pthread_mutex_lock (&mutex);
  p[0] = addTrain(p[0],numero[3]);
  pthread_cond_wait(&LA,&mutex);
  p[0] = removeTrain(p[0]);
  printf("le Train %d a passé l'aiguillage, et arrive dans la voie de garage.\n", numero[3]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();
  if (numero[0] == 1) {
    p[3] = addTrain(p[3],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&ATGV, &mutex);
    p[3] = removeTrain(p[3]);
  }
  else if (numero[0] == 3) {
    p[5] = addTrain(p[5],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AM2, &mutex);
    p[5] = removeTrain(p[5]);
  }
  else if (numero[0] == 2) {
    p[6] = addTrain(p[6],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AGL, &mutex);
    p[6] = removeTrain(p[6]);
  }
  printf("Le Train %d part vers sa destination finale.\n", numero[3]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();
  pthread_cond_signal(&superviseur);
  pthread_mutex_unlock(&mutex);
    return 0;

}
void* fonc_C(void* arg)
{
  int *numero = (int*)arg;

  if(print_full_info == 1)
  {
    printf("valeur du tableau passé en argument: no: %d type:%d \n dpt:%d \n arr:%d\n", numero[3], numero[0], numero[1], numero[2]);
  }

  pthread_mutex_lock (&mutex);
  p[1] = addTrain(p[1],numero[3]);
  pthread_cond_wait(&LC,&mutex);
  p[1] = removeTrain(p[1]);

  printf("le Train %d passe l'aiguillage, et arrive dans la voie de garage.\n", numero[3]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();
  if (numero[0] == 1) /*Pour un TGV*/ {
    p[3] = addTrain(p[3],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&ATGV, &mutex);
    p[3] = removeTrain(p[3]);
  }
  else if (numero[0] == 3) /*Pour un M */{
    p[5] = addTrain(p[5],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AM2, &mutex);
    p[5] = removeTrain(p[5]);
  }
  else if (numero[0] == 2) /*Pour une GL*/ {
    p[6] = addTrain(p[6],numero[3]);
    pthread_cond_signal(&superviseur);
    pthread_cond_wait(&AGL, &mutex);
    p[6] = removeTrain(p[6]);
  }
  else
  {
    printf("prob fonc c\n");
  }
  printf("Le Train %d part vers sa destination finale.\n", numero[3]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();
  pthread_cond_signal(&superviseur);
  pthread_mutex_unlock(&mutex);
  return 0;
}

void* fonc_S()
{
  while (1){
      pthread_cond_wait(&superviseur, &mutex);
      printf("Intervention Superviseur\n");

      if(p[2].head != NULL)
      {
        /* Pour un train sur la ligne EST*/
        pthread_cond_signal(&LEST);
      }
      else if(p[3].head != NULL)
      {
        /* Pour un train sur la voie de garage TGV*/
        pthread_cond_signal(&ATGV);
      }
      else if(p[6].head != NULL)
      {
        /* Pour un train dans la voie de garage GL*/
        pthread_cond_signal(&AGL);
      }
      else if(p[0].head != NULL)
      {
        /* Pour un train sur la voie A*/
        pthread_cond_signal(&LA);
      }
      else if(p[1].head != NULL)
      {
        /* Pour un train sur la voie C*/
        pthread_cond_signal(&LC);
      }
      else if(p[4].head != NULL)
      {
        /* Pour un train sur la voie de garage M, direction OUEST*/
        pthread_cond_signal(&AM1);
      }
      else if(p[5].head != NULL)
      {
        /* pour un train sur la voie de garege M, direction EST*/
        pthread_cond_signal(&AM2);
      }
      else
      {
        printf("Tous les trains sont passés !\n");
        return EXIT_SUCCESS;
      }
    }
}

/*Fonctions erreur*/

void erreur(const char* msg) {

  fprintf(stderr,"%s\n",msg);

  }

/* Déclaration des traitants pour SITSTP et SIGINT*/
void traitantSIGINT(int num) {
   if (num!=SIGINT){
     erreur("Probleme sur SigInt...");
   }
  printf("\n---- Suppression des tables IPC-----\n");
  printf("\n---- Au revoir, et a bientôt dans notre Glorieux Programme SNCF ;) -----\n");
  shmctl(shmid, IPC_RMID, NULL);
  signal(SIGINT,SIG_DFL);
  kill(getpid(),SIGINT);
  }

void traitantSIGTSTP(int num) {
 if (num!=SIGTSTP){
   erreur("Probleme sur SigSTP...");
 }
    printf("\n---- Suppression des tables IPC-----\n");
    printf("\n---- Malheureux. Il ne fallait pas faire ça. Tout est compromis. -----\n");
    shmctl(shmid, IPC_RMID, NULL);
    signal(SIGTSTP,SIG_DFL);
    kill(getpid(),SIGTSTP);
  }


/* FONCTION MAIN*/


int main(void)
{
  /*variables utiles au développement du programme*/
  int i = 0, j = 0, numtid = 0,nb_files = 0, number = 0, count = 0,x, Nb_A = 0, Nb_C = 0, Nb_EST = 0, size;

  /*variable utilisées pour les buffers de lecture du fichier*/
  char file_name[100];
  char line[128];

  /*variable d'entrée de la lecture du fichier et variable de sortie avec toutes les informations nécéssaires*/
  FILE* input;
  int ** output;

  /* création des files de trains*/
  for (nb_files = 0;nb_files<7;nb_files++)
  {
    p[nb_files] = createFileTrain();
  }

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
  /*lecture de la première ligne pour avoir le nombre de trains */
  fgets(line, sizeof line, input);
  /*initialisation de la taille du tableau output*/
  size = atoi(line);

  /*allocation de l'espace pour le tableau output*/
  output = malloc (size * sizeof (int *));

  for (x = 0;x < size;x ++)
  {
    output[x] = malloc (4 * sizeof (int));
  }

  /*boucle de remplisage du tableau output*/
  while(fgets(line, sizeof line, input) != NULL)
  {
    /* si le train est un TGV*/
    if (strncmp(line, "TGV", 3) == 0)
    {
      output[number][count] = 1;
      count ++;
    }
    /* si le train est une grande ligne*/
    else if (strncmp(line, "GL", 2) == 0)
    {
      output[number][count] = 2;
      count ++;
    }
    /*si le train est un train de marchandises*/
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
    /*gestion du nombre de trains, de l'entrée et de la sortie de chaque train*/
    if (count == 3)
    {
      count = 0;
      number ++;
    }

  }
  /*fermeture du fichier*/
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
    for (i = 0; i < size; i++)
    {
      if (output[i][1] == 5)
      {
        output[i][3] = i+1;
        /*creation du thread, envoi du tableau par cast en void* sur le pointeur du tableau*/
            pthread_create(tid+numtid,0,(void*(*)())fonc_EST,(void*) output[i]);
            numtid++;
      }

      else if (output[i][1] == 1)
      {
        output[i][3] = i+1;
        /*creation du thread, envoi du tableau par cast en void* sur le pointeur du tableau*/
        pthread_create(tid+numtid,0,(void*(*)())fonc_A,(void*) output[i]);
            numtid++;
      }

      else if (output[i][1] == 3)
      {
        output[i][3] = i+1;
        /*creation du thread, envoi du tableau par cast en void* sur le pointeur du tableau*/
        pthread_create(tid+numtid,0,(void*(*)())fonc_C,(void*) output[i]);
        numtid++;
      }
  }

    /*creation du thread superviseur*/
    i = pthread_create(tid+nbThreads,0,(void*(*)())fonc_S,NULL);
    if(i == 1)
    {
      perror("superviseur_thread_creation");
      return EXIT_FAILURE;
    }
  sleep(2);
  printf("Trains de la voie EST:\n");
  PrintTrainLine(p[2]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();
  getchar();
  printf("Trains de la voie A:\n");
  PrintTrainLine(p[0]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();
  printf("Trains de la voie C:\n");
  PrintTrainLine(p[1]);
  printf("Appuyez sur entrée pour continuer\n");
  getchar();

  pthread_cond_signal(&superviseur);

  for(number = 0;number < Nb_EST + Nb_A + Nb_C + 1;number ++)
  {
      pthread_join(tid[number],NULL);
  }
  PrintTrainLine(p[6]);
  shmctl(shmid, IPC_RMID, NULL);
  return EXIT_SUCCESS;
}
