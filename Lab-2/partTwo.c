#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

/* Authors:
   Abhinav Sasikumar
   Farhan Syed

   2023-09-28

   Compilation: gcc partTwo.c -o partTwo.out -lpthread
   Execution:   ./partTwo.out


*/
int MAX = 5;
int *buffer;
sem_t *readSemaphore;
sem_t *writeSemaphore;

// Function used by readers
void readerProcess(int pid)
{
   while (1)
   {
      if (*buffer >= MAX) // Break loop when max is exceeded
      {
         exit(0);
         break;
      }
      sem_wait(writeSemaphore); // wait for writeSemaphore

      int semValue;
      sem_getvalue(readSemaphore, &semValue); // Get value of readSemaphore
      if (semValue == 1) // Unlocked
      {
         sem_wait(readSemaphore); // Lock the semaphore
         printf("The first reader acquires the lock \n");
      }
      printf("Reader (%d) reads the value %d \n", pid, *buffer);

      if (semValue == 0) // locked
      {
         sem_post(readSemaphore); // Release the readSemaphore
         printf("The last reader releases the lock \n");
      }

      sem_post(writeSemaphore); // Release the writeSemaphore
      sleep(2);
   }
}
// Function used by writers
void writerProcess(int pid)
{
   while (1)
   {
      sem_wait(writeSemaphore); // lock
      printf("The writer acquires the lock \n");

      (*buffer)++;
      printf("The writer (%d) writes the value %d \n", pid, *buffer);

      sem_post(writeSemaphore); // unlock
      printf("The writer releases the lock. \n");
      if (*buffer >= MAX)
      {
         exit(0);
         break;
      }
      sleep(1);
   }
}

int main()
{
   sem_unlink("/write");
   sem_unlink("/read");

   // Creating a shared memory
   int shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
   buffer = shmat(shm_id, NULL, 0); // Assign pointer to shared memory. Shared memeory created by main process
   *buffer = 0;

   // Need two semaphores. Writers only use writeSemaphore. Readers use both. 
   writeSemaphore = sem_open("/write", O_CREAT | O_RDWR, 0600, 1); // 0 means locked, 1 means unlocked
   readSemaphore = sem_open("/read", O_CREAT | O_RDWR, 0600, 1);

   pid_t writer = fork();

   // Start the different processes
   if (writer == 0)
   {
      writerProcess(getpid());
      exit(0);
   }
   pid_t reader1 = fork();
   if (reader1 == 0)
   { // CHILD
      readerProcess(getpid());
      exit(0);
   }
   pid_t reader2 = fork();
   if (reader2 == 0)
   {
      readerProcess(getpid());
      exit(0);
   }

   while (wait(NULL) > 0)
      ;

   sem_close(readSemaphore);
   sem_close(writeSemaphore);
   shmctl(shm_id, IPC_RMID, NULL);
   exit(0);

   return 0;
}