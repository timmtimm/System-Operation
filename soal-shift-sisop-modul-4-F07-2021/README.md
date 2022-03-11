# soal-shift-sisop-modul-4-F07-2021
Nama Kelompok F07:
1. Timotius Wirawan 05111940000161
2. Hana Machmudah 05111940000072
3. Rosa Amalia 05111940000106

# Soal No. 1
Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :

`a.` Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

`b.` Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.

`c.` Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.

`d.` Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]

`e.` Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

# Jawaban No. 1
## A
Pada fungsi `xmp_mkdir`, akan dicek apakah suatu folder memilik 'AtoZ_' di file pathnya. Fungsi `getAnything` untuk mendapatkan nama folder dari filePath. Pada fungsi `xmp_mkdir`, terdapat kode sebagai berikut.
```C
..........

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
	
..........
```
Kemudian nama folder di-encode dengan kode cipher melalui fungsi berikut.
```C
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
```

## B
Pada fungsi `xmp_rename`, akan dicek apakah sebuah folder diubah namannya ke dengan awalan AtoZ_ atau tidak. Pada fungsi `xmp_rename`, terdapat kode sebagai berikut.
```C
..........

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
	
..........
```

Untuk mengganti tiap nama file dan folder di dalam folder berawalan AtoZ_, dengan menggunakan fungsi `renamingInnerFolder`. Pada fungsi ini, berlaku rekursif untuk mengganti semua file di dalam direktori.

## C
Pada fungsi `xmp_rename`, akan dicek apakah nama folder diubah menjadi tanpa awalan AtoZ_. Kemudian, nama file dalam direktori akan diubah dengan alur yang sama dengan penjelasan nomor B
```C
..........

	else if(strstr(from, "AtoZ_") && !strstr(to, "AtoZ_"))
	{
		FILE *log = fopen("log.txt", "a");
		fprintf(log, "rename: %s -> %s\n", old, new);
		fclose(log);

        	renamingInnerFile(old);
		res = rename(old, new);
		return 0;
	}
	
..........
```
## D
Pada fungsi `xmp_mkdir`, akan dipanggil fungsi `logs` untuk menuliskan log. Fungsi dijelaskan pada nomor 4.
```C
	logs("INFO", "MKDIR", path);
```
Dan fungsi rename.
```C
	logs("INFO", "RENAME", tempLog);
```

## E
Fungsi `renamingInnerFile` akan melakukan rekursif dalam mengganti nama tiap file dan folder
```C
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
```

# Soal No. 2
Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang dirancang oleh Sei
`a.` Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).

`b.` Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).

`c.` Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.

`d.` Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).

`e.` Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:

```
Suatu_File.txt.0000
Suatu_File.txt.0001
Suatu_File.txt.0002
```

Ketika diakses melalui filesystem hanya akan muncul Suatu_File.txt

# Jawaban No. 2
## A
Ketika melakukan perintah membuat direktori, maka akan melakukan checking apakah diawali dengan "RX_" atau tidak pada fungsi `xmp_mkdir()`
```C
static int xmp_mkdir(const char *path, mode_t mode)
{
    logs("INFO", "MKDIR", path);
....
// Membuat directory
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
    
// Cek apakah dir yang dibuat didalam folder RX
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
```

Kemudian jika terdapat `"RX_"` pada pathnya maka akan memanggil fungsi `rx_insert()`. Fungsi tersebut akan menyimpan directory dan jenis enkripsi yang akan digunakan pada folder tersebut.
```C
void rx_insert(char* dir, int chiper)
{
    sprintf(rx_directory[rx_last_idx].DIR, "%s", dir);
    rx_directory[rx_last_idx].CHIPER = chiper;
    rx_last_idx++;
}
```

Kemudian apakah direktori yang dibuat ada di dalam folder berawalan "RX_" atau tidak, dengan menggunakan fungsi `rx_insubstr()`. Apabila direktori tersebut berada di dalam direktori berawalan "RX_", maka direktori tersebut akan dienkripsi sesuai dengan jenis enkripsinya masing-masing.
```C
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
```

Fungsi `Chaesar Cipher`
```C
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
```
Fungsi `Vignere Chip`
```C
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
```
## B dan C
Ketika melakukan perintah merename direktori, maka akan melakukan checking apakah diawali dengan "RX_" atau tidak pada fungsi xmp_rename().
Apabila diawali dengan "RX_" maka direktori tersebut akan merename direktorinya dan mendecrypt secara rekursif semua direktori yang ada di dalamnya terlebih dahulu, kemudian dienkripsi ulang dengan Vignere Cipher.
```C
static int xmp_rename(const char *from, const char *to)
{
    char tempLog[1024];
    sprintf(tempLog, "%s::%s", from, to);
    logs("INFO", "RENAME", tempLog);
....    
    sprintf(old, "%s%s", dirPath, from);
	sprintf(new, "%s%s", dirPath, to);

	printf("\nrename\n");
	printf("From: %s\n", from);
	printf("To: %s\n\n", to);
    
....
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
.....
 // Secara rekursif ganti semua isi foldernya
        rename(from_realpath, to_realpath);

        if(last_rx >= 0)
        {
            decryptRecursively(to_realpath, rx_directory[last_rx].CHIPER);
        }
// kemudian dienkripsi ulang dengan Vignere Cipher
        encryptRecursively(to_realpath, VIGENERE);
        return 0;
	}

	rename(from_realpath, to_realpath);

	char rx_from[SIZE];
    sprintf(rx_from, "%s", dir_from);
    rx_from[strlen("RX_")] = 0;
    
 // Cek apakah direktori diganti dari /RX_ ke dir normal
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

	....
}
```

Kemudian mengecek apakah merename dari direktori berawalan "RX_" menjadi normal atau tidak. Jika ya, maka decrypt direktori tersebut dan seluruh isinya secara rekursif.
```C
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
```

## D
Melakukan logging, di `mkdir` dan `rename` dengan memanggil fungsi `logs()` dengan method `mkdir` atau `rename`.
```C
logs("INFO", "MKDIR", path);
logs("INFO", "RENAME", tempLog);
```


# Soal No. 3
Karena Sin masih super duper gabut akhirnya dia menambahkan sebuah fitur lagi pada filesystem mereka. 

`a.` Jika sebuah direktori dibuat dengan awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

`b.` Jika sebuah direktori di-rename dengan memberi awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.

`c.` Apabila direktori yang terenkripsi di-rename dengan menghapus “A_is_a_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.

`d.` Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ_” maupun “RX_” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive  “AtoZ_” dan “RX_” tetap berjalan pada subdirektori).

`e.` Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase insensitive dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.

Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.


# Soal No. 4
Untuk memudahkan dalam memonitor kegiatan pada filesystem mereka Sin dan Sei membuat sebuah log system dengan spesifikasi sebagai berikut.

`a.` Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.

`b.` Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.

`c.` Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.

`d.` Sisanya, akan dicatat pada level INFO.

`e.` Format untuk logging yaitu:

```
[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]

Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan

INFO::28052021-10:00:00:CREATE::/test.txt
INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt
```
# Jawaban No. 4
## A
Fungsi yang akan digunakan untuk melakukan logging adalah fungsi 'logFile()'
```C
static const char *logpath = "/home/timtim/SinSeiFS.log";
```
Yang pertama, melakukan pendefisinian path dari file log yang akan diproses oleh fungsi `logFile()`

## B dan E
```C
void logs(char* level, char *function, char *description){
    time (&my_time);
	timeinfo = localtime(&my_time);
	
	;
	;
}

```
Dalam fungsi ini, `my_time` akan diubah menjadi struct `tm` menggunakan fungsi localtime() kedalam variable `timeinfo`. 


```C
void logs(char* level, char *function, char *description){
  	;
	;
	
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
	
	;
	;
}
```
Kemudian mengidentifikasi `day` `month` `year` `hour` `minute` `second` untuk penamaan nama logging


```C
void logs(char* level, char *function, char *description){
  	;
	
 char content[1024];
    sprintf(content, "%s::%s%s%s-%s:%s:%s:%s::%s", level, day, month, year, hour, minute, second, function, description);
    
    	;
}
```
melakuakan print nama logging dengan `level` yaitu `INFO/WARNING`, waktu, `function` untuk mendefinisikan command system-call yang terpanggil. `descrption` untuk mendefinisikan result dari system-call yang terpanggil.


```C
void logs(char* level, char *function, char *description){
  	;
	
    FILE* fptr_logs = fopen(logpath, "a");
    fprintf(fptr_logs, "%s\r\n", content);
    fclose(fptr_logs);
}
```
Kemudian, membuka dan membuat (jika belum ada) file log pada logpath menggunakan `fopen()` kedalam pointer `FILE *fptr_logs`.
Melakukan print ke dalaam file `fptr_logs` menggunakan fungsi fprintf() sesuai format sebelumnya yang te;ah tersimpan dalam `content`.
Setelah log selesai, `fptr_logs` akan di fclose(). Untuk seluruh system-call akan memanggil fungsi `logFile()` tersebut untuk melakukan logging.

## C dan D
Level `WARNING` untuk syscall `rmdir` dan `unlink`
```C
 logs("WARNING", "UNLINK", path);
 logs("WARNING", "REMOVE", path);
```

Level `INFO` selain syscall `rmdir` dan `unlink`
```C
 logs("INFO", "GETATTR", path);
 logs("INFO", "READDIR", path);
 logs("INFO", "READ", path);
 logs("INFO", "MKDIR", path);
 logs("INFO", "MKDIR", path);
```

![image](https://user-images.githubusercontent.com/66562311/121806165-f72d1200-cc78-11eb-8819-f5758966ad91.png)

