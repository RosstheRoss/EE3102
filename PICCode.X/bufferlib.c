#include "xc.h"

#define BUFFERSIZE 32

volatile int write = 0, numE = 0;
volatile unsigned int buffer[BUFFERSIZE];   // Basic circular buffer. Prime for buffer overflows, probably.

    void putVal(int Vo, int ref) {       // add a new value to the buffer
        buffer[write++] = ref - Vo;     // This is backwards because reasons
        write %= BUFFERSIZE;
        
        if (numE < BUFFERSIZE) {
            ++numE;
        }
    }
    
    int getAvg() {                    // average all buffer values
        unsigned long int sum = 0;
        unsigned int avg;
        
        int i;
        for (i = 0; i < BUFFERSIZE; i++) {
            sum += buffer[i];
        }
        avg = sum/numE;
        
        return(avg);
    }
    
    void initBuffer() {              // set all buffer values to zero, likely also prime for a buffer overflow
        int i;
        for(i = 0; i < BUFFERSIZE; i++) {
            buffer[i] = 0;
        }
    }