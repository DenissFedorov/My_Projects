#include <stdio.h>
#include <string.h>
 
int no_change_str = 0;
int on_escape = 0;
int it_flag = 1;
void main(int argc, char * argv[]){
	for(int i = 1; i < argc; i++){
		if(it_flag && strcmp(argv[i], "-n") == 0){
			no_change_str = 1;
			continue;
		}	
		if(it_flag && strcmp(argv[i], "-e") == 0){
			on_escape = 1;
                        continue;
		}
		if(it_flag && strcmp(argv[i], "-E") == 0){
			on_escape = 0;
			continue;
		}	
		it_flag = 0;
	        if(on_escape == 0)
		        printf("%s ",argv[i]);
	        else{
			for(int j = 0; j < strlen(argv[i]); j++){
				if((j!= strlen(argv[i])-1) && (argv[i][j] =='\\' )){
					j++;
					switch(argv[i][j]){
						case 'n': printf("\n");break;
						case 'b': printf("\b");break;
						case 'c': no_change_str=1;break;
						case 't': printf("\t");break;
						case 'v': printf("\v");break;
						case 'r': printf("\r");break;
						case '\\': printf("\\");break;
						case 'e': printf("\e");break;
					        case 'f': printf("\f");break;
					        case 'a': printf("\a");break;
					        case '\'': printf("\'");break;
					        case '\"': printf("\"");break;
					        		   
					}				

				}
			        else{	        
				       printf("%c",argv[i][j]);
				}       
				
			}
		}
	        if(i != argc-1)	
	        	printf(" ");
	}
        if(no_change_str == 0)
	        printf("\n");	
} 
