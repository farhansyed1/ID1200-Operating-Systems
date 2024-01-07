/* Authors:
   Abhinav Sasikumar
   Farhan Syed

   2023-11-20

   Compilation: gcc diskscheduling.c -o diskscheduling.out
   Execution:   ./diskscheduling.out

   Question 1: Write a program that implements the following disk-scheduling algorithms:
   FCFS, SSTF, SCAN, C-SCAN, Look and C-Look.

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int numberOfCylinders;
int numberOfRequests;

/*
   First Come First Serve Disk Scheduling Algorithm
*/
long FCFS(int serviceQueue[], int headPosition)
{
   long headMovements = 0;
   int currHeadPosition = headPosition;
   for (int i = 0; i < numberOfRequests; i++)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
   }
   return headMovements;
}

/*
   SSTF Disk Scheduling Algorithm
*/
long SSTF(int serviceQueue[], int headPosition)
{
   long headMovements = 0;
   int currHeadPosition = headPosition;
   int visitedList[1000] = {0};

   for (int i = 0; i < numberOfRequests; i++) // Go through serviceQueue
   {
      int shortestSeek = INT_MAX;
      int closestIndex = -1;

      for (int j = 0; j < numberOfRequests; j++) // Go through whole service queue to find the index with the minimum seek time
      {
         if (visitedList[j] == 0)
         {
            int seekDistance = abs(serviceQueue[j] - currHeadPosition);
            if (seekDistance < shortestSeek) // If the seek distance is smaller than the shortestSeek, the current seekDistance becomes the new shortest distance.
            {
               shortestSeek = seekDistance;
               closestIndex = j;
            }
         }
      }

      if (closestIndex != -1) // A closest index has been found
      {
         headMovements += shortestSeek;
         currHeadPosition = serviceQueue[closestIndex]; // Move the head to the closest index position
         visitedList[closestIndex] = 1;
      }
   }

   return headMovements;
}

/*
   Compare method for quicksort sorting function
*/
int compare(const void *a, const void *b)
{
   return (*(int *)a - *(int *)b);
}

/*
   SCAN Disk Scheduling Algorithm
*/
long SCAN(int serviceQueue[], int headPosition)
{
   long headMovements = 0;
   int currHeadPosition = headPosition;
   int goToRight = 1;

   // Sort the array
   qsort(serviceQueue, numberOfRequests, sizeof(int), compare);

   // Find the correct index that corresponds to the headPosition.
   int i;
   int initialIndex;
   for (initialIndex = 0; initialIndex < numberOfRequests; initialIndex++)
   {
      if (serviceQueue[initialIndex] >= currHeadPosition)
      {
         i = initialIndex; // save the index to start at
         break;
      }
   }

   // Go all the way to the right through the service queue
   while (i < numberOfRequests)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i++;
   }

   // Go to very end of disk
   headMovements += abs(serviceQueue[numberOfRequests-1] - currHeadPosition);
   currHeadPosition = numberOfCylinders-1;

   // Right end is now reached.

   // Go through the elements to the left of the head position.
   i = initialIndex - 1;

   while (i >= 0)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i--;
   }

   return headMovements;
}

/*
   C-SCAN Disk Scheduling Algorithm
*/
long CSCAN(int serviceQueue[], int headPosition)
{
   long headMovements = 0;
   int currHeadPosition = headPosition;
   int goToRight = 1;

   // Sort the array
   qsort(serviceQueue, numberOfRequests, sizeof(int), compare);

   // Find the correct index that corresponds to the headPosition.
   int i;
   int initialIndex;
   for (initialIndex = 0; initialIndex < numberOfRequests; initialIndex++)
   {
      if (serviceQueue[initialIndex] >= currHeadPosition)
      {
         i = initialIndex; // save the index to start at
         break;
      }
   }

   // Go all the way to the right through the service queue
   while (i < numberOfRequests)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i++;
   }
   // Go to very end of disk
   headMovements += abs(serviceQueue[numberOfRequests-1] - currHeadPosition);
   currHeadPosition = numberOfCylinders-1;

   // Right end is now reached.

   // Go back to index 0, and then go through the elements to the right.
   i = 0;

   while (i < numberOfRequests)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i++;
   }

   return headMovements;
}

long Look(int serviceQueue[], int headPosition)
{
   long headMovements = 0;
   int currHeadPosition = headPosition;
   int goToRight = 1;

   // Sort the array
   qsort(serviceQueue, numberOfRequests, sizeof(int), compare);

   // Find the correct index that corresponds to the headPosition.
   int i;
   int initialIndex;
   for (initialIndex = 0; initialIndex < numberOfRequests; initialIndex++)
   {
      if (serviceQueue[initialIndex] >= currHeadPosition)
      {
         i = initialIndex; // save the index to start at
         break;
      }
   }

   // Go all the way to the right through the service queue
   while (i < numberOfRequests)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i++;
   }
   // Right end is now reached. We do not go to the end of the disk.

   // Go through the elements to the left of the head position.
   i = initialIndex - 1;

   while (i >= 0)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i--;
   }

   return headMovements;
}

long CLook(int serviceQueue[], int headPosition)
{
   long headMovements = 0;
   int currHeadPosition = headPosition;
   int goToRight = 1;

   // Sort the array
   qsort(serviceQueue, numberOfRequests, sizeof(int), compare);

   // Find the correct index that corresponds to the headPosition.
   int i;
   int initialIndex;
   for (initialIndex = 0; initialIndex < numberOfRequests; initialIndex++)
   {
      if (serviceQueue[initialIndex] >= currHeadPosition)
      {
         i = initialIndex; // save the index to start at
         break;
      }
   }

   // Go all the way to the right through the service queue
   while (i < numberOfRequests)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i++;
   }
   // Right end is now reached. We do not go to the end of the disk.

   // Go back to index 0, and then go through the elements to the right.
   i = 0;

   while (i < numberOfRequests)
   {
      headMovements += abs(serviceQueue[i] - currHeadPosition);
      currHeadPosition = serviceQueue[i];
      i++;
   }

   return headMovements;
}

int main(int argc, char const *argv[])
{
   
   numberOfCylinders = 5000;
   numberOfRequests = 1000;
   
   srand( time(NULL) );
   int sizeOfQueue = numberOfRequests;
   int serviceQueue[sizeOfQueue];

   // Generate the random numbers
   for (int i = 0; i < sizeOfQueue; i++)
   {
      serviceQueue[i] = (rand() % numberOfCylinders);
   }
   int headPosition = atoi(argv[1]);

   long fcfsResult = FCFS(serviceQueue, headPosition);
   long sstfResult = SSTF(serviceQueue, headPosition);
   long scanResult = SCAN(serviceQueue, headPosition);
   long cscanResult = CSCAN(serviceQueue, headPosition);
   long lookResult = Look(serviceQueue, headPosition);
   long clookResult = CLook(serviceQueue, headPosition);

   printf("FCFS head movements: %ld \n", fcfsResult);
   printf("SSTF head movements: %ld \n", sstfResult);
   printf("SCAN head movements: %ld \n", scanResult);
   printf("C-SCAN head movements: %ld \n", cscanResult);
   printf("LOOK head movements: %ld \n", lookResult);
   printf("CLOOK head movements: %ld \n", clookResult);
}