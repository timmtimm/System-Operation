#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <wait.h>
#include <dirent.h>
#include <time.h>

// 3.A.
void makeFolder(char namafolder[])
{
    pid_t pid;
    pid = fork();

    int status;
    if(pid == 0)
    {
        char *argv[] = {"mkdir","-p", namafolder, NULL};
        execv("/bin/mkdir",argv);
    }
    
    while ((wait(&status)) > 0);
}

// 3.B.
void download(char namafolder[])
{
    pid_t pid;
    int status;
    int i = 0;

    for(i ; i<10 ; i++)
    {
        pid = fork();

        time_t t = time(NULL);
        struct tm curTime = *localtime(&t);
        char namafile[100];
        clock_t start = time(NULL), end;
        strftime(namafile, sizeof(namafile),"%Y-%m-%d_%H:%M:%S",&curTime);

        if(pid == 0)
        {
            char source[50] = "https://picsum.photos/";
            char size[20];
            sprintf(size,"%ld",t%1000+50);
            strcat(source,size);

            char savefile[300];
            sprintf(savefile, "/home/timtim/Desktop/SisOp/Praktikum 2/soal3/%s/%s.jpeg", namafolder, namafile);

            char *argv[] = {"wget","-q",source, "-O", savefile, NULL};
            execv("/bin/wget",argv);
        }
        
        sleep(5);

    }
}

// 3.C.
void successStat(char namafolder[])
{
    char pesan[20] = "Download Success", ch;
    int i,key = 5;

    //Soal 3C : Enkripsi Caesar Cipher
    for(i = 0; pesan[i] != '\0'; ++i)
    {
		ch = pesan[i];
		if(ch >= 'a' && ch <= 'z'){
			ch = ch + key;
			if(ch > 'z')	ch = ch - 'z' + 'a' - 1;
			pesan[i] = ch;
		}
		else if(ch >= 'A' && ch <= 'Z'){
			ch = ch + key;
			if(ch > 'Z')	ch = ch - 'Z' + 'A' - 1;
			pesan[i] = ch;
		}
	}

    char path[112];
    sprintf(path, "%s/status.txt", namafolder);    
    FILE *fptr = fopen(path, "w");
    fputs(pesan, fptr);
    fclose(fptr);
}

void zipFile(char namafolder[])
{
    pid_t pid;
    pid = fork();
    int status;

    if(pid == 0)
    {
        char zip_path[150]; sprintf(zip_path, "%s.zip", namafolder);
        char *argv[] = {"zip", "-qrm", zip_path, namafolder, NULL};
        execv("/bin/zip", argv);
    }

    while(wait(&status) > 0);
}

// 3.D.
void fileKill(const char *argv[],int sid)
{
    FILE *fptr;
    fptr = fopen("killer.sh","w");
    fputs("#!/bin/bash\n\n",fptr);
    char program[100];

    if(!strcmp(argv[1],"-z"))
    {
        sprintf(program,"killall -9 %s\nrm killer.sh",argv[0]);
        fputs(program,fptr);
    }
    else if (!strcmp(argv[1],"-x"))
    {
        sprintf(program,"kill -9 %d\nrm killer.sh",sid);
        fputs(program,fptr);
    }

    fclose(fptr);
}

int main(int argc, const char **argv)
{
    pid_t parentPID;
    parentPID = getpid();

    if(!strcmp(argv[1],"-z") || !strcmp(argv[1],"-x"))
    {
        fileKill(argv, (int)parentPID);

        while(1)
        {
            time_t t = time(NULL);
            struct tm curTime = *localtime(&t);
            char namafolder[100];
            clock_t start = time(NULL), end;

            if(fork() == 0)
            {
                //soal 3A Membuat folder berdasarkan timestamp [YYYY-MM-dd_hh:mm:ss] per 40 detik
                strftime(namafolder, sizeof(namafolder),"%Y-%m-%d_%H:%M:%S",&curTime);
                makeFolder(namafolder);
                download(namafolder);
                successStat(namafolder);
                zipFile(namafolder);
                exit(EXIT_SUCCESS);
            }

            sleep(40);
        }
    }
}
