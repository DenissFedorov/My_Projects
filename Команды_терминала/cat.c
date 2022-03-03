#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void main(int argc, char * argv[]){
	int cur_file;
	char c;
	int k;
	int num_str = 0;
	if(argc == 1){
		while(read(0, &c, 1)){
			write(1, &c, 1);
		}
		return;
	}
	if(strcmp(argv[1], "-n") == 0)
                num_str = 1;
	if(num_str && argc == 2){
		k = 0;
                while(read(0, &c, 1)){
                        if(k == 0){
                                printf("%6.d  ", num_str++);
                                k = 1;
                        }
                        printf("%c", c);
                        if(c == '\n') k = 0;
                }
		return;
	}
	for(int i = num_str + 1; i < argc; i++){
		if((cur_file = open(argv[i], O_RDONLY)) == -1){
			printf("Не удалось открыть файл %s\n", argv[i]);
			printf("** Переход к следующему файлу **\n");
			continue;
		}
		k = 0;
		while(read(cur_file, &c, 1)){
			if(k == 0 && num_str){
				printf("%6.d  ", num_str++);
				k = 1;
			}	
			printf("%c", c);
			if(c == '\n') k = 0;
		}
	}
}	




