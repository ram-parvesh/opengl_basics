#include "object.h"

int main(){
    programObject_t example;
    init_programObject(&example);

    example.pfnMemberFunc();
    
    return 0;

}