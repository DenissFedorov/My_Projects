#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h> // Для структуры sockaddr_in домена AF_INET и htons()
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>
#define size 16
#define ADDR "address"

char ** list_str; // Масссив с указателями на строки
char c; // Переменная для текущего символа
char * buf=NULL; // Переменная с адресом буфера, куда запишем текущее слово
int curlist; // Индекс первого элемента массива list_str, в который
// возможна запись
int sizelist; // Текущий размер списка слов из строки
int sizebuf; // Размер буфера, куда записывается текущее слово
int curbuf; // Индекс текущего элемента в буфере, куда можно записать
int inc_num = 1, num_read;
int * all_clients = NULL;
//all_clients = malloc(sizeof(int));
//*all_clients = 0;
int * mas_pid_clients = NULL;
int * mas_sock_clients = NULL;


const char * err_vvod = "Некорректный ввод\n";

const char * tutor = "============= Помощь\nДоступны следующие команды:\n\\+ <число> - установится число, на которое сервер будет увеличивать числа (написав \\+ будет установлено число 1)\n<число> - это число вернется, увеличенное на заданное\n\\? - получить число, на которое увеличивает сервер\n\\- - сообщить серверу о завершении работы\n=============F";

// Набор функций для обработки сообщений от клиента

void null_list()
{
//    if(list_str != NULL) free(list_str);
    sizelist = 0;
    curlist = 0;
   // if(list_str == NULL) // Список и так пуст
     //   return;
    list_str = NULL;
}

void nullbuf()
{
//    if(buf != NULL) free(buf);
    buf = NULL;
    //buf = NULL; // Буфер пуст
    sizebuf = 0; // Обнуление размера буфера
    curbuf = 0;  // Индекс, куда можно записать в буфере = 0
}

void addsym()
{
    if(curbuf > sizebuf - 1) // Закончилось место в буфере
    {	    
	char * tmp_buf;
        tmp_buf = realloc(buf, sizebuf += 16); // Расширение
	if(tmp_buf == NULL)
	{
		free(buf);
		kill(getpid(), SIGINT);
	}
	buf = tmp_buf;
    }	
    buf[curbuf++] = c; // Добавление прочитанного символа в буфер,
    // увеличение индекса curbuf
}

void addword()
{
    char * tmp_buf;
    if(curbuf > sizebuf -1) // В буфере не хватает места, чтобы
        // записать '\0'
    { 
	sizebuf += 1;
        tmp_buf = (char *)realloc(buf, sizebuf);
	if(tmp_buf == NULL){
		free(buf);
		kill(getpid(), SIGINT);
	}
	buf = tmp_buf;
	//free(tmp_buf);
    }	
    buf[curbuf] = '\0';
    curbuf++;
    sizebuf = curbuf;
    tmp_buf = realloc(buf,sizebuf); // Обрезаем пямять под размер
    // слова
    if(tmp_buf == NULL){
	    free(buf);
	    kill(getpid(), SIGINT);
    }
    buf = tmp_buf;
    char ** tmp_list;    
    if(curlist > sizelist - 1) // В массиве под слова не хватает места
    {
	sizelist+=16;	    
        tmp_list = realloc(list_str, sizelist * sizeof(char *)); // Увеличение массива
    	if(tmp_list == NULL)
	{
		free(list_str);
		kill(getpid(), SIGINT);
	}
	list_str = tmp_list;
    }		
    list_str[curlist] = buf;
    curlist++;
    //free(tmp_buf);
}

void printlist()  // Печать массива слов
{
    if(list_str == NULL)
    {
        return;
    }
    for(int i = 0; i < sizelist - 1; i++)
    {
        printf("%s\n",list_str[i]);
    }
}

void termlist()
{
    char ** tmp_list;
    if(list_str == NULL)
    {
        return;
    }
    if(curlist > sizelist - 1)
    {	    
//	sizelist++;
        tmp_list = realloc(list_str,(sizelist+1) * sizeof(char *));
	if(tmp_list == NULL)
	{
		free(list_str);
		kill(getpid(), SIGINT);
	}
	list_str = tmp_list;
    }	
    list_str[curlist] = NULL;
    //curlist++;
    sizelist = curlist + 1;
    tmp_list = realloc(list_str, sizelist * sizeof(char *));
    if(tmp_list == NULL)
    {
	    free(buf);
	    kill(getpid(), SIGINT);
    }
    list_str = tmp_list;
}

int digit_or_no(char * s)
{
    if(s == NULL) return 0;
    int i = 0;
    while(s[i] != '\0')
    {
        if((i == 0) && (s[0] == '-'));
        else if((s[i] < '0') || (s[i] > '9'))
        {
            return 0;
        }
        i++;
    }
    return 1;
}

void sig_c(int sig)  // Обработка сигнала
{
    if(mas_pid_clients != NULL)
    {
        for(int i = 0; i < *all_clients; i++)
        {
            shutdown(mas_sock_clients[i], 2);
        }
    }
    free(buf);
    free(list_str);
    free(all_clients);
    free(mas_pid_clients);
    free(mas_sock_clients);
    exit(0);
}

void sig_cont(int sig)
{
    if(mas_pid_clients != NULL)
    {
        for(int i = 0; i < *all_clients; i++)
        {
            write(mas_sock_clients[i], "c", sizeof(char));
        }
    }
}

void sig_z(int sig)
{
    if(mas_pid_clients != NULL)
    {
        for(int i = 0; i < *all_clients; i++)
        {
            write(mas_sock_clients[i], "z", sizeof(char));
        }
    }
    signal(SIGTSTP,SIG_DFL);
    kill(getpid(),SIGTSTP);
}

int main()
{
    buf = malloc(sizeof(char));
    list_str = malloc(sizeof(char *));
    printf("Номер порта: 1111\n");
    int sock, cur_sock;
    struct sockaddr_in adr, cur_client;

    all_clients = (int *)malloc(sizeof(int));
    *all_clients = 0;

    signal(SIGINT, sig_c);
    signal(SIGCONT, sig_cont);
    signal(SIGTSTP, sig_z);

    adr.sin_family = AF_INET;
    adr.sin_port = htons(1111); // Номер порта, получаемый через htons()
    adr.sin_addr.s_addr = INADDR_ANY; // IP адрес

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        fprintf(stderr, "### Сокет не был создан\n");
        exit(1);
    }
    if(bind(sock, (struct sockaddr *) &adr, sizeof(adr)) == -1)
    {
        fprintf(stderr, "### Ошибка привязки сокета к адресу\n");
        fprintf(stderr, "### Повторите попытку позже\n");
        exit(1);
    }
    if(listen(sock, 5) == -1)  // Максимальное очередь запросов
    {
        // на соединение = 5
        fprintf(stderr, "### Ошибка установления сокета\n");
        exit(1);
    }
    // Завершена установка соединения, можно начинать взаимодествие
    for(;;)
    {
	int * tmp_mas=NULL;
	char * tmp_ch=NULL;
        int pid_cl;
        int sss = sizeof(struct sockaddr);
        cur_sock = accept(sock, (struct sockaddr *) &cur_client, (unsigned int *)&sss);
        // Обработка запроса
        // на соединение
        if(cur_sock == -1)
        {
            fprintf(stderr, "### Соединение не установлено\n");
            exit(1);
        }
        *all_clients += 1;
        tmp_mas = (int *)realloc(mas_pid_clients, (*all_clients)*sizeof(int));
        if(tmp_mas == NULL){
		free(mas_pid_clients);
		kill(getpid(), SIGINT);
	}
	mas_pid_clients = tmp_mas;	
	tmp_mas = realloc(mas_sock_clients,(*all_clients)*sizeof(int));
        if(tmp_mas == NULL){
		free(mas_sock_clients);
		kill(getpid(), SIGINT);
	}
	mas_sock_clients = tmp_mas;
	read(cur_sock, &pid_cl, sizeof(int));
        mas_pid_clients[(*all_clients)-1] = pid_cl;
        mas_sock_clients[(*all_clients)-1] = cur_sock;
        printf("*** Клиент %d подсоединился\n", cur_client.sin_port);
        if(fork() == 0)  // Обработка клиента
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGCONT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            for(;;)
            {
                num_read = read(cur_sock, &c, 1);
		//free(buf);
		//free(list_str);
                nullbuf();
                null_list();
                if(num_read == 0)
                {
                    printf("*** Клиент %d отсоединился\n", cur_client.sin_port);
		    shutdown(cur_sock, 2);
		    free(buf);
		    free(all_clients);
		    free(mas_pid_clients);
		    free(mas_sock_clients);
                    break;
                }

                while(c != '\n')
                {	
                    if(num_read == 0)
                    {
                        printf("*** Клиент %d отсоединился\n", cur_client.sin_port);
                        shutdown(cur_sock, 2);
                        break;
                    }
                    if(num_read == -1)
                    {
                        printf("*** Ошибка при чтении запроса клиента или клиент %d завершил свою работу с сервером\n", cur_client.sin_port);
                        shutdown(cur_sock, 2);
                        break;
                    }
                    if(num_read == 1)
                    {
                        if(c == ' ')
                        {
                            if(buf != NULL)
                            {   
                                addword();
                                termlist();
                                nullbuf();
                            }
                        }
                        else if(c == '\t');
                        else addsym();
                    }
                    num_read = read(cur_sock, &c, 1);
                }
                if(buf != NULL)
                {
                    addword();
                    termlist();
                    nullbuf();
                }
		//printf("** %d\n", list_str[0][2] == '\0' );
                if(list_str != NULL)  // Просмотр написанного
                {
                    if(strcmp(list_str[0], "\\help") == 0)
                    {
                        write(cur_sock, tutor, strlen(tutor)*sizeof(char));
                    }
                    else if((strcmp(list_str[0], "\\+") == 0) && sizelist ==3)
                    {
                        inc_num = atoi(list_str[1]);
                        write(cur_sock, "Ok\n", strlen("Ok\n")*sizeof(char));
                    }
                    else if((strcmp(list_str[0], "\\+") == 0) && sizelist == 2)
                    {
                        inc_num = 1;
                        write(cur_sock, "Ok\n", strlen("Ok\n")*sizeof(char));
                    }
                    else if(strcmp(list_str[0], "\\?") == 0)  // Перевод число в символьное представление(в т.ч. отрицательные)
                    {
                        char * chislo = NULL;
                        int cp=0, sb=0;
                        int t = inc_num;
                        if(t < 0)
                        {
                            t = -inc_num;
                        }
                        for(; t > 0; t = t/10)
                        {
                            if(cp > sb -1)
                            {
                                tmp_ch = realloc(chislo,sb+=16);
				if(tmp_ch == NULL){
					free(chislo);
					kill(getpid(),SIGINT);
				}	
				chislo = tmp_ch;	
                            }
                            chislo[cp++]=t%10 + '0';
                        }
                        if(inc_num < 0)
                        {
                            if(cp > sb - 1)
                            {
                                tmp_ch = realloc(chislo,sb+=16);
				if(tmp_ch == NULL){
					free(chislo);
					kill(getpid(), SIGINT);
				}	
				chislo = tmp_ch;
                            }
                            chislo[cp++]='-';
                        }
                        if(inc_num == 0)
                        {
                            tmp_ch=realloc(chislo,sb+=16);
			    if(tmp_ch == NULL){
				    free(chislo);
				    kill(getpid(), SIGINT);
		            }
			    chislo = tmp_ch;	    
                            chislo[cp++]='0';
                        }
			tmp_ch=realloc(chislo,cp+1);
			if(tmp_ch == NULL){
				free(chislo);
				kill(getpid(), SIGINT);
			}
			chislo = tmp_ch;
                        chislo[cp] = '\n';
                        char k;
                        for(t=0; t<cp/2; t++)
                        {
                            k = chislo[t];
                            chislo[t]=chislo[cp-1-t];
                            chislo[cp-1-t]=k;
                        }
                        write(cur_sock, chislo, cp+1);
                        free(chislo);
			//free(tmp_ch);
                    }
                    else if(strcmp(list_str[0], "\\-") == 0)
                    {
                        printf("*** Завершение работы с клиентом %d\n", cur_client.sin_port);
                        write(cur_sock, "Соединение с сервером прервано\n", 62*sizeof(char));
			printf("CCC\n");
                        shutdown(cur_sock, 2);
                        break;
                    }
                    else if(digit_or_no(list_str[0]) == 1)
                    {
                        char * chislo = NULL;

                        int cp=0, sb=0;
                        int t=atoi(list_str[0])+inc_num;
                        if(t < 0)
                        {
                            t = -atoi(list_str[0])-inc_num;
                        }
                        for(; t>0; t=t/10)
                        {
                            if(cp > sb -1)
                            {
                                tmp_ch = realloc(chislo,sb+=16);
				if(tmp_ch == NULL){
					free(chislo);
					kill(getpid(), SIGINT);
				}
				chislo = tmp_ch;
                            }
                            chislo[cp++]=t%10 + '0';
                        }
                        if((atoi(list_str[0])+inc_num) < 0)
                        {
                            if(cp > sb - 1)
                            {
                                tmp_ch = realloc(chislo,sb+=16);
				if(tmp_ch == NULL){
					free(chislo);
					kill(getpid(), SIGINT);
				}
				chislo = tmp_ch;	
                            }
                            chislo[cp++]='-';
                        }
                        if((atoi(list_str[0]) + inc_num) == 0)
                        {	
                            tmp_ch=realloc(chislo,sb+=16);
			    if(tmp_ch == NULL){
				    free(chislo);
				    kill(getpid(),SIGINT);
			    }	    
			    chislo = tmp_ch;
                            chislo[cp++]='0';
                        }
			tmp_ch=realloc(chislo,cp+1);
			if(tmp_ch == NULL){
				free(chislo);
				kill(getpid(), SIGINT);
			}
			chislo = tmp_ch;
                        chislo[cp] = '\n';
                        char k;
                        for(int t=0; t<cp/2; t++)
                        {
                            k = chislo[t];
                            chislo[t]=chislo[cp-1-t];
                            chislo[cp-1-t]=k;
                        }
			
                        write(cur_sock, chislo, cp+1);
                        free(chislo);
			//free(tmp_ch);
                    }
                    else
                    {
                        write(cur_sock, err_vvod,strlen(err_vvod)*sizeof(char));
                    }
                }
                else
                {
                    write(cur_sock, err_vvod, strlen(err_vvod)*sizeof(char));
                }
            }
	    free(buf);
	    nullbuf();
	    null_list();
        }
    }
    free(buf);
    free(list_str);
    free(all_clients);
    free(mas_pid_clients);
    free(mas_sock_clients);
    return 0;
}
