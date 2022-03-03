
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <netdb.h>

//int client_sock;
//char c;

void sig_c(int sig)
{
    printf("GOODBYE!\n");
    exit(0);
}

int main()
{
    int client_sock;
    char c;
    signal(SIGINT, sig_c);
    int s_z = 0;
    int serv_port;
    //serv_port = (char *)malloc(sizeof(char *));
    int my_pid;
    struct hostent * hst;
    //hst = malloc(sizeof(struct hostent));
    char * sss;
    sss = (char *)malloc(sizeof(char *));
    printf("Введите адрес сервера: ");
    scanf("%s",sss);
   // struct addrinfo ** res;
   // getaddrinfo(sss, serv_port, NULL, res);
    hst = gethostbyname(sss);
    free(sss);
    //struct addrinfo ** res;
    //res = malloc(sizeof(struct addrinfo **));
    struct sockaddr_in  adr_server;
    printf("Введите порт: ");
    scanf("%d", &serv_port);
    adr_server.sin_family = AF_INET;
    adr_server.sin_port = htons(serv_port);
    adr_server.sin_addr = *(struct in_addr *) hst->h_addr_list[0];
    //getaddrinfo(sss, serv_port, NULL, res);
    if((client_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "### Сокет не был создан\n");
        exit(1);
    }
    if(connect(client_sock, (struct sockaddr *) &adr_server, sizeof(adr_server)) == -1)
    {
        fprintf(stderr, "### Соединения с сервером не произошло\n");
        exit(1);
    }
    int num_read;
    char * s;
    int i, flag_h, flag_end, yes_eof=0;
    scanf("%c", &c);
    my_pid = getpid();
    write(client_sock, &my_pid, sizeof(int));
    while(1)
    {
        if(s_z == 0)
        {
            s = NULL;
            i = 0;
            flag_h = 0;
            flag_end = 0;
            if(scanf("%c",&c) == EOF){
		    printf("GOODBYE!\n");
		    exit(0);
	    }
            while(c != '\n')
            {
	        i++;
                s = realloc(s, i*sizeof(char));
                s[i-1] = c;
                if(scanf("%c", &c) == EOF){
		       	yes_eof=1;
			printf("\n");
			break;
		}	
            }
            i++;
            s = realloc(s, i*sizeof(char));
            s[i-1] = '\n';
            write(client_sock, s, i*sizeof(char));
            s[i-1] = '\0';
            if(strcmp(s,"\\-") == 0) flag_end = 1;
            if(strcmp(s,"\\help") == 0) flag_h = 1;
            free(s);
            num_read = read(client_sock, &c, 1);
            if(num_read == 0)
            {
                printf("Сервер завершил работу\n");
                exit(0);
            }
            if(flag_end)
            {
                printf("GOODBYE!\n");
                exit(0);
            }
            if(flag_h)
            {
                while(c != 'F')
                {
                    printf("%c",c);
                    read(client_sock, &c, 1);
                }
            }
            else
            {
                while(c != '\n')
                {
                    if(c == 'z')
                    {
                        printf("### Работа сервера приостановлена");
                        s_z = 1;
                        break;
                    }
                    printf("%c",c);
                    read(client_sock, &c, 1);
                }
            }
            printf("\n");
        }
        else
        {
            if(read(client_sock, &c, 1) == 1)
            {
                if(c == 'c')
                {
                    printf("### Работа сервера возобновлена\n");
                    s_z = 0;
                }
            }
        }
	if(yes_eof){
		printf("GOODBYE!\n");
		exit(0);
	}	
    }
    return 0;
}

