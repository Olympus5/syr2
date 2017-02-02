#include "reverse.h"

//Variable global contenant les nombres d'élément composant la liste chainé de caractère d'un fichier
int nbElement = 0;

int main(int argc, char* argv[])
{
  int fd, i;

  file_list* fl;
  file_list* tmp;

  //Si il y a des arguments on continue, sinon fin du programme
  if(argc > 1)
  {
    fd = open(argv[1], O_RDONLY);

    if(fd > -1)
    {
      printf("Ouverture réussi\n");

      fl = fileToList(fd);
      printf("Test\n");
      tmp = fl;

      reverse(&tmp);

      for(i = 0; i < nbElement; i++)
      {
        printf("%d => %s / %d éléments \n", tmp->index, tmp->buf, tmp->nbByte);
        tmp = tmp->next;
      }

      //cleanList(fl);

      close(fd);
    }
    else
    {
      printf("Impossible d'ouvrir le fichier %s\n", argv[1]);
      printf("Fichiers de test présent dans le dossier res/\n");
    }
  }

  return 0;
}

file_list* fileToList(int fd)
{
  //Variable local qui stocke le nombre de caractères retournés
  int c;

  file_list* header;
  file_list* currentElement;
  file_list* lastElement;

  header = currentElement = newElement();

  while(((c = read(fd, currentElement->buf, 1024)) > 0) && (currentElement != NULL))
  {
    currentElement->nbByte = c;
    currentElement->next = newElement();
    lastElement = currentElement;
    currentElement = currentElement->next;
  }

  //pintf("%s", lastElement->buf);
  //lastElement->next = NULL;
  //free(currentElement);

  return header;
}

int reverse(file_list** header)
{
  int ret = -1;
  int i,j;
  file_list* fl = *header;
  file_list* tmpFl;
  file_list* lastFl;
  char tmp;

  //Regarde si l'argument est diff de NULL
  if(header != NULL)
  {
      //Regarde qu'on a bien une liste non vide
      if(fl != NULL)
      {
        //Inverse les caractères dans les différents buffer
        for(i = 0; i < nbElement; i++)
        {
          for(j = 0; j < fl->nbByte/2; j++)
          {
            tmp = fl->buf[j];
            fl->buf[j] = fl->buf[((fl->nbByte)-j)-1];
            fl->buf[((fl->nbByte)-j)-1] = tmp;
          }

          fl = fl->next;
        }

        /*fl = *header;
        tmpFl = fl->next;
        fl->next = NULL;
        lastFl = fl;

        //Renverse la liste
        for(i = 0; i < nbElement; i++)
        {
          fl = tmpFl;
          tmpFl = fl->next;
          fl->next = lastFl;
          lastFl = fl;
        }*/

        ret = 1;
      }
  }

  return ret;
}

file_list* newElement()
{
  file_list* fl = (file_list*) malloc(sizeof(file_list));

  if(fl != NULL)
  {
    printf("création du maillon réussi\n");
    fl->next = NULL;
    nbElement++;
    fl->index = nbElement;
  }
  else
  {
    printf("création du maillon échoué\n");
  }

  return fl;
}

void freeElement(file_list* fl)
{
  free(fl);
  nbElement--;
}
