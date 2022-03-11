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
#include <time.h>
#include <dirent.h>

// 1.A.
void makeFolder(pid_t pid, int *status, char *folderStevany[])
{
  pid = fork();

  if(pid == 0)
  {
    char *argv[] = {"mkdir", "-p", folderStevany[0], folderStevany[1], folderStevany[2], NULL};
    execv("/bin/mkdir", argv);
  }

  while(wait(status)>0);

}

// 1.B.
void downloadZip(pid_t pid, int *status, char *linkDownload[], char *saveName[])
{
  int i = 0;
  
  for(i ; i<3 ; i++)
  {
    pid = fork();

    if(pid == 0)
    {
      char *argv[] = {"wget", "-q","--no-check-certificate", linkDownload[i], "-O", saveName[i], NULL};
      execv("/usr/bin/wget", argv);
    }
  }

  while(wait(status)>0);

}

// 1.C.
void unZip(pid_t pid, int *status, char *saveName[])
{
  int i = 0;

  for(i ; i<3 ; i++)
  {
    pid = fork();
    
    if(pid == 0)
    {
    char *argv[] = {"unzip", saveName[i], NULL};
    execv("/usr/bin/unzip", argv);
    }
  }

  while(wait(status)>0);

}

// 1.D.
void moveFolder(pid_t pid, int *status, char *folderExtract[], char *moveTo[])
{
  int i = 0;
  
  for(i ; i<3 ; i++)
  {
    DIR *dp;
    struct dirent *ep;
    dp = opendir(folderExtract[i]);

    if(dp != NULL)
    {
      while((ep = readdir(dp)))
      {
          if(strcmp(ep->d_name, ".") != 0 || strcmp(ep->d_name, "..") != 0)
          {
            char file_to_move[355]; sprintf(file_to_move, "%s%s", folderExtract[i], ep->d_name);
            char file_move_to[355]; sprintf(file_move_to, "%s%s", moveTo[i], ep->d_name);
            
            int status;
            pid = fork();
            
            if(pid == 0)
            {
              char *argv[] = {"mv", file_to_move, file_move_to, NULL};
              execv("/bin/mv", argv);
            }

            while(wait(&status) > 0);

          }
      }
    }
  }

  while(wait(status)>0);
  
}

// 1.E.
void timeBefore6Hours(pid_t pid, int *status, char *folderStevany[], char *linkDownload[], char *saveName[], char *folderExtract[], char *moveTo[])
{
  makeFolder(pid, status, folderStevany);
  downloadZip(pid, status, linkDownload, saveName);
  unZip(pid, status, saveName);
  moveFolder(pid, status, folderExtract, moveTo);
}

// 1.F.
void removeFolder(pid_t pid, int *status, char *folderExtracted[], char *folderStevany[])
{
  pid = fork();

  if(pid == 0)
  {
    char *argv[] = {"rm", "-rf", folderExtracted[0], folderExtracted[1], folderExtracted[2],
                    folderStevany[0], folderStevany[1], folderStevany[2], NULL};
    execv("/bin/rm", argv);
  }

  while(wait(status)>0);

}

// 1.F.
void zipForStevany(pid_t pid, int *status, char *folderStevany[])
{
  pid = fork();

  if(pid == 0)
  {
    char *argv[] = {"zip", "-r", "Lopyu_Stevany.zip", folderStevany[0], folderStevany[1], folderStevany[2], NULL};
    execv("/bin/zip", argv);
  }

  while(wait(status)>0);
  
}

void runTime(int *status, char *folderStevany[], char *linkDownload[], char *saveName[], char *folderExtract[], char *moveTo[], char *folderExtracted[])
{
  pid_t pid;

  int bDay = 9;
  int bMonth = 4;
  int bHour = 22;
  int bMinute = 22;

  while(1)
  {
    time_t times = time(NULL);
    struct tm *timeNow = localtime(&times);

    int nDay = timeNow -> tm_mday;
    int nMonth = timeNow -> tm_mon + 1;
    int nHour = timeNow -> tm_hour;
    int nMinute = timeNow -> tm_min;
    int nSecond = timeNow -> tm_sec;

    if((bDay == nDay) && (bMonth == nMonth) && (bMinute == nMinute) && (0 == nSecond))
    {
      if(bHour - 6 == nHour)
      {
        timeBefore6Hours(pid, status, folderStevany, linkDownload, saveName, folderExtract, moveTo);
      }

      if(bHour == nHour)
      {
        zipForStevany(pid, status, folderStevany);
        removeFolder(pid, status, folderExtracted, folderStevany);
      }
    }

    sleep(1);
    
  }
}

// void makeFolder(pid_t pid, int *status, char *folderStevany[]);
// void downloadZip(pid_t pid, int *status, char *linkDownload[], char *saveName[]);
// void unZip(pid_t pid, int *status, char *saveName[]);
// void copyFolder(pid_t pid, int *status, char *FolderExtract[], char *moveTo[]);
// void timeBefore6Hours(pid_t pid, int *status, char *folderStevany[], char *linkDownload[], char *saveName[], char *pathFolderExtract[], char *moveTo[]);
// void removeFolder(pid_t pid, int *status, char *folderExtracted[], char *folderStevany[]);
// void zipForStevany(pid_t pid, int *status, char *folderStevany[]);

int main()
{
  char *folderStevany[]   = {"Fylm", "Pyoto", "Musyik"};
  char *linkDownload[]    = {"https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", 
                              "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download",
                              "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download"};
  char *saveName[]        = {"Film_for_Stevany.zip", "Foto_for_Stevany.zip", "Musik_for_Stevany.zip"};
  char *folderExtract[]   = {"/home/timtim/Desktop/SisOp/Praktikum 2/soal1/FILM/",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/FOTO/",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/MUSIK/"};
  char *moveTo[]          = {"/home/timtim/Desktop/SisOp/Praktikum 2/soal1/Fylm/",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/Pyoto/",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/Musyik/"};
  char *folderExtracted[] = {"FILM", "FOTO", "MUSIK"};
  char *folder = {"/home/timtim/Desktop/SisOp/Praktikum 2/soal1/"};

  int status;
  pid_t pid, sid;
  pid = fork();

  if(pid < 0) exit(EXIT_FAILURE);
  if(pid > 0) exit(EXIT_SUCCESS);

  umask(0);

  sid=setsid();
  if(sid < 0) exit(EXIT_FAILURE);
  if((chdir(folder)) < 0) exit(EXIT_FAILURE);

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  
  runTime(&status, folderStevany, linkDownload, saveName, folderExtract, moveTo, folderExtracted);

  return 0;
}
