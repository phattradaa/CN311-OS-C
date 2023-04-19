#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_THREADS 4

void *PrintHello(void *threadid)
{
    long tid;
    char name[9] = "Phattrada";
    char lastname[6] = "Mikota";
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", (tid+1));
    if (tid == 0) 
    {
        printf("%s\n",name);
    }
    else if (tid == 1)
    {
        printf("%s\n",lastname);
    }
    else if (tid == 2)
    {
        char fullname[16] = "Phattrada Mikota";
        int l = strlen(name) + strlen(lastname);
        int i;
        for (i = l - 1; i >= 0;i--)
        {
            printf("%c", fullname[i]);
        }
        printf("\n");
    }
    else if (tid == 3)
    {
        int ans = strlen(name) + strlen(lastname);
        printf("%d\n",ans);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    for (t = 0; t < NUM_THREADS; t++)
    {
        printf("In main: creating thread %ld\n", (t+1));
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}