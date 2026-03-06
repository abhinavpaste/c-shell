#include<stdio.h>
#include<stdlib.h>

#define MAX_IP 1024

int main(){
    char input[MAX_IP];

    while(1){
        printf("abhi-sh> ");
        fflush(stdout);

        if(fgets(input,MAX_IP,stdin)==NULL){
            printf("\n");
            exit(0);
        }
    }
    return 0;
}