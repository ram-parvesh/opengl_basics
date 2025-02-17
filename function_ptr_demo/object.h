#include "object.h"
#include <stdio.h>

void printSomething(){
    printf("Hello");
} 

void init_programObject_t(programObject_t* o){
    o->value =0;
    o->pfnMemberFunc = printSomething;
}