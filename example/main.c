#include <stdio.h>
#include <unistd.h>
#include "network.h"

int NetworkOper(char* oper_type, void* in, void* out, char* res, int res_size){
    return 0;
}

int main(int argc, char** argv) {
    NetworkInit(NetworkOper);

    while (1) {
        sleep(1);
    }
    
}