#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <limits.h>
#include <stdbool.h>

#define PORT 8080
#define SIZE 1024

char currentWorkingDirectory[PATH_MAX];
char buffer[SIZE];

char *setUpString(char *str)
{
    if (str != NULL)
    {
        char *start = str;
        // printf("setUpString:\t%s\n\t", str);
        while (*str)
        {
            if (*str >= 'a' && *str <= 'z')
                *str = *str - 32;
            // printf("%c", *str);
            str++;
        }
        // printf("\n");
        return start;
    }
    else
    {
        return NULL;
    }
}

char* convertToCharPtr(char *str)
{
	int len=strlen(str);
	char* ret = malloc((len+1) * sizeof(char));
	for(int i=0; i<len; i++)
    {
		ret[i] = str[i];
	}
	ret[len] = '\0';
	return ret;
}

char* readServer(int server_fd)
{
	bzero(buffer, SIZE);
	read(server_fd, buffer, SIZE);
	buffer[strcspn(buffer, "\n")] = 0;
	return convertToCharPtr(buffer);
}

void sendtoServer(int server_fd, char *message)
{
	send(server_fd, message, strlen(message), 0);
}

int main(int argc, char const *argv[])
{
    getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory));
    int server_fd;
    bzero(buffer, SIZE);
    
    // set socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket failed\n");
        exit(EXIT_FAILURE);
    }
    
    int opt = 1;
    // setup socket option
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Initialize
    struct sockaddr_in server;
    memset(&server, '0', sizeof(server)); 

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    int message_in = inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    if(message_in < 0)
    {
        perror("Client Error: IP not initialized successfully");
        return 0;
    }

    // Connect to server
    message_in = connect(server_fd, (struct sockaddr*) &server, sizeof(server));
    if(message_in < 0)
    {
        perror("Client Error: Connection failed");
    }

    bzero(buffer,256);
    int n;
    while(1)
    {
        bzero(buffer,256);

        if((n = recv(server_fd, buffer, 256, 0)) > 0)
        {
            printf("%s",buffer);
            fflush(stdout);

            printf("\nCurrent command we can handle:\n");
            printf("[-]CREATE\n[-]USE\n[-]DROP\n[-]INSERT\n[-]DELETE\n[-]SELECT\n[-]END\n\n[->]");

            char message[SIZE];
            fgets(message, 255, stdin);
            printf("Result: ");
            sendtoServer(server_fd, message);

            
            if(!strcmp(setUpString(message), "END\n"))
                exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
