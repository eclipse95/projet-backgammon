//
// Created by Twilibri on 07/04/2015.
//

#include "strategie\librairy.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int size = 15;
    int* array = calloc(size,sizeof(int));

    int ite;
    for(ite = 0 ; ite < size ; ite++){
        array[ite] = ite;
    }

    testCombi(array, size);


    return 0;
}
