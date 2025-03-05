#include "console.h"
#include <mySimpleComputer.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() { 
    int sc_memoryInit();
    int sc_accumulatorInit();
    int sc_icounterInit();
    int sc_regInit();

    srand(time(NULL));
    int k = rand()%128+1;
    for(int i=0; i<k; i++){
        int value=rand()%(1 << 15);
        sc_memorySet(i, value);
    }
    for(int i=0; i<128; i++){
        void printCell(i);
        if ((i+1)%10==0){
            printf("\n");
        }
        else printf(" ");
    }
    int commandStatus = sc_memorySet(0,-1);
    printf("\nStatus: %d", commandStatus);
    return 0; }

