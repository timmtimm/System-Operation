#include <stdio.h>
#include <netinet/in.h> 
#include <unistd.h> 
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <dirent.h>
#include <stdlib.h>

#define DATA_BUFFER 50
#define MAX_CONNECTIONS 10 
#define SUCCESS_MESSAGE "Your message delivered successfully"
#define LOGIN_MESSAGE "Id and Password is sent\n"
#define SIZE 100

int create_tcp_server_socket() {
    struct sockaddr_in saddr;
    int fd, ret_val;

    /* Step1: create a TCP socket */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (fd == -1){
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        return -1;
    }
    printf("Created a socket with fd: %d\n", fd);

    /* Initialize the socket address structure */
    saddr.sin_family = AF_INET;         
    saddr.sin_port = htons(7000);     
    saddr.sin_addr.s_addr = INADDR_ANY; 

    /* Step2: bind the socket to port 7000 on the local host */
    ret_val = bind(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val != 0) {
        fprintf(stderr, "bind failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }

    /* Step3: listen for incoming connections */
    ret_val = listen(fd, 5);
    if (ret_val != 0) {
        fprintf(stderr, "listen failed [%s]\n", strerror(errno));
        close(fd);
        return -1;
    }
    return fd;
}

int checkIdPassword(char id[], char password[], char cmd[])
{
	char line[512];
	const char delim[2] = ":";
	char *tempId, *tempPass;

	FILE *fp = fopen("akun.txt", "r");
	while(fgets(line, 512, fp))
	{
		char *newline = strchr( line, '\n' );
		if ( newline )
			*newline = 0;
		
		tempId = strtok(line, delim);
		tempPass = strtok(NULL, delim);

		if(!strcmp(cmd, "register"))
		{
			if(!strcmp(tempId, id))
				return 1;
		}
		else
		{
			if(!strcmp(tempId, id) && !strcmp(tempPass, password))
				return 1;
		}
	}
	fclose(fp);
	return 0;
}

void registerLogin(int all_connections_i, char cmd[], char id[], char password[], int *userLoggedIn, int all_connections_serving )
{
    int ret_val;
    int status_val;

    if(!strcmp(cmd, "register"))
	{
        ret_val = recv(all_connections_i, id, SIZE, 0);
        ret_val = recv(all_connections_i, password, SIZE, 0);
     
	    if(checkIdPassword(id, password, cmd))
		{
            status_val = send(all_connections_serving, "userfound\n", SIZE, 0);
        }
		else
		{
            *userLoggedIn = 1;
            FILE *app = fopen("akun.txt", "a+");
            fprintf(app, "%s:%s\n", id, password);
            fclose(app);
            status_val = send(all_connections_serving, "regloginsuccess\n", SIZE, 0);
        }
    }
	else if(!strcmp(cmd, "login"))
	{
        ret_val = recv(all_connections_i, id, SIZE, 0);
        ret_val = recv(all_connections_i, password, SIZE, 0);

        if(!checkIdPassword(id, password, cmd))
            status_val = send(all_connections_serving, "wrongpass\n", SIZE, 0);
        else
		{
            *userLoggedIn = 1;
            status_val = send(all_connections_serving, "regloginsuccess\n", SIZE, 0);
        }
    }
}

void makeLog(char cmd[], char fileName[], char id[], char password[])
{
	FILE *fp = fopen("running.log", "a");
	
	if(!strcmp(cmd, "add"))
	{
		printf("Tambah : %s(%s:%s)\n\n", fileName, id, password);
		fprintf(fp, "Tambah : %s(%s:%s)\n", fileName, id, password);
	}
	else if(!strcmp(cmd, "delete"))
	{
		printf("Hapus : %s(%s:%s)\n\n", fileName, id, password);
		fprintf(fp, "Hapus : %s(%s:%s)\n", fileName, id, password);
	}
	fclose(fp);
}

void getFileName(char filePathDir[], char fileName[])
{
    int len = strlen(filePathDir)-1;
    int index = 0;

    while(len)
	{
        filePathDir[len+1] = '\0';
        if(filePathDir[len]=='/')
            break;
		
        fileName[index] = filePathDir[len];
        len--;
        index++;
    }
	fileName[index] = '\0';
}

char *strrev(char *str)
{
    char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

void writeFile(int fd, char fileName[])
{
    int n;
    char fullPath[100] = "/home/timtim/Desktop/SisOp/Praktikum3/soal1/server/FILES/";
    char buffer[SIZE];

    strcat(fullPath, fileName);
    FILE *fp = fopen(fullPath, "w");
    fclose(fp);
 
    while(1)
	{
        n = recv(fd, buffer, SIZE, 0);

        if(n <= 0)
		{
            break;
            return;
        }

        if(!strcmp(buffer, "done"))
            return;
		
        fp = fopen(fullPath, "a");
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
        fclose(fp);
    }
    return;
}

void addBooks(int send_clt, int rcv_clt, char id[], char password[])
{
	char publisher[SIZE], tahun[SIZE], filePath[SIZE],  filePathDir[SIZE], fileName[15];
	int ret_val;
	
	ret_val = recv(send_clt, publisher, SIZE, 0);
	ret_val = recv(send_clt, tahun, SIZE, 0);
	ret_val = recv(send_clt, filePath, SIZE, 0);

	sprintf(filePathDir, "%s", filePath);
	getFileName(filePathDir, fileName);
	strrev(fileName);

	writeFile(send_clt, fileName);


	FILE *temp = fopen("files.tsv", "a+");
	fprintf(temp, "%s\t%s\t%s\n", filePath, publisher, tahun);
	fclose(temp);

	makeLog("add", fileName, id, password);
}

void sendFile(int sockfd, char filePath[])
{
    int n;
    char data[SIZE] = {0};
    FILE *fp = fopen(filePath, "r");

    while(fgets(data, SIZE, fp) != NULL) {
        if (send(sockfd, data, SIZE, 0) == -1) {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }

    fclose(fp);
    int ret_val = send(sockfd, "done", SIZE, 0);
}


int checkPath(char fileName[])
{
    char temp[100];
    FILE *fp = fopen("files.tsv", "r");

    while(fscanf(fp,"%s", temp) == 1)
	{
        if(strstr(temp, fileName)!=0)
                return 1;
    }
    fclose(fp);
    return 0;
}

void downloadBooks(int send_clt, int rcv_clt)
{
	char books[SIZE];
	int line = 1;
	int ret_val, status_val;
	ret_val = recv(send_clt, books, SIZE, 0);

	if(checkPath(books))
	{
		status_val = send(rcv_clt, "Begin to download\n", SIZE, 0);
		char temp[SIZE] = "/home/timtim/Desktop/SisOp/Praktikum3/soal1/server/FILES/";

		strcat(temp, books);
		sendFile(send_clt, temp);
	}
	else
	{
		status_val = send(rcv_clt, "File does not exist\n", SIZE, 0);
	}
}

int deleteTsv(char filename[])
{
    FILE *tsv = fopen("files.tsv", "r+");
    FILE *tmp = fopen("temp.tsv", "w+");
    char temp[256], line[256];

	while(fgets(line, 256, tsv) != 0)
	{
        if(sscanf(line, "%255[^\n]", temp) != 1) 
			break;
        if(strstr(temp, filename) == 0) 
            fprintf(tmp, "%s\n", temp);
    }

    while(fgets(line, 256, tsv) != 0)
	{
        if(sscanf(line, "%255[^\n]", temp) != 1) break;
        fprintf(tsv, "%s\n", temp);
    }

    remove("files.tsv");
    rename("temp.tsv", "files.tsv");

    fclose(tmp);
    fclose(tsv);
    return 0;
}

void deleteBooks(int send_clt, int rcv_clt, char id[], char password[])
{
	char books[SIZE];
	int ret_val1 = recv(send_clt, books, SIZE, 0);
	int  status_val;
	char temp[SIZE] = "/home/timtim/Desktop/SisOp/Praktikum3/soal1/server/FILES/";
	char temp2[120] = "/home/timtim/Desktop/SisOp/Praktikum3/soal1/server/FILES/old-";

	printf("The book that wants to be deleted : %s\n", books);
	if(checkPath(books))
	{
	    status_val = send(rcv_clt, "Begin to delete\n", SIZE, 0);
	    strcat(temp, books);
	    strcat(temp2, books);

	    rename(temp, temp2);
	    deleteTsv(books);
		makeLog("delete", books, id, password);
	}else 
	    status_val = send(rcv_clt, "File does not exist\n", SIZE, 0);
}

void seeBooks(int rcv_clt)
{
	char *publisher, *tahun, *filePath, *nameFile, *ekstensi;
	char  line[512], filePathDir[SIZE], tempName[SIZE];
	const char tab[2] = "\t";
	int status_val;

	FILE *fp = fopen("files.tsv", "r");
	status_val = send(rcv_clt, "not-done", SIZE, 0);

	while(fgets(line, 512, fp))
	{
		char *newline = strchr( line, '\n' );
		if(newline)
			*newline = 0;

		filePath = strtok(line, tab);
		publisher = strtok(NULL, tab);
		tahun = strtok(NULL, tab);

		sprintf(filePathDir, "%s", filePath);
		getFileName(filePathDir, tempName);
		strrev(tempName);
		nameFile = strtok(tempName, ".");
		ekstensi = strtok(NULL, ".");

		printf("Nama: %s\n", nameFile);
		printf("Publisher: %s\n", publisher);
		printf("Tahun publishing: %s\n", tahun);
		printf("Esktensi file: %s\n", ekstensi);
		printf("FilePath: %s\n\n", filePath);

		status_val = send(rcv_clt, nameFile, SIZE, 0);
		status_val = send(rcv_clt, publisher, SIZE, 0);
		status_val = send(rcv_clt, tahun, SIZE, 0);
		status_val = send(rcv_clt, ekstensi, SIZE, 0);
		status_val = send(rcv_clt, filePath, SIZE, 0);
	}
	fclose(fp);
	status_val = send(rcv_clt, "done", SIZE, 0);
}

void findBooks(int send_clt, int rcv_clt){
	int status_val, found = 0;
    char *publisher, *tahun, *filePath, 
			*nameFile, *ekstensi;
	char bookFind[SIZE], line[512], filePathDir[SIZE], tempName[SIZE];
	const char tab[2] = "\t";

    status_val = recv(send_clt, bookFind, SIZE, 0);
    printf("FILE TO FIND --- %s\n", bookFind);

    FILE *fp = fopen("files.tsv", "r");
    while(fgets(line, 512, fp))
	{
        char *newline = strchr( line, '\n' );
		if ( newline )
			*newline = 0;
		
		filePath = strtok(line, tab);
        if(strstr(filePath, bookFind) == 0)
            continue;

        found = 1;           
		publisher = strtok(NULL, tab);
		tahun = strtok(NULL, tab);

		sprintf(filePathDir, "%s", filePath);
		getFileName(filePathDir, tempName);
		strrev(tempName);
		nameFile = strtok(tempName, ".");
		ekstensi = strtok(NULL, ".");

		printf("Nama: %s\n", nameFile);
		printf("Publisher: %s\n", publisher);
		printf("Tahun publishing: %s\n", tahun);
		printf("Esktensi file: %s\n", ekstensi);
		printf("FilePath: %s\n\n", filePath);

		status_val = send(rcv_clt, nameFile, SIZE, 0);
		status_val = send(rcv_clt, publisher, SIZE, 0);
		status_val = send(rcv_clt, tahun, SIZE, 0);
		status_val = send(rcv_clt, ekstensi, SIZE, 0);
		status_val = send(rcv_clt, filePath, SIZE, 0);
    }

    fclose(fp);
	status_val = send(rcv_clt, "done", SIZE, 0);
    if(!found)
        status_val = send(rcv_clt, "file not found", SIZE, 0);
    else
        status_val = send(rcv_clt, "found", SIZE, 0);
}

int main()
{
    fd_set read_fd_set;
    struct sockaddr_in new_addr;
    int server_fd, new_fd, i, serving = 1;
    int ret_val, ret_val1, ret_val2, ret_val3, ret_val4, status_val;
    char message[SIZE], id[SIZE], password[SIZE], cmd[SIZE];
    socklen_t addrlen;
    char buf[DATA_BUFFER];
    int all_connections[MAX_CONNECTIONS];
    
	if(access("akun.txt", F_OK ) != 0 )
	{
		FILE *fp = fopen("akun.txt", "w+");
		fclose(fp);
	} 
    if(access("files.tsv", F_OK ) != 0 )
	{
		FILE *fp = fopen("files.tsv", "w+");
		fclose(fp);
	}
	if(access("running.log", F_OK ) != 0 )
	{
		FILE *fp = fopen("running.log", "w+");
		fclose(fp);
	}  

    /* Get the socket server fd */
    server_fd = create_tcp_server_socket(); 
    if (server_fd == -1) {
        fprintf(stderr, "Failed to create a server\n");
        return -1; 
    }

    /* Initialize all_connections and set the first entry to server fd */
    for(i=0;i < MAX_CONNECTIONS;i++) {
        all_connections[i] = -1;
    }
    all_connections[0] = server_fd;

	printf("\nServer is running....\n\n");
    int userLoggedIn = 0;
    while (1) {
        FD_ZERO(&read_fd_set);
        /* Set the fd_set before passing it to the select call */
        for (i=0;i < MAX_CONNECTIONS;i++) {
            if (all_connections[i] >= 0) {
                FD_SET(all_connections[i], &read_fd_set);
            }
        }

        /* Invoke select() and then wait! */
        ret_val = select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL);

        /* select() woke up. Identify the fd that has events */
        if (ret_val >= 0 )
		{
            /* Check if the fd with event is the server fd */
            if (FD_ISSET(server_fd, &read_fd_set))
			{ 
                /* accept the new connection */
                new_fd = accept(server_fd, (struct sockaddr*)&new_addr, &addrlen);
                if (new_fd >= 0) {
                    printf("Accepted a new login try:\n\n");
                    for (i=0;i < MAX_CONNECTIONS;i++)
					{
                        if (all_connections[i] < 0)
						{
                            all_connections[i] = new_fd;
                            if(i != serving)
                                ret_val1 = send(all_connections[i], "wait",  SIZE, 0);
							else
                                ret_val1 = send(all_connections[i], "serve",  SIZE, 0);
                            break;
                        }
                    }
                }
				else
				{
                    fprintf(stderr, "accept failed [%s]\n", strerror(errno));
                }
                ret_val--;
                if (!ret_val) continue;
            } 

            /* Check if the fd with event is a non-server fd */
            // step2
            for (i=1;i < MAX_CONNECTIONS;i++) {
                if ((all_connections[i] > 0) &&
                    (FD_ISSET(all_connections[i], &read_fd_set))) {
                    // read command from client 
                    ret_val3 = recv(all_connections[i], cmd, sizeof(cmd), 0);
                    printf("Returned fd is %d [index, i: %d]\n", all_connections[i], i);
                    printf("State: %s\n", cmd);

                    //check if client terminate
                    if (ret_val1 == 0 || ret_val2 == 0 || ret_val3 == 0) {
                        printf("Id of the user now : %s\n", id);
                        printf("Password of the user now : %s\n", password);
                        printf("Closing connection for fd:%d\n\n", all_connections[i]);
                        close(all_connections[i]);
                        all_connections[i] = -1; /* Connection is now closed */

                        //make another client wait while a client is loggein
                        while(1)
						{
                            if(serving == 9)
							{
                                serving = 1;
                                break;
                            }

                            if(all_connections[serving + 1] != -1)
							{
                                serving++;
                                break;
                            }

                            serving++;
                        }

                        userLoggedIn = 0;
                        if(all_connections[serving] != -1)
                            status_val = send(all_connections[serving], "serve",  SIZE, 0);
                    } 
                    if (ret_val3 > 0)
					{
                        if(!strcmp(cmd, "register") || !strcmp(cmd, "login"))
                              registerLogin(all_connections[i], cmd, id, password, &userLoggedIn,  all_connections[serving] );
                        else {
                            if(userLoggedIn)
							{
                                printf("Kamu telah berhasil register atau login\n\n");

                                if(!strcmp(cmd, "add"))
									addBooks(all_connections[i], all_connections[serving], id, password);
                                else if(!strcmp(cmd, "download"))
									downloadBooks(all_connections[i], all_connections[serving]);
                                else if(!strcmp(cmd, "delete"))
									deleteBooks(all_connections[i], all_connections[serving], id, password);
                                else if(!strcmp(cmd, "see"))
									seeBooks(all_connections[serving]);
                                else if(!strcmp(cmd, "find"))
									findBooks(all_connections[i], all_connections[serving]);
                            }
							else
							{
                                status_val = send(all_connections[serving], "notlogin\n", SIZE, 0);
                                continue;
                            }
                        }
                        
                        printf("Id of the user now : %s\n", id);
                        printf("Password of the user now : %s\n\n", password);
                    } 
                    if (ret_val1 == -1 || ret_val2 == -1 || ret_val3 == -1)
					{
                        printf("recv() failed for fd: %d [%s]\n", 
                            all_connections[i], strerror(errno));
                        break;
                    }
                }
                ret_val1--;
                ret_val2--;
                ret_val3--;

                if (!ret_val1) continue;
                if (!ret_val2) continue;
                if (!ret_val3) continue;
            }
		} 
    }

    /* Last step: Close all the sockets */
    for(i=0;i < MAX_CONNECTIONS;i++) {
        if (all_connections[i] > 0) {
            close(all_connections[i]);
        }
    }

    return 0;
}
