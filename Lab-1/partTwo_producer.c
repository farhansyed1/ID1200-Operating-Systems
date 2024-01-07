#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

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

int main()
{
    mq_unlink("/my_queue");
    mqd_t mq;
    struct mq_attr attr;
    char buffer[maxSize];

    // Set the message queue attributes
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = maxSize;
    attr.mq_curmsgs = 0;

    // Open the message queue for writing
    mq = mq_open("/my_queue", O_WRONLY | O_CREAT, 0666, &attr);
    if (mq == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Open the file for reading
    FILE *file = fopen("text.txt", "r");
    if (!file)
    {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, maxSize, file) != NULL)
    {
        // Send the contents of the file through the message queue
        if (mq_send(mq, buffer, strlen(buffer) + 1, 0) == -1)
        {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }
    }

    // Send an empty message to signal the end of file
    if (mq_send(mq, "", 1, 0) == -1)
    {
        perror("mq_send");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    mq_close(mq);

    return 0;
}