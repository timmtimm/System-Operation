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

void hapussubstr (char *str, char *substr) {
    char *match;
    int len_sub = strlen(substr);
    
    while ((match = strstr(str, substr))) {
      	*match = '\0';
      	strcat(str, match+len_sub);
    }
}

void buatfolder(char jenis[15], char path[]) {
    pid_t child_id;
    int status;
    
    char path_temp[100]="";
    strcpy(path_temp, path);
    strcat(jenis, "/");
    strcat(path_temp, jenis);

    child_id = fork();

    if (child_id < 0) {
      	exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
    	// this is child

       	char *argv[] = {"mkdir", "-p", path_temp, NULL};
       	execv("/bin/mkdir", argv);
    } else {
        while((wait(&status)) > 0);
    	return;
    }
}

void pindahfoto(char *file, char jenis[15], char nama[15], char path[]) {
	//memindahkan file dengan cara dicopy
    pid_t child_id;
    int status;

    child_id = fork();

    //path file asli
    char pathasli[100]="";
    strcat(pathasli, path);
    strcat(pathasli, file);

    //path folder sekalian ganti nama
    char pathfile[100]="";
    strcat(pathfile, path);
    strcat(pathfile, jenis);
    strcat(pathfile, "/");
    strcat(pathfile, nama);
    strcat(pathfile, ".jpg");

    if (child_id < 0) {
      	exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
    }

    if (child_id == 0) {
       	// this is child

    	char *argv[] = {"cp", pathasli, pathfile, NULL};
       	execv("/bin/cp", argv);
    } else {
        while((wait(&status)) > 0);
        return;
    }
}

void keterangan(char jenis[15], char nama[15], char umur[15], char path[]) {
    FILE *fp;
    char pathfolder[100] = "";

    strcat(pathfolder, path);
    strcat(pathfolder, jenis);
    strcat(pathfolder, "/keterangan.txt");

    fp = fopen (pathfolder, "a");
    fprintf(fp, "nama: %s\numur: %s tahun\n\n", nama, umur);

    fclose(fp);
    return;
}

void managefile() {
    DIR *dp;
    struct dirent *ep;
    char path[] = "/home/rosaam/modul2/petshop/";

    dp = opendir(path);

    if (dp != NULL)
    {
		while ((ep = readdir (dp))) {
          
			if(strcmp(ep->d_name, ".") !=0 && strcmp(ep->d_name, "..") !=0) {

            	//hapus folder yang tidak dibutuhkan
             	if(ep->d_type == 4) {
                	char path[] = "/home/rosaam/modul2/petshop/";
            		strcat(path, ep->d_name);
                 
                	pid_t child_id;
                 	int status;

                 	child_id = fork();

                 	if (child_id < 0) {
                   		exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
                 	}

                 	if (child_id == 0) {
                   		char *argv[] = {"rm", "-r", path, NULL};
                   		execv("/bin/rm", argv);
                 	}
               	} else {
                	char path[] = "/home/rosaam/modul2/petshop/";
                 	char file[100];
                 	strcpy(file, ep->d_name);

                 	hapussubstr(file, ".jpg");
                 	char jenis[15];
                 	char nama[15];
                 	char umur[15];

                 	char *token;
                 	token = strtok(file, "_;");
             
                 	//memisahkan jenis, nama dan umur
                 	int i = 0;
                 	while(token != NULL) {
      
                   	if(i==0 | i==3) {
                    		strcpy(jenis, token);
                     		//membuat folder sesuai dengan jenis hewan
                     		buatfolder(jenis, path);
                   	}

                   	if(i==1 | i==4) {
                     		strcpy(nama, token);
                     		//memindahkan foto sesuai dengan kategori
                     		pindahfoto(ep->d_name, jenis, nama, path);
                   	}

                   	if(i==2 | i==5) {
                     		strcpy(umur, token);
                     		//membuat keterangan di tiap folder
                     		keterangan(jenis, nama, umur, path);
                   	}

                   	token = strtok(NULL, "_;");
                   	i++;
                }

                pid_t child_id;
                int status;

                child_id = fork();

                //file asli yg akan dihapus
                char pathasli[100] = "";
                strcat(pathasli, path);
                strcat(pathasli, ep->d_name);

                if (child_id < 0) {
                	exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
                }

                if (child_id == 0) {
                	char *argv[] = {"rm", "-r", pathasli, NULL};
                	execv("/bin/rm", argv);
                }
            }
        }
    }

    (void) closedir (dp);
    } else perror ("Couldn't open the directory");

    return;
}

int main() {
	pid_t child_id;
	int status;

	child_id = fork();

	if(child_id == 0) {
		pid_t child_id2;
      		child_id2 = fork();

      		if(child_id2 == 0) {
        		char *argv[] = {"mkdir", "-p", "/home/rosaam/modul2/petshop", NULL};
        		execv("/bin/mkdir", argv);
      		}
      		else {
        		while((wait(&status)) > 0);
        		char *argv[] = {"unzip", "pets.zip", "-d", "/home/rosaam/modul2/petshop", NULL};
        		execv("/usr/bin/unzip", argv);
      		}
	}
    else {
      	while((wait(&status)) > 0);
      	managefile();
    }
    return 0;
}
