#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* Authors:
   Abhinav Sasikumar
   Farhan Syed

   2023-09-28

   Compilation: gcc partOne.c -o partOne.out -lpthread
   Execution:   ./partOne.out


*/
int buffer = 0; // shared integer
pthread_mutex_t mutex;

void *thread_func(void *arg)
{
    long tid = pthread_self();
    int pid = getpid();
    int thread_accesses = 0;
    while (1)
    {

        if (!pthread_mutex_trylock(&mutex))
        {
            // Mutex is successfully acquired.
            if (buffer < 15)
            {
                printf("TID: %ld, PID: %i, Buffer: %i \n", tid, pid, buffer);
                buffer++; // increment the buffer

                thread_accesses++; // increment number of accesses for this thread
            }
            else // max buffer size is reached, so break the loop.
            {
                pthread_mutex_unlock(&mutex);

                break;
            }
            pthread_mutex_unlock(&mutex);
        }
        sleep(1); // Allow for other threads to execute
    }
    printf("TID: %ld worked on the buffer %d times \n", tid, thread_accesses);
}
int main()
{
    // Create 3 threads
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, thread_func, NULL);
    pthread_create(&t2, NULL, thread_func, NULL);
    pthread_create(&t3, NULL, thread_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    printf("Total buffer accesses: %d \n", buffer);

    pthread_mutex_destroy(&mutex);
}