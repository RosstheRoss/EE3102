/*
 * File:   spies046_lab6_buffer_v001.c
 * Author: micha
 *
 * Created on April 14, 2020, 1:56 PM
 */

#include "xc.h"

#define BUFFERSIZE 32

volatile int write = 0, numE = 0;
volatile unsigned int buffer[BUFFERSIZE];

    void putVal(int newValue) {       // add a new value to the buffer
        buffer[write++] = newValue;
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
    
    void initBuffer() {              // set all buffer values to zero
        int i;
        for(i = 0; i < BUFFERSIZE; i++) {
            buffer[i] = 0;
        }
    }