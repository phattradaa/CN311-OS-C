#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_THREADS 16
#define MAX_VALUE 1000000
#include <time.h>
int counter1 = 0;
int counter2 = 0;
float time_time[NUM_THREADS];
void *PrintHello(void *threadid)
{
    clock_t start, end;
    int *id_ptr, taskid;
    sleep(1);
    id_ptr = (int *)threadid;
    taskid = *id_ptr;
    int i;
    int count = 0;
    start = clock();
    for (i = 1; i < (MAX_VALUE + 1); i++)
    {
        count++;
    }
    end = clock();
    time_time[taskid] = (float)(end - start) / CLOCKS_PER_SEC;
    counter1++;
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];
    int rc, t;

    for (t = 0; t < NUM_THREADS; t++)
    {
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        printf("Creating thread %d\n", t+1);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)taskids[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    int i,j;
    float min=1;
    int winner = 0;
    while (counter1 != 16)
    {
        counter2++;
    }

    for (i = 0; i < NUM_THREADS; i++)
    {
        printf("%d : %f\n", i, time_time[i]);
    }
    i = 0;
    for (i = 0; i < 3; i++)
    {
        min = 2;
        j = 0;
        winner = 0;
        for (j = 0; j < NUM_THREADS; j++)
        {
            if (min >= time_time[j])
            {
                min = time_time[j];
                winner = j;
            }
        }
        printf("The Winner Number %d is %d\n", i + 1, winner);
        time_time[winner] = 2;
    }

    pthread_exit(NULL);
}
