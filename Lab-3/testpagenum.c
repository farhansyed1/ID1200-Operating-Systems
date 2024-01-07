#include <stdio.h>
int main(int argc, char const *argv[])
{

    int addresses[8] = {1, 256, 32768, 32769, 128, 65534, 3315};

    for(int i = 0; i < 7; i++){
        int addressAsNumber = addresses[i]; // 32 bit integer
        int pageNumber = (addressAsNumber >> 8) & 0xFF; // Get 8-bit page number
        int pageOffset = addressAsNumber & 0xFF; // Get 8-bit page offset
        printf("Address %d, P. num: %d,  P. offset %d \n", addressAsNumber, pageNumber, pageOffset);
    }
    
    return 0;
}
