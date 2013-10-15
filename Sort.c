#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void read(char* nameInFile);
void Sort(int left, int right);
void write(char* nameOutFile);
void CreateTh();
void StartTh();
int* array;
int size;
pthread_t thread1, thread2;
bool Flag = false;
int lg, rg;

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    printf("Error\n");
    return 0;
  }
  read(argv[1]);

  struct timespec startone;
  clock_gettime(1, &startone);
  Sort(0, size - 1);
  struct timespec stopone;
  clock_gettime(1, &stopone);
  printf("one thread: %d sec %lu nanosec\n", abs(stopone.tv_sec - startone.tv_sec), labs(stopone.tv_nsec - startone.tv_nsec));

  read(argv[1]);

  struct timespec starttwo;
  clock_gettime(1, &starttwo);
  CreateTh();
  struct timespec stoptwo;
  clock_gettime(1, &stoptwo);
  printf("two thread: %d sec %lu nanosec\n", abs(stoptwo.tv_sec - starttwo.tv_sec), labs(stoptwo.tv_nsec - starttwo.tv_nsec));

  write(argv[2]);
  delete[] array;
  return 0;
}

static void *StartTh(void *vptr_args)
{
  int *i = (int *)vptr_args;
  if (*i == 0)
  Sort(0, lg);
  if (*i == 1)
  Sort(rg, size - 1);
}

void CreateTh()
{
  int par1 = 0;
  int par2 = 1;

  Flag = true;
  Sort(0, size - 1);
  Flag = false;

  pthread_create(&thread1, NULL, StartTh, &par1);
  pthread_create(&thread2, NULL, StartTh, &par2);
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
}

void Sort(int left, int right)
{
  int i, j, x, temp;
  i = left;
  j = right;
  x = array[(left + right) / 2];
  while(i < j)
  {
    while (array[i] < x) 
    {   
      i++;
    }
    while (array[j] > x) 
    {
      j--;
    }
    if (i <= j) 
    {
      temp = array[i];
      array[i] = array[j];
      array[j] = temp;
      i++;
      j--;
    }
  }
  if (Flag)
  {
    lg = j;
    rg = i;
    return;
  }
  else
  {
    if (left < j) 
    Sort(left, j);
    if (i < right) 
    Sort(i, right); 
  }
}

void write(char* nameOutFile)
{
  FILE *fp;
  fp = fopen(nameOutFile, "wt");
  fprintf(fp, "%d", size);
  fprintf(fp, "\n");
  for(int i = 0; i < size; i++)
  {
    fprintf(fp, "%d", array[i]);
    fprintf(fp, "\n");
  }
}

void read(char* nameInFile)
{
  FILE *fp;
  fp = fopen(nameInFile, "rt");
  fscanf(fp, "%d", &size);
  array = new int[size];
  for(int i = 0; i <= size; i++)
  {
    fscanf(fp, "%d", &array[i]);
  }
}
