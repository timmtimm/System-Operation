#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <malloc.h>
#include <sys/stat.h>
#include <limits.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
#include <dirent.h>
#include <strings.h>

#define PORT 8080
#define arraySize 1024

// Global variable for easy passing
char logMessage[arraySize];
char currentWorkingDirectory[PATH_MAX];
char commandInput[arraySize];
char sendToClientMessage[arraySize];
char buffer[arraySize];
char runResult[arraySize];
char printToTerminal[arraySize];
char logPath[arraySize];
char sendSelect[arraySize];
int fd_in;

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


struct tableList
{
    char columnName[arraySize];
    char columnType[arraySize];
};

// Start Logs function

time_t my_time;
struct tm * timeinfo;

void storeToLogs()
{
    time(&my_time);
	timeinfo = localtime(&my_time);

	char day[10], month[10], year[10], hour[10], minute[10], second[10];

	sprintf(day, "%d", timeinfo->tm_mday);
	if(timeinfo->tm_mday < 10)
		sprintf(day, "0%d", timeinfo->tm_mday);

	sprintf(month, "%d", timeinfo->tm_mon+1);
	if(timeinfo->tm_mon+1 < 10)
		sprintf(month, "0%d", timeinfo->tm_mon+1);

	sprintf(year, "%d", timeinfo->tm_year+1900);

	sprintf(hour, "%d", timeinfo->tm_hour);
	if(timeinfo->tm_hour < 10)
		sprintf(hour, "0%d", timeinfo->tm_hour);

	sprintf(minute, "%d", timeinfo->tm_min);
	if(timeinfo->tm_min < 10)
		sprintf(minute, "0%d", timeinfo->tm_min);

	sprintf(second, "%d", timeinfo->tm_sec);
	if(timeinfo->tm_sec < 10)
		sprintf(second, "0%d", timeinfo->tm_sec);

    // ini diapus usernamenya jangan lupa dibalikin
    sprintf(logMessage, "%s-%s-%s %s:%s:%s:%s",  year, month, day, hour, minute, second, commandInput);
    sprintf(printToTerminal, "|->%s %s\n", runResult, logMessage);
    printf("%s", printToTerminal);

    sprintf(logPath, "%s/logs", currentWorkingDirectory);

    FILE* fptr_logs = fopen(logPath, "a");
    fprintf(fptr_logs, "%s\n", logMessage);
    fclose(fptr_logs);
}

// End of Logs function

//// Start necessary Server-Client function

void sendToClient(int client_fd, char *message)
{
	send(client_fd, message, strlen(message), 0);
}

char* readFromClient(int client_fd)
{
	bzero(buffer, arraySize);
	read(client_fd, buffer, arraySize);
	buffer[strcspn(buffer, "\n")] = 0;
	return convertToCharPtr(buffer);
}

//// End of necessary Server-Client function

// Start Necessary function

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

char *trimString(char *str)
{
    if(str == NULL) return NULL;

    size_t len = strlen(str);

    while(isspace(str[len - 1])) --len;
    while(*str && isspace(*str)) ++str, --len;

    return strndup(str, len);
}

char *getStringBetween(char *str, char *PATTERN1, char *PATTERN2)
{
    if(PATTERN1 == NULL)
    {
        char temp[arraySize]; sprintf(temp, "Temp%s", str);
        return getStringBetween(temp, "Temp", PATTERN2);
    }
    else if(PATTERN2 == NULL)
    {
        char temp[arraySize]; sprintf(temp, "%sTemp", str);
        return getStringBetween(temp, PATTERN1, "Temp");
    }

    char *target = NULL;
    char *start, *end;

    if(start = strstr(str, PATTERN1))
    {
        start += strlen(PATTERN1);
        if (end = strstr(start, PATTERN2))
        {
            target = (char *)malloc(end - start + 1);
            memcpy(target, start, end - start);
            target[end - start] = '\0';
        }
    }

    if(target == NULL)
        return NULL;
    else
        return target;
}

char* removeStrQuotes(char* str)
{
    if(str[0] == '\'' && str[strlen(str)-1] == '\'')
        return getStringBetween(str, "'", "'");
    else if(str[0] == '"' && str[strlen(str)-1] == '"')
        return getStringBetween(str, "\"", "\"");
    else
        return str;
}

//// Start function for avoid Segmentation Fault
static jmp_buf context;
typedef void (*func_t)(void);

static void sig_handler(int signo)
{
    longjmp(context, 1);
}

static void catch_segv(int catch)
{
    struct sigaction sa;

    if(catch)
    {
        memset(&sa, 0, sizeof(struct sigaction));
        sa.sa_handler = sig_handler;
        sa.sa_flags = SA_RESTART;
        sigaction(SIGSEGV, &sa, NULL);
    }
    else
    {
        sigemptyset(&sa);
        sigaddset(&sa, SIGSEGV);
        sigprocmask(SIG_UNBLOCK, &sa, NULL);
    }
}

static int safe_run(func_t func)
{
    catch_segv(1);

    if (setjmp(context))
    {
            catch_segv(0);
            return 0;
    }

    func();
    catch_segv(0);
    return 1;
}
//// End of function for avoid Segmentation Fault

// End of Necessary function

// Authentication the user is root or noot
bool authenticationRoot()
{
    int a=getuid();

    if(a==0)
        return 1;
    else
        return 0;
}

void createUser(char* username, char* password)
{
    FILE *fp;
    char path[arraySize] = "user.csv";

    fp = fopen (path, "a");

    fprintf(fp, "%s,%s\n", username, password);

    fclose(fp);

    printf(">> User created <<\n");
    return;
}

void grantPermission(char* databaseName, char* username)
{
    int found = 0, line = 0;
    char buffer[512];
    const char delim[2] = ",";
    char *tempUser, *tempPass;

    FILE *fp = fopen("user.csv", "r");
    
    while(fgets(buffer, 512, fp))
    {
        tempUser = strtok(buffer, delim);
        tempPass = strtok(NULL, "\n");
        tempPass = strtok(tempPass, " ");

        if(strcmp(tempUser, username)==0)
        {
            found = 1;
            break;
        }

        if(strchr(buffer, "\n"))
        {
            line++;
        }
    }

    fclose(fp);

    if(found==1)
    {
        // printf("%s:%s di line nomor %d\n", tempUser, tempPass, line+1);

        FILE *fp = fopen("user.csv", "a+");
        fprintf(fp, "%s,%s,%s\n", tempUser, tempPass, databaseName);
        fclose(fp);

        printf(">> Permission saved <<\n");
    } else 
    {
        printf(">> Can't find the username <<\n");
    }
    return;
}

void checkUse(char* databaseName)
{
    DIR *dp;
    struct dirent *ep;
    char path[arraySize]="";
    sprintf(path, "%s", currentWorkingDirectory);

    dp = opendir(path);

    if (dp != NULL)
    {
      while ((ep = readdir (dp)))
      {
        // printf("%s\n", ep->d_name);
          if (strcmp(ep->d_name, databaseName) == 0)
          {
            printf(">> Database is found <<\n");

            return;
          }
      }

      (void) closedir (dp);
    } else perror(">> Couldn't find the database <<\n");

    return;
}

void createDatabase(char* databaseName)
{
    pid_t child_id;
    int status;
    
    char path[arraySize]=" ";
    strcpy(path, databaseName);

    child_id = fork();

    if (child_id < 0) {
        printf(">> Database failed to create <<\n");
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child

        char *argv[] = {"mkdir", "-p", path, NULL};
        execv("/bin/mkdir", argv);
    } else {
        while((wait(&status)) > 0);
        printf(">> Database created <<\n");

        return;
    }
}

void createTable(char* databaseName, char* tableName, struct tableList tableDatabase[arraySize], int index)
{
    FILE *fp;
    char path[arraySize] = "";

    strcat(path, databaseName);
    strcat(path, "/");
    strcat(path, tableName);
    strcat(path, ".csv");

    fp = fopen (path, "w+");

    for(int i=0; i<index-1; i++)
    {
        fprintf(fp, "%s, ", tableDatabase[i].columnName);
    }
    fprintf(fp, "%s\n", tableDatabase[index-1].columnName);

    fclose(fp);

    printf(">> Table created <<\n");
    return;
}

void dropDatabase(char* databaseName)
{
    id_t child_id;
    int status;
    
    char path[arraySize]=" ";
    strcpy(path, databaseName);

    child_id = fork();

    if (child_id < 0) {
        printf(">> Database failed to delete <<\n");
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child

        char *argv[] = {"rm", "-r", path, NULL};
        execv("/bin/rm", argv);
    } else {
        while((wait(&status)) > 0);
        printf(">> Database deleted <<\n");

        return;
    }
}

void dropTable(char* databaseName, char* tableName)
{
    id_t child_id;
    int status;
    
    char path[arraySize]=" ";
    strcpy(path, databaseName);
    strcat(path, "/");
    strcat(path, tableName);
    strcat(path, ".csv");

    child_id = fork();

    if (child_id < 0) {
        printf(">> Table failed to delete <<\n");
        exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
        // this is child

        char *argv[] = {"rm", "-d", path, NULL};
        execv("/bin/rm", argv);
    } else {
        while((wait(&status)) > 0);
        printf(">> Table deleted <<\n");

        return;
    }
}

// DML
void insertData(char* databaseName, char* tableName, char separateValue[100][arraySize], int index)
{
    DIR *dp;
    struct dirent *ep;
    char path[arraySize]="";

    strcat(path, databaseName);
    strcat(path, "/");

    strcat(tableName, ".csv");

    // printf("%s\n", path);

    dp = opendir(path);

    if (dp != NULL)
    {
      while ((ep = readdir (dp)))
      {
        // printf("%s\n", ep->d_name);
          if (strcmp(ep->d_name, tableName) == 0)
          {
            FILE *fp;
            char pathtable[arraySize]="";

            strcpy(pathtable, path);
            strcat(pathtable, tableName);

            // printf("%s\n", pathtable);

            fp = fopen (pathtable, "a+");

            for(int i=0; i<index-1; i++)
            {
                fprintf(fp, "%s, ", separateValue[i]);
            }
            fprintf(fp, "%s\n", separateValue[index-1]);

            fclose(fp);

            printf(">> Insert data to table successfull <<\n");

            return;
          }
      }

      (void) closedir (dp);
    } else perror ("Couldn't open the directory");

    return;
}

void updateSet(char* databaseName, char* tableName, char* columnName, char* valueTable)
{
    char line[arraySize];
    char path[arraySize]="";

    strcpy(path, databaseName);
    strcat(path, "/");
    strcat(path, tableName);
    strcat(path, ".csv");

    // mendapatkan nama kolom
    FILE *fp;
    fp = fopen(path, "r");

    fgets(line, arraySize, fp);

    fclose(fp);

    // nama kolom dipisah ke array
    int index=0;
    char nama_kolom[100][arraySize];

    char* token;
    token = strtok(line, ",\n ");


    while(token != NULL)
    {
        strcpy(nama_kolom[index], token);

        token = strtok(NULL, ",\n ");
        index++;
    }

    // cek nama kolom yang sama, terus ganti
    for(int i=0; i<index; i++)
    {
        if(!strcmp(nama_kolom[i], columnName))
        {
            strcpy(nama_kolom[i], valueTable);
        }
    }

    // path untuk file temporary
    char pathtemp[arraySize];
    strcpy(pathtemp, databaseName);
    strcat(pathtemp, "/temp.csv");

    FILE *ftemp;
    FILE *file;
    ftemp = fopen(pathtemp, "w");
    file = fopen(path, "r");

    int count=0;
    while(fgets(line, arraySize, file) != NULL)
    {
        count++;

        if(count==1)
        {
            for(int i=0; i<index-1; i++)
            {
                fprintf(ftemp, "%s,", nama_kolom[i]);
            }
            fprintf(ftemp, "%s\n", nama_kolom[index-1]);
        }
        else {
            fprintf(ftemp, "%s\n", line);
        }
    }

    fclose(file);
    fclose(ftemp);

    remove(path);
    rename(pathtemp, path);

    printf(">> Column name updated <<\n");

    return;
}

void selectAll(char* databaseName, char* tableName)
{
    sprintf(sendSelect, "");
    
    char line[arraySize];
    char path[arraySize]="";

    strcpy(path, databaseName);
    strcat(path, "/");
    strcat(path, tableName);
    strcat(path, ".csv");

    FILE *fp;
    fp = fopen(path, "r");

    while(fgets(line, arraySize, fp) != NULL)
    {
        sprintf(sendSelect, "%s%s", sendSelect, line);

        // int index=0;
        // char kolom[100][arraySize];

        // char* token;
        // token = strtok(line, ",\n ");


        // while(token != NULL)
        // {
        //     strcpy(kolom[index], token);

        //     token = strtok(NULL, ",\n ");
        //     printf("token: %s\n", token);
        //     index++;
        // }

        // for(int i=0; i<index-1; i++)
        // {
        //     sprintf(sendSelect, "%s%s", sendSelect, kolom[i]);
        // }
        // printf("%s", sendSelect);
    }

    printf("%s", sendSelect);
    sendToClient(fd_in, sendSelect);

    fclose(fp);

    return;
}

void compareFunction()
{
    // Get first command
    commandInput[arraySize] = trimString(commandInput);
    char commandTemp[arraySize];
    sprintf(commandTemp, "%s", commandInput);
    char *command = setUpString(getStringBetween(commandTemp, NULL, " "));
    // printf("%s\n", command);

    if(!strcmp(command, "CREATE"))
    {
        char *temp = getStringBetween(commandTemp, "CREATE ", " ");
        
        // Authentication
        if(!strcmp(temp, "USER"))
        {
            char *username = trimString(getStringBetween(commandTemp, "USER ", " "));

            char anotherTemp[arraySize] = "";
            sprintf(anotherTemp, "%s ", username);
            temp = getStringBetween(commandTemp, anotherTemp, " ");

            if(!strcmp(temp, "IDENTIFIED"))
            {
                sprintf(anotherTemp, "IDENTIFIED ");
                temp = getStringBetween(commandTemp, anotherTemp, " ");
                
                if(!strcmp(temp, "BY"))
                {
                    char *password = trimString(getStringBetween(commandTemp, "BY ", ";"));
                    
                    printf("\nCommand: Authentication\nDefinition: Get access\n");
                    printf("Command input: %s\n", commandInput);

                    printf("Username: %s\n", username);
                    printf("Password user: %s\n", password);

                    createUser(username, password);
                }
                else
                {
                    printf("Error command: Create\n");
                }
            }
            else
            {
                printf("Error command: Create\n");
            }
        }
        // Data Definition Language (DDL)
        else if(!strcmp(temp, "DATABASE") || !strcmp(temp, "TABLE"))
        {
            char *databaseName;

            if(!strcmp(temp, "DATABASE"))
            {
                printf("\nCommand: Create Database\nDefinition: Create Database\n");
                printf("Command input: %s\n", commandInput);

                databaseName = trimString(getStringBetween(commandTemp, "DATABASE ", ";"));
                printf("Database Name: %s\n", databaseName);

                createDatabase(databaseName);
            }
            else if(!strcmp(temp, "TABLE"))
            {
                char *tableName = trimString(getStringBetween(commandTemp, "TABLE ", " "));
                char *tableDefinition = trimString(getStringBetween(commandTemp, "(", ")"));

                printf("\nCommand: Create Table\nDefinition: Create Table on Database\n");
                printf("Command input: %s\n", commandInput);

                printf("Table Name: %s\n", tableName);
                printf("Table Definition: %s\n", tableDefinition);

                struct tableList tableDatabase[arraySize] = {0};
                int index = 0;
                char *tok = strtok(tableDefinition, ",");

                while(tok != NULL)
                {
                    tok = trimString(tok);
                    printf("Column %d\n", index);

                    char *columnName = getStringBetween(tok, NULL, " ");
                    char *columnType = getStringBetween(tok, " ", NULL);

                    printf("Column Name : %s\n", columnName);
                    printf("Column Type : %s\n", columnType);
                    sprintf(tableDatabase[index].columnName, "%s", columnName);
                    sprintf(tableDatabase[index].columnType, "%s", columnType);

                    tok = strtok(NULL, ",");
                    index++;
                }

                printf("\n");

                createTable(databaseName, tableName, tableDatabase, index);
            }
        }
    }
    // Authorization
    else if(!strcmp(command, "USE") || !strcmp(command, "DROP") || !strcmp(command, "INSERT") || !strcmp(command, "UPDATE") || !strcmp(command, "SELECT"))
    {
        char *databaseName;

        if(!strcmp(command, "USE"))
        {
            printf("\nCommand: Authorization\nDefinition: Use Database\n");
            printf("Command input: %s\n", commandInput);

            databaseName = trimString(getStringBetween(commandTemp, "USE ", ";"));
            printf("Database Name: %s\n", databaseName);

            checkUse(databaseName);
        }
        // Data Definition Language (DDL)
        else if(!strcmp(command, "DROP"))
        {
            char *temp = getStringBetween(commandTemp, "DROP ", " ");

            if(!strcmp(temp, "DATABASE"))
            {
                printf("\nCommand: Drop Database\nDefinition: Drop Database\n");
                printf("Command input: %s\n", commandInput);
                
                char *databaseName = trimString(getStringBetween(commandTemp, "DATABASE ", ";"));
                printf("Database name: %s\n", databaseName);

                dropDatabase(databaseName);
            }
            else if(!strcmp(temp, "TABLE"))
            {
                printf("\nCommand: Drop Table\nDefinition: Drop Table from Database\n");
                printf("Command input: %s\n", commandInput);
                
                char *tableName = trimString(getStringBetween(commandTemp, "TABLE ", ";"));
                printf("Table name: %s\n", tableName);

                dropTable(databaseName, tableName);
            }
            else if(!strcmp(temp, "COLUMN"))
            {
                printf("\nCommand: Drop column\nDefinition: Drop column form Table\n");
                printf("Command input: %s\n", commandInput);

                char *columnName = trimString(getStringBetween(commandTemp, "COLUMN ", " "));
                char *tableName = trimString(getStringBetween(commandTemp, "FROM ", ";"));
                printf("Column name: %s\n", columnName);
                printf("Table name: %s\n", tableName);

                // dropColumn(databaseName, tableName, columnName);
            }
            else
            {
                printf("Error command: Drop\n");
            }
        }
        // Data Manipulation Language (DML)
        else if(!strcmp(command, "INSERT"))
        {
            char *temp = getStringBetween(commandTemp, "INSERT ", " ");

            if(!strcmp(temp, "INTO"))
            {
                printf("\nCommand: Insert Table\nDefinition: Insert data to Table\n");
                printf("Command input: %s\n", commandInput);

                char *tableName = trimString(getStringBetween(commandTemp, "INTO ", " "));
                char *valueTable = trimString(getStringBetween(commandTemp, " (", ");"));
                printf("Table name: %s\n", tableName);
                // printf("Value: %s\n", valueTable);

                char separateValue[100][arraySize];
                int index = 0;
                char *tok = strtok(valueTable, ",");

                while(tok != NULL)
                {
                    tok = trimString(tok);
                    tok = removeStrQuotes(tok);

                    sprintf(separateValue[index], "%s", tok);
                    printf("Value %d: %s\n", index, separateValue[index]);

                    tok = strtok(NULL, ",");
                    index++;
                }

                insertData(databaseName, tableName, separateValue, index);

                printf("\n");
            }
            else
            {
                printf("Error command: Insert\n");
            }
        }
        else if(!strcmp(command, "UPDATE"))
        {
            char *tableName = trimString(getStringBetween(commandTemp, "UPDATE ", " "));
            char anotherTemp[arraySize] = "";
            sprintf(anotherTemp, "%s ", tableName);
            char *temp = getStringBetween(commandTemp, anotherTemp, " ");
            char *columnName = "";
            char *valueTable= "";

            if(!strcmp(temp, "SET"))
            {
                if(trimString(getStringBetween(commandTemp, "=", " ")) == NULL)
                {
                    columnName = trimString(getStringBetween(commandTemp, "SET ", "="));
                    valueTable = trimString(getStringBetween(commandTemp, "=", ";"));

                    sprintf(anotherTemp, "%s", valueTable);
                    temp = getStringBetween(commandTemp, anotherTemp, ";");

                    updateSet(databaseName, tableName, columnName, valueTable);
                }
                else
                {
                    columnName = trimString(getStringBetween(commandTemp, "SET ", "="));
                    valueTable = trimString(getStringBetween(commandTemp, "=", " "));
                    sprintf(anotherTemp, "%s ", valueTable);
                    temp = getStringBetween(commandTemp, anotherTemp, " ");
                }
                
                if(!strcmp(temp, "WHERE"))
                {
                    printf("\nCommand: Update data\nDefinition: Update data from Table with Where\n");
                    printf("Command input: %s\n", commandInput);

                    char *columnNameWhere = trimString(getStringBetween(commandTemp, "WHERE ", "="));
                    sprintf(anotherTemp, "WHERE %s=", columnNameWhere);
                    char *valueTableWhere = removeStrQuotes(trimString(getStringBetween(commandTemp, anotherTemp, ";")));
                    valueTable = removeStrQuotes(trimString(getStringBetween(commandTemp, "=", " ")));
                    printf("Table name: %s\n", tableName);
                    printf("Column name on Set: %s\n", columnName);
                    printf("Value on Set: %s\n", valueTable);
                    printf("Column name on Where: %s\n", columnNameWhere);
                    printf("Value on Where: %s\n", valueTableWhere);
                }
                else
                {
                    printf("\nCommand: Update Table\nDefinition: Update data from Table\n");
                    printf("Command input: %s\n", commandInput);

                    printf("Table name: %s\n", tableName);
                    printf("Column name: %s\n", columnName);
                    printf("Value: %s\n", valueTable);
                }
            }
        }
        else if(!strcmp(command, "SELECT"))
        {
            char *columnName = trimString(getStringBetween(commandTemp, "SELECT ", " "));
            char anotherTemp[arraySize] = "";
            sprintf(anotherTemp, "%s ", columnName);
            char *temp = getStringBetween(commandTemp, anotherTemp, " ");

            if(!strcmp(temp, "FROM"))
            {
                char *tableName = trimString(getStringBetween(commandTemp, "FROM ", " "));

                if(trimString(getStringBetween(commandTemp, "FROM ", " ")) == NULL)
                {
                    tableName = trimString(getStringBetween(commandTemp, "FROM ", ";"));
                    sprintf(anotherTemp, "%s", tableName);
                    temp = getStringBetween(commandTemp, anotherTemp, ";");

                    selectAll(databaseName, tableName);
                }
                else
                {
                    tableName = trimString(getStringBetween(commandTemp, "FROM ", " "));
                    sprintf(anotherTemp, "%s ", tableName);
                    temp = getStringBetween(commandTemp, anotherTemp, " ");
                }
                
                char separateColumn[100][arraySize];

                if(strcmp(columnName, "*"))
                {
                    sprintf(separateColumn, "%s", columnName);
                    printf("separateColumn: %s\n", separateColumn);

                    if(strstr(columnName, ","))
                    {
                        int index = 0;
                        char *tok = strtok(columnName, ",");

                        while(tok != NULL)
                        {
                            tok = trimString(tok);

                            sprintf(separateColumn[index], "%s", tok);
                            printf("Column %d: %s\n", index, separateColumn[index]);

                            tok = strtok(NULL, ",");
                            index++;
                        }
                    }

                }

                if(!strcmp(temp, "WHERE"))
                {
                    printf("\nCommand: Select Table\nDefinition: View data from Table with Where\n");
                    printf("Command input: %s\n", commandInput);

                    tableName = trimString(getStringBetween(commandTemp, "FROM ", " "));

                    
                    char *columnNameWhere = trimString(getStringBetween(commandTemp, "WHERE ", "="));
                    char *valueTableWhere = trimString(getStringBetween(commandTemp, "=", ";"));
                    

                    if(strstr(columnName, ","))
                    {
                        char *tok = strtok(columnName, ",");
                        while(tok != NULL)
                        {
                            printf("Column name: %s\n", columnName);
                        }  
                    }
                    else
                    {
                        printf("Column name: %s\n", columnName);
                    }
                    
                    printf("Table name: %s\n", tableName);
                    printf("Column name on Where: %s\n", columnNameWhere);
                    printf("Value on Where: %s\n", valueTableWhere);
                }
                else 
                {
                    printf("\nCommand: Select Table\nDefinition: View data from Table\n");
                    printf("Command input: %s\n", commandInput);

                    printf("Column name: %s\n", columnName);
                    printf("Table name: %s\n", tableName);
                }
            }
            else
            {
                printf("Error command: Select\n");
            }
        }
        else
        {
            printf("Error command: Invalid\n");
        }
    }
    else if(!strcmp(command, "GRANT"))
    {
        char *temp = getStringBetween(commandTemp, "GRANT ", " ");

        if(!strcmp(temp, "PERMISSION"))
        {
            char *databaseName = trimString(getStringBetween(commandTemp, "PERMISSION ", " "));
            char *temp = trimString(getStringBetween(trimString(getStringBetween(commandTemp, databaseName, NULL)), NULL, " "));
            
            if(!strcmp(temp, "INTO"))
            {
                printf("\nCommand: Grant Permission\nDefinition: Give permission to user\n");
                printf("Command input: %s\n", commandInput);

                char *username = trimString(getStringBetween(commandTemp, "INTO ", ";"));
                printf("Database Name: %s\n", databaseName);
                printf("Username: %s\n", username);

                grantPermission(databaseName, username);
            }
        }
        else
        {
            printf("Error command: Use\n");
        }
    }
    // Data Manipulation Language (DML)
    
    else if(!strcmp(command, "DELETE"))
    {
        char *temp = getStringBetween(commandTemp, "DELETE ", " ");

        if(!strcmp(temp, "FROM"))
        {
            char *tableName = trimString(getStringBetween(commandTemp, "FROM ", ";"));
            char anotherTemp[arraySize] = "";
            sprintf(anotherTemp, "%s ", tableName);
            char *temp = getStringBetween(commandTemp, anotherTemp, " ");

            if(trimString(getStringBetween(commandTemp, "FROM ", " ")) == NULL)
            {
                tableName = trimString(getStringBetween(commandTemp, "FROM ", ";"));
                sprintf(anotherTemp, "%s", tableName);
                temp = getStringBetween(commandTemp, anotherTemp, ";");
            }
            else
            {
                tableName = trimString(getStringBetween(commandTemp, "FROM ", " "));
                sprintf(anotherTemp, "%s ", tableName);
                temp = getStringBetween(commandTemp, anotherTemp, " ");
            }
            
            if(!strcmp(temp, "WHERE"))
            {
                printf("\nCommand: Delete data\nDefinition: Delete data from Table with Where\n");
                printf("Command input: %s\n", commandInput);

                tableName = trimString(getStringBetween(commandTemp, "FROM ", " "));
                char *columnNameWhere = trimString(getStringBetween(commandTemp, "WHERE ", "=")); 
                char *valueTableWhere = removeStrQuotes(trimString(getStringBetween(commandTemp, "=", ";")));
                printf("Table name: %s\n", tableName);
                printf("Column name on Where: %s\n", columnNameWhere);
                printf("Value on Where: %s\n", valueTableWhere);
            }
            else 
            {
                printf("\nCommand: Select Table\nDefinition: View data from Table\n");
                printf("Command input: %s\n", commandInput);

                printf("Table name: %s\n", tableName);
            }
        }
        else
        {
            printf("Error command: Delete\n");
        }
    }
}

int main(int argc, char const *argv[])
{
	int server_fd;
	struct sockaddr_in server;
	char *msg_in;
	
	// Make fd
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
		perror("Server Error: Socket not created succesfully");
		exit(EXIT_FAILURE);
	}
	
	// Initialize
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	// Bind
	bind(server_fd, (struct sockaddr *)&server, sizeof(server));

	// Set listen
	listen(server_fd, 3);

	int fd_in;
	int addrlen = sizeof(server);
	getpeername(server_fd, (struct sockaddr*)&server, (socklen_t*)&addrlen);
	printf("Host created: ip %s:%d \n",inet_ntoa(server.sin_addr) , ntohs(server.sin_port));
    
    getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory));

	while(1)
    {
		fd_in = accept(server_fd, (struct sockaddr*) &server, (socklen_t*)&addrlen);
		getpeername(server_fd, (struct sockaddr*)&server, (socklen_t*)&addrlen);
        
		sprintf(logMessage, "|-> New client connected with fd:%d", fd_in);
        printf("%s\n", logMessage);
		storeToLogs();

		sendToClient(fd_in, "Welcome to Sishopu F07 DBMS\n");
        char message[arraySize];
        sprintf(message, "HEHE");

        while(strcmp(setUpString(message), "END"))
        {
            char *response = readFromClient(fd_in);
            sprintf(commandInput, "%s", response);

            sprintf(sendToClientMessage, "");
            if(safe_run(compareFunction))
            {
                sprintf(runResult, "Success");
                storeToLogs();
                sprintf(sendToClientMessage, runResult);
            }
            else
            {
                sprintf(runResult, "Failed");
                storeToLogs();
                sprintf(sendToClientMessage, runResult);
            }
            
            sprintf(sendToClientMessage, "%s\n", sendToClientMessage);

            sendToClient(fd_in, sendToClientMessage);       
        }

		getpeername(server_fd, (struct sockaddr*)&server, (socklen_t*)&addrlen);
		sprintf(logMessage, "Client disconnected with fd:%d", fd_in);
		storeToLogs();

		close(fd_in);
	}
	return 0;
}
