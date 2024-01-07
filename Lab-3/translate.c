/* Authors:
   Abhinav Sasikumar
   Farhan Syed

   2023-11-07

   Compilation: gcc translate.c
   Execution:   ./a.out addresses.txt

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define page_size 256
#define memory_frame_size 256
#define page_number_bits 8
#define page_offset_bits 8
#define TLB_size 16
int indexCounter = 0;

double pageFaultCounter = 0;
double tlbHitCounter = 0;
double numberOfAddressesCounter = 0;
/*
  A TLB and Page table implementation that calculates the physical address and writes it to stdout.

*/
void calculatePhysicalAddress(int logicalAddress, unsigned int pageNumber, unsigned int addressOffset, int tlb[16][2], char *pageTable, char physMem[memory_frame_size][memory_frame_size])
{
    unsigned int physicalAddress;
    unsigned char frameNumber = 0;
    int tlb_hit = 0;

    // Go through the TLB and check whether the page number is in the TLB. If hit, we can skip page table step.
    for (int i = 0; i < TLB_size; i++)
    {
        if (tlb[i][0] == pageNumber)
        {
            tlb_hit = 1;
            frameNumber = tlb[i][1];
            physicalAddress = frameNumber * memory_frame_size + addressOffset;
            tlbHitCounter++;
            break;
        }
    }
    // Miss in TLB. Must go through the page table
    if (!tlb_hit)
    {
        // Hit in page table
        if (pageTable[pageNumber] != -1)
        {
            // Get the frame that is mapped to the pageNumber
            frameNumber = pageTable[pageNumber];

            // Calculate the physical address
            physicalAddress = frameNumber * memory_frame_size + addressOffset;
        }
        // Miss in page table
        else
        {
            pageFaultCounter++;

            FILE *backing_store = fopen("BACKING_STORE.bin", "r");          // Open the backing store file
            fseek(backing_store, pageNumber * memory_frame_size, SEEK_SET); // Read from where there was page fault

            unsigned char buffer[256];
            fread(buffer, sizeof(unsigned char), page_size, backing_store); // Read one page (256 bytes)

            // Read the missed frame into physical memory
            frameNumber = 0;

            // 1. Find a free page frame in physical memory
            for (unsigned int i = 0; i < 256; i++)
            {
                if (physMem[i][0] == -1)
                {
                    frameNumber = i;
                    break;
                }
            }
            // 2. Write the data read from backing store to page frame
            for (unsigned int i = 0; i < page_size; i++)
            {
                physMem[frameNumber][i] = buffer[i];
            }
            // 3. Add frame number to page table
            pageTable[pageNumber] = frameNumber;
            fclose(backing_store);

            physicalAddress = frameNumber * memory_frame_size + addressOffset;
        }

        // Update the TLB using FIFO
        tlb[indexCounter][0] = pageNumber;
        tlb[indexCounter][1] = frameNumber;

        indexCounter = (indexCounter + 1) % TLB_size;
    }

    // Obtain value from physical address and print output
    char value = physMem[frameNumber][addressOffset];
    printf("Virtual address: %d Physical address: %u Value: %d \n", logicalAddress, physicalAddress, value);
}

int main(int argc, char const *argv[])
{
    /*
    Program specifics:
    16 entries in the TLB
    256 entries in the page table
    Page size of 256 bytes
    256 frames in the physical memory
    Frame size of 256 bytes
    Physical memory of 65,536 bytes (256 frames × 256-byte frame size
    */

    if (argc < 1)
    {
        printf("No argument provided");
        return EXIT_FAILURE;
    }

    // TLB consisting of page number and corresponding frame in physical memory
    int TLB[16][2];
    int pageSize = 256;
    int frameSize = 256;

    // Page table containing the frame of the given page
    char pageTable[pageSize];

    // Physical memory 256 bytes * 256 frames
    char physicalMemory[frameSize][frameSize];

    // The file containing logical addresses
    FILE *file = fopen(argv[1], "r");

    // The logical address
    int addressAsNumber = 0;

    // Initializing TLB entries to -1
    for (int i = 0; i < TLB_size; i++)
    {
        TLB[i][0] = -1; // Initialize page number to -1
        TLB[i][1] = -1; // Initialize frame number to -1
    }
    // Initializing pageTable entries to -1
    for (int i = 0; i < pageSize; i++)
    {
        pageTable[i] = -1;
    }
    // Initializing physical memory to -1
    for (int i = 0; i < memory_frame_size; i++)
    {
        for (int j = 0; j < memory_frame_size; j++)
        {
            physicalMemory[i][j] = -1;
        }
    }

    // Read one address at a time from the file
    while (fscanf(file, "%d", &addressAsNumber) == 1)
    {

        // Extracting page number and offset
        unsigned int pageNumber = (addressAsNumber >> 8) & 0xFF; // Get bits 8-15
        unsigned int pageOffset = addressAsNumber & 0xFF;        // Get bits 0-7

        // Calculate physical address and print the results:
        calculatePhysicalAddress(addressAsNumber, pageNumber, pageOffset, TLB, pageTable, physicalMemory);
        numberOfAddressesCounter++;
    }
    // Print statistics
    double pageFaultRate = (pageFaultCounter / numberOfAddressesCounter) * 100;
    double tlbHitRate = (tlbHitCounter / numberOfAddressesCounter) * 100;

    printf("Page fault rate: %.2f%% \n", pageFaultRate);
    printf("TLB hit rate: %.2f%% \n", tlbHitRate);

    return EXIT_SUCCESS;
}