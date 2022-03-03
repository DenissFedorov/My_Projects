#include <stdio.h>

void main(){
        char * buf;
        char * k;
        if((k = getwd(buf)) == NULL){
                printf("Не определен путь к текущей директории\n");
                return;
        }
        printf("%s\n", buf);
}

