#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

static  const  char *dirPath = "/home/timtim/Documents";
static const char *logpath = "/home/timtim/SinSeiFS.log";

#define SIZE 1024
#define ARR_SIZE 100

#define ROT13 1
#define VIGENERE 2

time_t my_time;
struct tm * timeinfo;

char *strrev(char *str)
{
	char *p1, *p2;

	if (!str || !*str)
		return str;
	
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	
	return str;
}

char *getStrBetween(char *str, char *PATTERN1, char *PATTERN2){
    char *target = NULL;
    char *start, *end;

    if ( start = strstr( str, PATTERN1 ) ){
        start += strlen( PATTERN1 );
        if ( end = strstr( start, PATTERN2 ) ){
            target = ( char * )malloc( end - start + 1 );
            memcpy( target, start, end - start );
            target[end - start] = '\0';
        }
    }
    if(target == NULL){
        return "";
    }else{
        return target;
    }
}

void getAnything(char string[], char extension[], char delimiter)
{
	int temp = 0;
	int length = strlen(string) - 1;

	while(string[length] != delimiter)
	{
		extension[temp] = string[length];
		string[length] = '\0';
		
		temp++;
		length--;
	}

	extension[temp] = string[length];
	temp++;

	extension[temp] = '\0';
	string[length] = '\0';

	strrev(extension);
}

// start no 1
void cipherCode(char *string)
{
	int i;

	for(i = 0; i < strlen(string); i++)
	{
		if(string[i] >= 'A' && string[i] <= 'Z')
			string[i] = 'z' + 'A' - string[i];

		if(string[i] >= 'a' && string[i] <= 'z')
			string[i] = 'z'  + 'a' - string[i];
	}

}

void renamingInnerFile(const char *path)
{
	char fileName[1024];
	char old[1024];
	char new[1024];
	char save[1024];

	DIR *dp;
	struct dirent *de;

	dp = opendir(path);
	if (dp == NULL) return;

	while ((de = readdir(dp)) != NULL)
	{
		if(!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
			continue;
		
		sprintf(fileName, "%s", de->d_name);

		if(de->d_type == DT_DIR)
		{
            bzero(save, sizeof(save));
			cipherCode(fileName);
            printf("Cipher result : %s\n\n", fileName);
		}
		else if(de->d_type == DT_REG)
		{
            getAnything(fileName, save, '.');
			cipherCode(fileName);
            printf("Cipher result : %s%s\n\n", fileName, save);
		}

		sprintf(old, "%s/%s", path, de->d_name);
		sprintf(new, "%s/%s%s", path, fileName, save);

		if(de->d_type == DT_DIR && !strstr(fileName, "AtoZ_") && !strstr(de->d_name, "AtoZ_"))
		{
			renamingInnerFile(old);
		}

		printf("renamed inner folder\n");
		printf("old: %s\n", old);
		printf("new: %s\n\n", new);
		rename(old, new);
	}

	closedir(dp);

}

// end of no 1

// start no 2

struct rx
{
    char DIR[SIZE];    
    int CHIPER;    
};

struct rx rx_directory[ARR_SIZE];
int rx_last_idx = 0;

void rx_insert(char* dir, int chiper)
{
    sprintf(rx_directory[rx_last_idx].DIR, "%s", dir);
    rx_directory[rx_last_idx].CHIPER = chiper;
    rx_last_idx++;
}

bool rx_contains(char *dir)
{
    for(int i=0; i<rx_last_idx; i++){
        if(!strcmp(rx_directory[i].DIR, dir)) return true;
    }
    return false;
}

int rx_insubstr(char *dir)
{
    int to_return = -1;
    for(int i=0; i<rx_last_idx; i++){
        if(strstr(dir, rx_directory[i].DIR)){
            to_return = i;
        }
    }
    return to_return;
}

void rx_remove(char *dir)
{
    for(int i=0; i<rx_last_idx; i++)
    {
        if(!strcmp(rx_directory[i].DIR, dir))
        {
            strcpy(rx_directory[i].DIR, "xxx..............xxx");
        }
    }
}

char *chaesarEncrypt(char* str, int shift)
{
    int len = strlen(str);
    char *ret = malloc((len+1) * sizeof(char));

    for(int i=0; i<len; i++)
    {
        if(str[i] >= 97 && str[i] <= 122)
        {
            int asciinum = str[i] - 'a';
            asciinum = 97 + (asciinum+shift)%26;
            ret[i] = asciinum;
        }
        else if (str[i] >= 65 && str[i] <= 90)
        {
            int asciinum = str[i] - 'A';
            asciinum = 65 + (asciinum+shift)%26;
            ret[i] = asciinum;
        }
        else
        {
            ret[i] = str[i];
        }
    }
    ret[len] = '\0';

    return ret;
}

char *vignereEncrypt(char *str, char *key)
{    
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    sprintf(str_copy, "%s", str);
    char temp[SIZE]; sprintf(temp, "%s", str);

    int i = 0, curKey = 0;
    for(i = 0; i < strlen(str_copy); i++)
    {
        if(str_copy[i] >= 'a' && str_copy[i] <= 'z')
        {
            str_copy[i] = str_copy[i] - 'a' + 'A';
        }
    }

    for(int i = 0; i < strlen(str_copy); i++)
    {
        if(curKey == strlen(key)) curKey = 0;

        if(str_copy[i] >= 'A' && str_copy[i] <= 'Z')
            str_copy[i] = ((str_copy[i] + key[curKey]) % 26);
            
        if(temp[i] >= 'a' && temp[i] <= 'z')
            str_copy[i] += 'a';
        else if(temp[i] >= 'A' && temp[i] <= 'Z')
            str_copy[i] += 'A';
        else
            curKey--;
        
        curKey++;
    }

    str_copy[strlen(str)] = 0;
    return str_copy;
}

char* vignereDecrypt(char *str, char *key)
{    
    char *str_copy = malloc((strlen(str)+1) * sizeof(char));
    sprintf(str_copy, "%s", str);
    char temp[SIZE]; sprintf(temp, "%s", str);

    int i = 0, curKey = 0;
    for(i = 0; i < strlen(str_copy); i++)
    {
        if(str_copy[i] >= 'a' && str_copy[i] <= 'z')
        {
            str_copy[i] = str_copy[i] - 'a' + 'A';
        }
    }

    for(int i = 0; i < strlen(str_copy); i++)
    {
        if(curKey == strlen(key)) curKey = 0;

        if(str_copy[i] >= 'A' && str_copy[i] <= 'Z')
        {
            str_copy[i] = str_copy[i] - key[curKey];

            if(str_copy[i] < 0) 
                str_copy[i] += 26;
        }

        if(temp[i] >= 'a' && temp[i] <= 'z')
            str_copy[i] += 'a';
        else if(temp[i] >= 'A' && temp[i] <= 'Z')
            str_copy[i] += 'A';
        else
            curKey--;
        
        curKey++;
    }

    str_copy[strlen(str)] = 0;
    return str_copy;
}

void encryptRecursively(char *path, int CHIPER)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path)))
        return;

    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char from_rename[SIZE];
            sprintf(from_rename, "%s/%s", path, entry->d_name);
            
            char *encrypted;
            if(CHIPER == ROT13)
            {
                encrypted = chaesarEncrypt(entry->d_name, 13);
            }
            else if(CHIPER == VIGENERE)
            {
                encrypted = vignereEncrypt(entry->d_name, "SISOP");
            }

            char to_rename[SIZE]; sprintf(to_rename, "%s/%s", path, encrypted);

            printf("rename: %s -> %s\n", from_rename, to_rename);

            rename(from_rename, to_rename);

            char next_path[SIZE];
            sprintf(next_path, "%s/%s", path, encrypted);
            encryptRecursively(next_path, CHIPER);
        }
        else
        {
            char to_rename[SIZE];
            char *encrypted;
            char from_rename[SIZE];
            sprintf(from_rename, "%s/%s", path, entry->d_name);

            char *dir_ext;
			getAnything(entry->d_name, dir_ext, '.');
            if(strlen(dir_ext) > 0)
            {
                char to_decrypt[SIZE]; sprintf(to_decrypt, "%s", entry->d_name);
                to_decrypt[strlen(to_decrypt) - strlen(dir_ext) -1] = 0;

                if(CHIPER == ROT13)
                {
                    encrypted = chaesarEncrypt(to_decrypt, 13);
                }
                else if(CHIPER == VIGENERE)
                {
                    encrypted = vignereEncrypt(to_decrypt, "SISOP");
                }
                sprintf(to_rename, "%s/%s.%s", path, encrypted, dir_ext);
            }
            else
            {
                if(CHIPER == ROT13)
                {
                    encrypted = chaesarEncrypt(entry->d_name, 13);
                }
                else if(CHIPER == VIGENERE)
                {
                    encrypted = vignereEncrypt(entry->d_name, "SISOP");
                }
                sprintf(to_rename, "%s/%s", path, encrypted);
            }

            rename(from_rename, to_rename);
        }
    }
    closedir(dir);
}


void decryptRecursively(char *path, int CHIPER)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(path)))
        return;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            char from_rename[SIZE];
            sprintf(from_rename, "%s/%s", path, entry->d_name);
            
            char *decrypted;
            if(CHIPER == ROT13)
            {
                decrypted = chaesarEncrypt(entry->d_name, 13);
            }else if(CHIPER == VIGENERE)
            {
                decrypted = vignereDecrypt(entry->d_name, "SISOP");
            }

            char to_rename[SIZE];
            sprintf(to_rename, "%s/%s", path, decrypted);

			printf("rename: %s -> %s\n", from_rename, to_rename);

            rename(from_rename, to_rename);

            char next_path[SIZE];
            sprintf(next_path, "%s/%s", path, decrypted);
            decryptRecursively(next_path, CHIPER);
        }
        else
        {
            char from_rename[SIZE]; sprintf(from_rename, "%s/%s", path, entry->d_name);
            char to_rename[SIZE];
            char *decrypted;

            char *dir_ext;
			getAnything(entry->d_name, dir_ext, '.');

            if(strlen(dir_ext) > 0)
            {
                char to_decrypt[SIZE]; sprintf(to_decrypt, "%s", entry->d_name);
                to_decrypt[strlen(to_decrypt) - strlen(dir_ext) -1] = 0;

                if(CHIPER == ROT13)
                {
                    decrypted = chaesarEncrypt(to_decrypt, 13);
                }
                else if(CHIPER == VIGENERE)
                {
                    decrypted = vignereDecrypt(to_decrypt, "SISOP");
                }
                sprintf(to_rename, "%s/%s.%s", path, decrypted, dir_ext);
            }
            else
            {
                if(CHIPER == ROT13)
                {
                    decrypted = chaesarEncrypt(entry->d_name, 13);
                }
                else if(CHIPER == VIGENERE)
                {
                    decrypted = vignereEncrypt(entry->d_name, "SISOP");
                }
                sprintf(to_rename, "%s/%s", path, decrypted);
            }

            rename(from_rename, to_rename);
        }
    }
    closedir(dir);
}

// end of no 2

// start no 4

void logs(char* level, char *function, char *description){
    time (&my_time);
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

    char content[1024];
    sprintf(content, "%s::%s%s%s-%s:%s:%s:%s::%s", level, day, month, year, hour, minute, second, function, description);

    FILE* fptr_logs = fopen(logpath, "a");
    fprintf(fptr_logs, "%s\r\n", content);
    fclose(fptr_logs);
}

// end of no 4

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    logs("INFO", "GETATTR", path);

	int res;
	char fpath[1024];

	// bzero(path, sizeof(path));
	sprintf(fpath, "%s%s", dirPath, path);

	printf("getAttr: %s\n", fpath);

	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    logs("INFO", "READDIR", path);

	char fpath[1024];

	if(strcmp(path,"/") == 0)
	{
		path=dirPath;
		sprintf(fpath,"%s",path);
	}
	else
	{
		sprintf(fpath, "%s%s",dirPath,path);
	}

	int res = 0;

	DIR *dp;
	struct dirent *de;
	(void) offset;
	(void) fi;

	dp = opendir(fpath);

	if (dp == NULL) return -errno;

	while ((de = readdir(dp)) != NULL)
	{
		struct stat st;

		memset(&st, 0, sizeof(st));

		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		res = (filler(buf, de->d_name, &st, 0));

		if(res!=0) break;
	}

	closedir(dp);

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    logs("INFO", "READ", path);

	char fpath[1024];

	if(strcmp(path,"/") == 0)
	{
		path=dirPath;
		sprintf(fpath,"%s",path);
	}
	else
	{
		sprintf(fpath, "%s%s",dirPath,path);
	}

	int res = 0;
	int fd = 0 ;

	(void) fi;

	fd = open(fpath, O_RDONLY);
	if (fd == -1) return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1) res = -errno;

	close(fd);

	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    logs("INFO", "MKDIR", path);

	int res;
	char fileName[1024];
	char current[1024];
	char fPath[1024];
	char save[1024];

	sprintf(current, "%s", path);
	printf("\nmkdir\n");
	printf("%s\n\n", path);

	getAnything(current, fileName, '/');
	
	if(strstr(current, "AtoZ_"))
	{
		cipherCode(fileName);
		sprintf(fPath, "%s%s%s", dirPath, current, fileName);
		printf("%s\n", fPath);
	}
	else
	{
		sprintf(fPath, "%s%s", dirPath, path);
	}

    char path_copy[SIZE]; sprintf(path_copy, "%s", path);
    char *end_str;
    char *dirname = strrev(strtok_r(strrev(path_copy), "/", &end_str));

	char rx_dir[SIZE]; sprintf(rx_dir, "%s", dirname);
    rx_dir[strlen("RX_")] = 0;

	
	if(!strcmp(rx_dir, "RX_"))
    {
        sprintf(fPath, "%s%s", dirPath, path);
        rx_insert(fPath, ROT13);
	}

    char full_dir[SIZE];
    sprintf(full_dir, "%s%s", dirPath, path);

    if(rx_insubstr(full_dir) >= 0 && !!strcmp(rx_dir, "RX_"))
    {
        int THIS_CHYPER = rx_directory[rx_insubstr(full_dir)].CHIPER;
        char *encrypted;

        if(THIS_CHYPER == ROT13)
        {
            encrypted = chaesarEncrypt(dirname, 13);
			printf("encrypted: %s", encrypted);
        }
        else if(THIS_CHYPER == VIGENERE)
        {
            encrypted = vignereEncrypt(dirname, "SISOP");
        }

        char *before_current = getStrBetween(full_dir, dirPath, dirname);
        sprintf(fPath, "%s%s%s", dirPath, before_current, encrypted);
    }
	printf("mkdir RX_ : %s\n", fPath);

	FILE *log = fopen("log.txt", "a");
	fprintf(log, "mkdir: %s\n", fPath);
	fclose(log);

	res = mkdir(fPath, mode);

	if(res == -1)
		return -errno;
	
	return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    char tempLog[1024];
    sprintf(tempLog, "%s::%s", from, to);
    logs("INFO", "RENAME", tempLog);
    
	int res;
	char old[1024];
	char new[1024];

	sprintf(old, "%s%s", dirPath, from);
	sprintf(new, "%s%s", dirPath, to);

	printf("\nrename\n");
	printf("From: %s\n", from);
	printf("To: %s\n\n", to);

	if(strstr(to, "AtoZ_") && !strstr(from, "AtoZ_"))
	{
		FILE *log = fopen("log.txt", "a");
		fprintf(log, "rename: %s -> %s\n", old, new);
		fclose(log);

		renamingInnerFile(old);
		res = rename(old, new);
		return 0;
	}
	else if(strstr(from, "AtoZ_") && !strstr(to, "AtoZ_"))
	{
		FILE *log = fopen("log.txt", "a");
		fprintf(log, "rename: %s -> %s\n", old, new);
		fclose(log);

        renamingInnerFile(old);
		res = rename(old, new);
		return 0;
	}
	
	char from_copy[SIZE];
    sprintf(from_copy, "%s", from);
    char *end_str;
    char *dir_from = strrev(strtok_r(strrev(from_copy), "/", &end_str));
    char from_fulldir[SIZE];
    sprintf(from_fulldir, "%s%s", dirPath, from);

    char to_copy[SIZE];
    sprintf(to_copy, "%s", to);
    char *end_str2;
    char *dir_to = strrev(strtok_r(strrev(to_copy), "/", &end_str2));
    char to_fulldir[SIZE];
    sprintf(to_fulldir, "%s%s", dirPath, to);

	char from_realpath[SIZE];
    char to_realpath[SIZE];
    sprintf(from_realpath, "%s%s", dirPath, from);
    sprintf(to_realpath, "%s%s", dirPath, to);

	if(strstr(to, "/RX_"))
    {
        printf("0\n");
        int last_rx = rx_insubstr(to_realpath);
        rx_insert(to_realpath, VIGENERE);

        rename(from_realpath, to_realpath);

        if(last_rx >= 0)
        {
            decryptRecursively(to_realpath, rx_directory[last_rx].CHIPER);
        }

        encryptRecursively(to_realpath, VIGENERE);
        return 0;
	}

	rename(from_realpath, to_realpath);

	char rx_from[SIZE];
    sprintf(rx_from, "%s", dir_from);
    rx_from[strlen("RX_")] = 0;

    char rx_to[SIZE];
    sprintf(rx_to, "%s", dir_to);
    rx_to[strlen("RX_")] = 0;

    printf("from_realpath: %s\n", from_realpath);
    printf("to_realpath: %s\n", to_realpath);
    printf("rx_from: %s\n", rx_from);
    printf("dir_to: %s\n", dir_to);

    if(!strcmp(rx_from, "RX_") && !!strcmp(rx_to, "RX_"))
    {
        printf("1\n");
        int last_rx = rx_insubstr(from_realpath);
        if(last_rx >= 0)
        {
            printf("2\n");
            decryptRecursively(to_realpath, rx_directory[last_rx].CHIPER);
            strcpy(rx_directory[last_rx].DIR, "xxx..............xxx");
        }
    }

	FILE *log = fopen("log.txt", "a");
	fprintf(log, "rename: %s -> %s\n", from_realpath, to_realpath);
	fclose(log);

	res = rename(from_realpath, to_realpath);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	logs("WARNING", "UNLINK", path);

	char fPath[1024];
	sprintf(fPath, "%s%s", dirPath, path);

	printf("\nunlink: %s", fPath);

	int res;

	res = unlink(fPath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_rmdir(const char *path)
{
	logs("WARNING", "REMOVE", path);

	char fPath[1024];
	sprintf(fPath, "%s%s", dirPath, path);
	printf("\nremove: %s\n\n", fPath);

	int res;

	res = rmdir(fPath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi)
{

    (void) fi;

	char fPath[1024];
	sprintf(fPath, "%s%s", dirPath, path);

    int res;
    res = creat(fPath, mode);
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static struct fuse_operations xmp_oper =
{
	.getattr 	= xmp_getattr,
	.readdir 	= xmp_readdir,
	.read 		= xmp_read,
	.mkdir 		= xmp_mkdir,
	.rename		= xmp_rename,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.create		= xmp_create,
};

int  main(int  argc, char *argv[])
{
	umask(0);

	if(access("log.txt", F_OK))
	{
		FILE *fp = fopen("log.txt", "w+");
		fclose(fp);
	}

	return fuse_main(argc, argv, &xmp_oper, NULL);
}
