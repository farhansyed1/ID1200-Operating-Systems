#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>
#include <stdbool.h>
#include <ctype.h>

/* Authors:
   Abhinav Sasikumar
   Farhan Syed

   2023-09-18

   Compilation: gcc partTwo_producer.c -o producer.out -lrt
                gcc partTwo_consumer.c -o consumer.out -lrt
   Execution:   ./producer.out
                ./consumer.out

*/

#define maxSize 1024

/*
Function: countWordsInMessage
-----------------------------
Helper  that counts the number of words in one message recieved from the message queue.

message: The message to count the words in.

returns: the number of words in the message.
 */
int countWordsInMessage(char *message)
{
    int wordCount = 0;
    char *wordstart = message;
    bool inWord = false;

    while (*wordstart != '\0')
    {
        if (isalpha(*wordstart) > 0)
        {
            if (!inWord)
            {
                inWord = true;
                wordCount++;
            }
        }
        else
        {
            inWord = false;
        }
        wordstart++;
    }
    return wordCount;
}

int main()
{

    char fileContents[maxSize];
    int wordCount = 0;

    mqd_t mq = mq_open("/my_queue", O_RDONLY);
    if (mq == -1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    while (1)
    {

        ssize_t receivedMessage = mq_receive(mq, fileContents, maxSize, NULL); // Receive message from queue
        if (receivedMessage == -1)
        {
            perror("mq_receive");
            exit(EXIT_FAILURE);
        }

        if (receivedMessage == 1 && fileContents[0] == '\0') // Break when reaching end
        {
            break;
        }
        wordCount += countWordsInMessage(fileContents);
    }

    printf("Total words in the file: %d\n", wordCount);
    mq_close(mq);

    mq_unlink("/my_queue");

    return 0;
}