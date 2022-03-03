#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

char * ls_r(char * name){
	DIR * s = opendir(name);
	struct dirent * ff;
	struct stat aa;

	while((ff = readdir(s)) != NULL){
		if((strcmp((*ff).d_name, ".") == 0) || (strcmp((*ff).d_name, "..") == 0)) continue;
	        stat((*ff).d_name, &aa);
		if((aa.st_mode & S_IFMT) == S_IFDIR){
			printf("\n/%s:\n", (*ff).d_name);
		       	ls_r((*ff).d_name);
			printf("\n");
		}
	        else{
                	printf("%s\n", (*ff).d_name);
		}	
	}	
}	

void main(int argc, char * argv[]){
	int masks[3] = {0700, 0070, 0007}; // Маски для получения прав доступа
	struct dirent * cur_file; // Переменная, хранящая данные о тек. файле
	char buf[PATH_MAX]; // Переменная для записи текущего каталога
	DIR * structure; // Переменная содержит инфо о каталоге
	char * dir_name = getcwd(buf, PATH_MAX); // Путь до текущего каталога
	int yes_r = 0; // равно 1, если есть флаг -R
	int yes_l = 0; // равно 1, если есть флаг -l
	int yes_g = 0; // равно 1, если есть флаг -g
	struct stat statbuf; // Указатель на структуру, которая содержит
	                       // инфо о текущем файле
        struct passwd * usr_data; // В этой структуре имя владельца
	struct group * group_data; // В этой структуре имя группы

	if(dir_name == NULL){
		printf("Ошибка определения пути файла buf\n");
		return;
	}	
	for(int i = 1; i < argc; i++){ // Считывание флагов
		if(strcmp(argv[i], "-R") == 0)
			yes_r = 1;
		if(strcmp(argv[i], "-l") == 0)
		        yes_l = 1;
		if(strcmp(argv[i], "-g") == 0)
		        yes_g = 1;
        }
        if(yes_r == 1){
                ls_r(buf);
        }

        if(yes_r == 0){
	structure = opendir(buf); // Открыт каталог, из которого вызвана ls	
	while((cur_file = readdir(structure)) != NULL){
		if((*cur_file).d_name[0] == '.') continue;
		if(yes_l == 1){
			stat((*cur_file).d_name, &statbuf);
			switch(statbuf.st_mode & S_IFMT){ // Опред. типа файла
				case S_IFDIR:printf("d");break;
				case S_IFCHR:printf("c");break;
				case S_IFBLK:printf("b");break;
			        case S_IFREG:printf("-");break;
			} 
			for(int i = 0; i < 3; i++){ // Печать прав доступа
				int k = (statbuf.st_mode & masks[i]) >> ((2 - i)*3);
                                k & 4 ? printf("r") : printf("-");
				k & 2 ? printf("w") : printf("-");
				k & 1 ? printf("x") : printf("-");
			}
		        printf(" ");
			usr_data = getpwuid(statbuf.st_uid);
		        printf("%s ", (*usr_data).pw_name); // Имя владельца	
                        printf("%ld ", statbuf.st_size); // Размер файла		
                }
                if(yes_g == 1){
			stat((*cur_file).d_name, &statbuf);
			group_data = getgrgid(statbuf.st_gid);
			printf("%s ", (*group_data).gr_name); // Имя группы
		}	
		        	
		printf("%s\n", (*cur_file).d_name);
	}
	}
}	
        	

