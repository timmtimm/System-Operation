#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h> 
#include <netdb.h> 
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define SIZE_BUF 100

int registerLogin(int fd, char cmd[])
{
    int ret_val, isFound = 0;
	char id[SIZE_BUF], password[SIZE_BUF], message[SIZE_BUF];
	
    printf("Username: "); scanf("%s", id);
    ret_val = send(fd, id, SIZE_BUF, 0);

    printf("Password: "); scanf("%s", password);
    ret_val = send(fd, password, SIZE_BUF, 0);

    ret_val = recv(fd, message, SIZE_BUF, 0);
    puts(message);

    if(!strcmp(message, "regloginsuccess\n"))
        return 1;
    else if(!strcmp(message, "userfound\n")) {
        printf("Username or ID already exist !\n\n");
        return 0;
    }
    else if(!strcmp(message, "wrongpass\n")) {
        printf("Id or Passsword doesn't match !\n\n");
        return 0;
    }
	
}

void sendFile(int sockfd, char filePath[])
{
    int n;
    char data[SIZE_BUF] = {0};
    FILE *fp = fopen(filePath, "r");

    while(fgets(data, SIZE_BUF, fp) != NULL)
	{
        if(send(sockfd, data, sizeof(data), 0) == -1)
		{
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE_BUF);
    }

    fclose(fp);
    int ret_val = send(sockfd, "done", SIZE_BUF, 0);
}

void writeFile(int fd, char fileName[])
{
    int n;
    char buffer[SIZE_BUF];

    FILE *fp = fopen(fileName, "w");
    fclose(fp);

    while(1)
	{
        n = recv(fd, buffer, SIZE_BUF, 0);

        if (n <= 0)
		{
            break;
            return;
        }

        if(!strcmp(buffer, "done"))
            return;
		
        fp = fopen(fileName, "a");
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE_BUF);
        fclose(fp);
    }
    return;
}

void addBooks(int fd)
{
    int ret_val;
    char publisher[SIZE_BUF], tahun[SIZE_BUF], filePath[SIZE_BUF];
    
    printf("Publisher: "); scanf("%s", publisher);
    ret_val = send(fd, publisher, SIZE_BUF, 0);

    printf("Tahun Publikasi: "); scanf("%s", tahun);
    ret_val = send(fd, tahun, SIZE_BUF, 0);

    printf("Filepath: "); scanf("%s", filePath);
    ret_val = send(fd, filePath, SIZE_BUF, 0);

    sendFile(fd, filePath);
}

void downloadBooks(int fd)
{
    int ret_val;
    char books[SIZE_BUF], message[SIZE_BUF];

    printf("Book: "); scanf("%s", books);
    ret_val = send(fd, books, SIZE_BUF, 0);

    ret_val = recv(fd, message, SIZE_BUF, 0);
    puts(message);

    if(!strcmp(message, "Begin to download\n"))
		writeFile(fd, books);
}

void delete_books(int fd)
{
    int ret_val;
    char books[SIZE_BUF], message[SIZE_BUF];

    printf("Book: "); scanf("%s", books);
    ret_val = send(fd, books, SIZE_BUF, 0);

    ret_val = recv(fd, message, SIZE_BUF, 0);
    puts(message);

}

void seeBooks(int fd)
{
    int ret_val;
    char temp[SIZE_BUF], flag[100];
    int loop = 1;

    ret_val = recv(fd, flag, SIZE_BUF, 0);
    puts("");

    while(loop)
	{
        ret_val = recv(fd, temp, SIZE_BUF, 0);

        if(strstr(temp, "done") != NULL)
		{
            loop = 0;
            break;
        }
        printf("Nama: %s\n", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("Publisher: %s\n", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("Tahun publishing: %s\n", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("Esktensi file: %s\n", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("FilePath: %s\n\n", temp);
    }
}

void findBooks(int fd)
{
    int ret_val;
    char nameFile[SIZE_BUF], flag[100], temp[SIZE_BUF];
    int loop = 1;
    
    scanf("%s", nameFile);
    ret_val = send(fd, nameFile, SIZE_BUF, 0);
    
    while(loop){
        ret_val = recv(fd, temp, SIZE_BUF, 0);

        if(strstr(temp, "done") != NULL)
		{
            loop = 0;
            break;
        }
        printf("Nama: %s\n", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("Publisher: %s\n", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("Tahun publishing: %s", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("Esktensi file: %s\n", temp);

        ret_val = recv(fd, temp, SIZE_BUF, 0);
        printf("FilePath: %s\n\n", temp);

    }

    ret_val = recv(fd, flag, SIZE_BUF, 0);
    if(!strcmp(flag, "file not found"))
        printf("%s\n\n", flag);
}

int main()
{
    struct sockaddr_in saddr;
    int fd, ret_val;
    struct hostent *local_host; /* need netdb.h for this */
    char message[SIZE_BUF],  cmd[SIZE_BUF];

    /* Step1: create a TCP socket */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
    if (fd == -1) {
        fprintf(stderr, "socket failed [%s]\n", hstrerror(errno));
        return -1;
    }
    printf("Created a socket with fd: %d\n", fd);

    /* Let us initialize the server address structure */
    saddr.sin_family = AF_INET;         
    saddr.sin_port = htons(7000);     
    local_host = gethostbyname("127.0.0.1");
    saddr.sin_addr = *((struct in_addr *)local_host->h_addr);
    /* Step2: connect to the TCP server socket */
    ret_val = connect(fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
    if (ret_val == -1) {
        fprintf(stderr, "connect failed [%s]\n", hstrerror(errno));
        close(fd);
        return -1;
    }

    ret_val = recv(fd, message, SIZE_BUF, 0);
    while(strcmp(message, "wait") == 0) {
        printf("Server is full!\n");
        ret_val = recv(fd, message, SIZE_BUF, 0);
    }

    int commandTrue = 0;
    while(1)
    {
        // sign up user
        while(!commandTrue) {
            printf("Insert Command\n[-]register\n[-]login\n");
            printf("Command: "); scanf("%s", cmd);
            int b=0;
            for(b=0;b<strlen(cmd);b++)
			{
                cmd[b] = tolower(cmd[b]);
            }
			
            ret_val = send(fd, cmd, SIZE_BUF, 0);
            if(!strcmp(cmd, "register") || !strcmp(cmd, "login"))
			{
                if(registerLogin(fd, cmd))
                    commandTrue = 1;
            }
			else
			{
                ret_val = recv(fd, message, SIZE_BUF, 0);
                if(!strcmp(message, "notlogin\n")) {
                    printf("Silahkan register atau login terlebih dahulu\n\n");
                }
				else
				{
                    commandTrue = 1;
                }
            }
        }

        // other command
        while(1)
		{
            printf("Insert Command\n[-]add\n[-]download\n[-]delete\n[-]see\n[-]find\n");
            printf("Command: "); scanf("%s", cmd);
            ret_val = send(fd, cmd, SIZE_BUF, 0);
            
			if(!strcmp(cmd, "add"))
                addBooks(fd);
            else if(!strcmp(cmd, "download"))
                downloadBooks(fd);
            else if(!strcmp(cmd, "delete"))
                delete_books(fd);
            else if(!strcmp(cmd, "see"))
                seeBooks(fd);
            else if(!strcmp(cmd, "find"))
                findBooks(fd);
        }

        sleep(2);
        if(commandTrue) break;
    }

    /* Last step: close the socket */
    close(fd);
    return 0;
}
