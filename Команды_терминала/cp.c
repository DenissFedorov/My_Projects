#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#define N 10

void main(int argc, char * argv[]){
	int num_bytes;
	char buf[10];
	int orig_file, copy_file;
	struct stat info_f;
	struct stat info_g;
	stat(argv[1], &info_f);
	stat(argv[2], &info_g);
	if((info_f.st_dev == info_g.st_dev)&&(info_f.st_ino == info_g.st_ino)){
		printf("cp: Файлы совпадают\n");
		return;
	}	
	if((argv[1][0] == '/') && (argv[2][0] != '/')){
		printf("Попытка копировать директорию в обычный файл\n");
		return;
	}
	else if(argv[1][0] == '/'){
		DIR * dir_info;
		struct dirent * cur_file;
		dir_info = opendir(argv[1]);
		while((cur_file = readdir(dir_info)) != NULL){
			if((*cur_file).d_name[0] == '.')
				continue;
			chdir(argv[1]);
			stat((*cur_file).d_name, &info_f);
			if((orig_file = open((*cur_file).d_name, O_RDONLY)) == -1){
				printf("Не удалось открыть файл %s\n", (*cur_file).d_name);
				return;
			}
			chdir(argv[2]);
			if((copy_file = open((*cur_file).d_name, O_WRONLY|O_CREAT|O_TRUNC, info_f.st_mode)) == -1){
				printf("Не удалось открыть файл %s\n", (*cur_file).d_name);
				return;
			}
			while((num_bytes = read(orig_file, buf, N)) > 0)
				write(copy_file, buf, num_bytes);
		}
		return;
	}			
        if((orig_file = open(argv[1], O_RDONLY)) == -1){
                printf("Не удалось открыть файл %s\n", argv[1]);
                return;
        }
        stat(argv[1],&info_f);	
	if(argv[2][0] == '/'){
		int l;
	        if((l = chdir(argv[2])) == -1){
			printf("Не удалось открыть директорию %s\n", argv[2]);
			return;
		}	
		if((copy_file = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, info_f.st_mode)) == -1){
			printf("Не удалось открыть файл %s\n", argv[1]);
			return;
		}
		while((num_bytes = read(orig_file, buf, N)) > 0)
			write(copy_file, buf, num_bytes);
		return;
	}
	if((copy_file = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, info_f.st_mode)) == -1){
		printf("Не удалось открыть файл %s\n", argv[2]);
		return;
	}
	while((num_bytes = read(orig_file, buf, N)) > 0)
		write(copy_file, buf, num_bytes);
}	
