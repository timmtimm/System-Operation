# soal-shift-sisop-modul-3-F07-2021
Kelompok F07
1. Timotius Wirawan 05111940000161
2. Hana Machmudah 05111940000072
3. Rosa Amalia 05111940000106

# Soal No.1
Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini. 

Di dalam proyek itu, Keverk diminta: 

`a.` Pada saat client tersambung dengan server, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama akun.txt dengan format :

**akun.txt**
```
id:password
id2:password2
```

`b.` Sistem memiliki sebuah database yang bernama **files.tsv**. Isi dari **files.tsv** ini adalah **path file saat berada di server**, **publisher**, dan **tahun publikasi**. Setiap penambahan dan penghapusan file pada folder file yang bernama **FILES** pada server akan memengaruhi isi dari **files.tsv**. Folder **FILES** otomatis dibuat saat server dijalankan. 

`c.` Tidak hanya itu, Keverk juga diminta membuat fitur agar client dapat menambah file baru ke dalam server. Direktori **FILES** memiliki struktur direktori di bawah ini :

Direktori **FILES**
```
File1.ekstensi
File2.ekstensi
```

Pertama client mengirimkan input ke server dengan struktur sebagai berikut :
Contoh Command Client :
```
add
```

Output Client Console:
```
Publisher:
Tahun Publikasi:
Filepath:
```

Kemudian, dari aplikasi client akan dimasukan data buku tersebut (perlu diingat bahwa Filepath ini merupakan **path file yang akan dikirim ke server**). Lalu client nanti akan melakukan pengiriman file ke aplikasi server dengan menggunakan socket. Ketika file diterima di server, maka row dari files.tsv akan bertambah sesuai dengan data terbaru yang ditambahkan.

`d.` Dan client dapat mendownload file yang telah ada dalam folder FILES di server, sehingga sistem harus dapat mengirim file ke client. Server **harus** melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di **folder client** tersebut. 

Contoh Command client
```
download TEMPfile.pdf
```

`e.` Setelah itu, client juga dapat menghapus file yang tersimpan di server. Akan tetapi, Keverk takut file yang dibuang adalah file yang penting, maka file hanya akan diganti namanya menjadi ‘old-NamaFile.ekstensi’. Ketika file telah diubah namanya, maka row dari file tersebut di file.tsv akan terhapus.

Contoh Command Client:
```
delete TEMPfile.pdf
```

`f.` Client dapat melihat semua isi files.tsv dengan memanggil suatu perintah yang bernama see. Output dari perintah tersebut keluar dengan format. 

Contoh Command Client :
```
see
```

Contoh Format Output pada Client:
```
Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 
```

`g.` Aplikasi client juga dapat melakukan pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. Format output seperti format output f.

Contoh Client Command:
```
find TEMP
```

`h.` Dikarenakan Keverk waspada dengan pertambahan dan penghapusan file di server, maka Keverk membuat suatu log untuk server yang bernama **running.log**. Contoh isi dari log ini adalah

**running.log**
```
Tambah : File1.ektensi (id:pass)
Hapus : File2.ektensi (id:pass)
```

Akan tetapi, kevek saat itu masih belum paham dengan sistem server dan client. Bantulah Keverk untuk menyelesaikan proyek ini!

**Note:**
- Dilarang menggunakan system() dan execv(). Silahkan dikerjakan sepenuhnya dengan thread dan socket programming. 
- Untuk download dan upload silahkan menggunakan file teks dengan ekstensi dan isi bebas (yang ada isinya bukan touch saja dan tidak kosong) dan requirement untuk benar adalah percobaan dengan minimum 5 data.

Struktur Direktori
```
├── Client
│   ├── client.c
│   ├── File2.extensi
│   └── File1.extensi
└── Server
    ├── akun.txt
    ├── files.tsv
    ├── server.c
    ├── running.log
    └── FILES
            ├── File2.extensi
            └── File1.ekstensi
```

# Jawaban No. 1
## Penjelasan A
User akan memberikan input command Register atau Login untuk masuk ke server melalui client.c
```C
..........

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
..........

```

File client.c akan menerima input username dan password dari user melalui fungsi `registerLogin`
```C
.........

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

..........
```

Pada server.c akan mengecek sesuai command yang dikirim oleh client.c.
Jika command berupa `register`, program akan mengecek apakah username yang dibuat sudah ada atau belum di file `akun.txt`
```C
.........

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

..........
```

Jika command `login`, program akan mengecek apakah username dan password cocok pada file `akun.txt`
```C
..........
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
    
.........
```

Untuk membuat program dapat menerima multi-connections, menggunakan program di bawah ini pada server.c.
```C
..........

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
		   
..........
```
Jika pada server terdapat client yang belum selesai, pada client.c akan memberikan pesan `Server is full!`

```C
..........

ret_val = recv(fd, message, SIZE_BUF, 0);
    while(strcmp(message, "wait") == 0) {
        printf("Server is full!\n");
        ret_val = recv(fd, message, SIZE_BUF, 0);
    }
 
.........
```

## Penjelasan B
File `files.tsv` dibuat di server.c dengan kode program sebagai berikut.
```C
..........

    if(access("files.tsv", F_OK ) != 0 )
    {
	FILE *fp = fopen("files.tsv", "w+");
	fclose(fp);
    }
    
..........
```
## Penjelasan C
Command-command yang dapat digunakan oleh user pada client.c adalah sebagai berikut.
```C
.........

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
	
..........
```

Pada client.c akan menerima input dari user di antaranya Publisher, Tahun Publikasi dan Filepath dari file tersebut di fungsi `addBooks` dan mengirimnya ke server
```C
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
```

File dikirim dengan fungsi `sendFile`
```C
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
```

Data-data dari buku yang diterima oleh server kemudian ditulis ke dalam file `files.tsv`
```C
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
```

Untuk mendapatkan nama file dari filepath yang telah dikirim, membutuhkan fungsi `getFileName` dengan kode sebagai berikut.
```C
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
```

Dan karena nama file yang didapatkan dalam keadaan terbalik, membutuhkan fungsi `strrev` sebagai berikut.
```C
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
```

## Penjelasan D
Untuk mendownload file dari server, client.c akan menerima input berupa nama file dari user
```C
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
```

Pada server, nama file yang diterima dari client dicek, apakah ada di `files.tsv` atau tidak
```C
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
```
File dicek dengan menggunakan fungsi `checkPath`
```C
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
```

Jika data file ditemukan, server akan mengirimkan pesan `Begin to download` ke client, dan client akan mulai mendownload file dengn fungsi `writeFile`
```C
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
```

## Penjelasan E
Untuk menghapus file di server, client akan menerima input buku apa yang mau dihapus
```C
void delete_books(int fd)
{
    int ret_val;
    char books[SIZE_BUF], message[SIZE_BUF];

    printf("Book: "); scanf("%s", books);
    ret_val = send(fd, books, SIZE_BUF, 0);

    ret_val = recv(fd, message, SIZE_BUF, 0);
    puts(message);

}
```

Setelah diterima oleh server, server akan mengganti nama file di folder FILES dengan menambahkan 'old-' di depan nama file sebelumnya
```C
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
```

Kemudian, data file di file `files.tsv` akan dihapus dengan fungsi `deleteTsv`
```C
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
```

## Penjelasan F
Client akan mengirimkan command `see` ke server. Kemudian server akan mengirimkan kembali data pada `files.tsv` satu per satu ke client
```C
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
```

Data yang dikirimkan kemudian diterima oleh client, dan client menampilkannya kepada user
```C
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
```

## Penjelasan G
Client akan menerima file yang mau dicari. Kemudian file tersebut dikirim ke server untuk dicari data-datanya
```C
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
```

Pada server, setelah menerima file yang akan dicari, program akan membandingkan di data mana file tersebut cocok. Jika sudah menemukan, program akan mengirimkan data-data dari `files.tsv` ke client untuk ditampilkan ke user
```C
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
```

## Penjelasan H
Untuk membuat `running.log`, setiap penambahan dan penghapusan file akan ditulis. Untuk menulisnya membutuhkan fungsi `makeLog` sebagai berikut.
```C
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
```
Fungsi ini akan dipanggil saat terjadi penambahan data atau penghapusan data.

# Soal No. 2
Crypto (kamu) adalah teman Loba. Suatu pagi, Crypto melihat Loba yang sedang kewalahan mengerjakan tugas dari bosnya. Karena Crypto adalah orang yang sangat menyukai tantangan, dia ingin membantu Loba mengerjakan tugasnya. Detil dari tugas tersebut adalah:

`a.` Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

`b.` Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (**Catatan!**: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang a	da. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya matri(dari paling besar ke paling kecil) (**Catatan!**: gunakan thread untuk perhitungan di setiap cel). 

**Ketentuan**
```
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0
```

Contoh :
|  A  |  B  | Angka-Angka Faktorial |
|:---:|:---:|          :---:        |
| 4 | 4 | 4 3 2 1 |
| 4 | 5 | 4 3 2 1 |
| 4 | 3 | 4 3 2 |
| 4 | 0 | 0 |
| 0 | 4 | 0 |
| 4 | 6 | 4 3 2 1 |

Contoh:
| Matriks A | Matriks B | Matriks Hasil |
|:---------:|:---------:|:-------------:|
| 0_4 | 0_4 | 0 _ 4* 3* 2*1|
| 4_5| 6_2 | 4* 3* 2* 1 _ 5* 4|
| 5_6| 6_0 | 5* 4* 3* 2* 1 _ 0 |

`c.` Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5” (**Catatan!**: Harus menggunakan IPC Pipes)

**Note:**
- Semua matriks berasal dari input ke program.
- Dilarang menggunakan system()

# Jawaban No. 2
### **A. Membuat program perkalian matriks (4x3) dan (3x6) lalu hasilnya dioutputkan pada program A dan B.**
```C
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>

long long firstMatrix[10][10], secondMatrix[10][10];

void enterData()
{
	int i, j;
	printf("Enter elements of matrix 1 (4x3)\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 3; ++j)
		{
			scanf("%lld", &firstMatrix[i][j]);
		}
	}

	printf("\nEnter elements of matrix 2 (3x6)\n");
	for(i = 0; i < 3; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			scanf("%lld", &secondMatrix[i][j]);
		}
	}
}


void multiplyMatrix(long long multy[4][6])
{
	int i, j, k;

	for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{
			multy[i][j] = 0;
		}
	}

	for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{
			for(k=0; k<3; k++)
			{
				multy[i][j] += firstMatrix[i][k] * secondMatrix[k][j];
			}
		}
	}
}

void displayMatrix(long long multy[][6])
{
	int i, j;
	printf("\nOutput Matrix\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			printf("%lld ", multy[i][j]);
			if(j == 5)
				printf("\n\n");
		}
	}
}

int main() 
{
    key_t key = 1234;
    void* memory;
    int shmid = shmget(key, 512, IPC_CREAT | 0666);
	memory = shmat(shmid, NULL, 0);
    long long (*multy)[6] = memory;
	
    enterData();
    multiplyMatrix(multy);
    displayMatrix(multy);
	return 0;
}
```

#### Penjelasan A
Fungsi:
- enterData digunakan untuk menginput matriks ke array
- multiplyMatrix digunakan untuk mengalikan matriks
- displayMatrix digunakan untuk mengoutputkan hasil perkalian matriks

Pada fungsi main di bagian pertama digunakan untuk shared memory array lalu bagian kedua dijalankan ketiga fungsi diatas.

### **B. Menggunakan output dari program A dan dibandingkan dengan input baru untuk menghasilkan matriks baru sesuai kodnisi yang telah ditentukan**
```C
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

long long thirdMatrix[10][10], mult2[10][10];

struct result
{
    long long mat1;
    long long mat2;
    long long *lastRes;
};

void enterData()
{
	int i, j;
	printf("\nEnter elements of matrix 3:\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			scanf("%lld", &thirdMatrix[i][j]);
		}
	}
}

long long factorial(long long n)
{
    long long i, fact = 1;
    if (n < 0)
        printf("Error! Factorial of a negative number doesn't exist.\n");
    else {
        for (i = 1; i <= n; ++i)
        {
            fact *= i;
        }
    }
    return fact;
}

long long midCase(long long a, long long b)
{
    long long hasil=1;
    long long start=a-b+1;
    
	if(a == b)
	{
		return factorial(a);
	}

	if(a==b-1)
	{
		return a;
	}

    for(long long i=start; i<=a; i++)
    {
        hasil*=i;
    }
    printf("hasil dari %lld dan %lld: %lld\n",a,b, hasil);

    return hasil;
}

void *function(void *temp)
{
    printf("FUNGSI TERPANGGIL\n");
    struct result *res = temp;
    printf("mat1 : %lld\n", res->mat1);

    if(res->mat1 == 0 || res->mat2 == 0)
    {
        *res->lastRes = 0;
    }
    else if(res->mat1 >= res->mat2)
    {
        *res->lastRes = midCase(res->mat1, res->mat2);
    }
    else if(res->mat1 < res->mat2)
    {
        *res->lastRes = factorial(res->mat1);
    }

    free(res);
    pthread_exit(NULL);
    return NULL;
}

void displayMatrix()
{
	int i, j;
	printf("\nOutput Matrix\n");
	for(i = 0; i < 4; ++i)
	{
		for(j = 0; j < 6; ++j)
		{
			printf("%lld ", mult2[i][j]);
			if(j == 5)
				printf("\n\n");
		}
	}
}

int main()
{
    key_t key = 1234;
    int shmid = shmget(key, 512, IPC_CREAT | 0666);
    void* memory;
    memory = shmat(shmid, NULL, 0);
    long long (*mult)[6] = memory;

    pthread_t tid[25][25];
    int i, j;
    struct result *res;

    printf("\nOutput Matrix:\n");
    for(i = 0; i < 4; ++i)
    {
        for(j = 0; j < 6; ++j)
        {
            printf("%lld ", mult[i][j]);
        }
        printf("\n");
    }

    enterData();

    for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{		
            res = (struct result *)malloc(sizeof(struct result));
            res->mat1 = mult[i][j];
            res->mat2 = thirdMatrix[i][j];
            res->lastRes = &mult2[i][j];

            pthread_create(&tid[i][j], NULL, &function, (void*) res);
            
		}
	}

    for(i=0; i<4; i++)
	{
		for(j=0; j<6; j++)
		{		
            pthread_join(tid[i][j], NULL);
		}
	}

    displayMatrix();
}
```

#### Penjelasan B
Fungsi:
- Struct digunakan untuk menampung matriks beserta hasilnya karena dari fungsi threadnya hanya bisa menggunakan satu variabel saja
- enterData digunakan untuk menginput matriks ke array
- factorial digunakan untuk mengfactorialkan angka yang diinput
- MidCase untuk perhitungan pada kondisi tengah (a>=b)
- function untuk logika pada kondisi yang telah ditentukan
- displayMatrix digunakan untuk mengoutputkan hasil perkalian matriks

Pada fungsi main dioutputkan terlebih dahulu untuk shared memory dari program A, lalu menginputkan matriks yang akan diproses nanti dari matriks output program A dan input tadi akan dimasukkan ke Struct. Setelahnya perhitungan dilakukan menggunakan thread lalu hasilnya akan ditampilkan.

### **C. Melakukan pipeing 3 fungsi**
```C
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pid;
    int pipeing[2][2];
    
    if(pipe(pipeing[0]) == -1)
    {
        perror("failed pipe");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) == -1)
    {
        perror("failed fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        dup2(pipeing[0][1], 1);
        close(pipeing[0][0]);
        close(pipeing[0][1]);
        execlp("ps", "ps", "-aux", NULL);
        exit(EXIT_SUCCESS);
    }

    if(pipe(pipeing[1]) == -1)
    {
        perror("failed pipe");
        exit(EXIT_FAILURE);
    }

    if((pid = fork()) == -1)
    {
        perror("failed fork");
        exit(EXIT_FAILURE);
    }else if(pid == 0)
    {
        dup2(pipeing[0][0], 0);
        dup2(pipeing[1][1], 1);
        close(pipeing[0][0]);
        close(pipeing[0][1]);
        close(pipeing[1][0]);
        close(pipeing[1][1]);
        execlp("sort", "sort", "-nrk", "3,3", NULL);
        perror("failed sort");
        exit(EXIT_SUCCESS);
    }

    close(pipeing[0][0]);
    close(pipeing[0][1]);

    if((pid = fork()) == -1)
    {
        perror("failed fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        dup2(pipeing[1][0], 0);
        close(pipeing[1][0]);
        close(pipeing[1][1]);
        execlp("head", "head", "-5", NULL);
        perror("failed head");
        exit(EXIT_SUCCESS);
    }
}
```
#### Penjelasan C
Sesuai soal disini dilakukan 3 pipe untuk tiap fungsi yaitu ps, sort, dan head.

# Soal No. 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

Contoh apabila program dijalankan:
```
# Program soal3 terletak di /home/izone/soal3
$ ./soal3 -f path/to/file1.jpg path/to/file2.c path/to/file3.zip
#Hasilnya adalah sebagai berikut
/home/izone
|-jpg
        |--file1.jpg
    |-c
        |--file2.c
    |-zip
        |--file3.zip
```

`a.` Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 

Output yang dikeluarkan adalah seperti ini :
```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```

`b.` Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. 

Contohnya adalah seperti ini:
```
$ ./soal3 -d /path/to/directory/
```

Perintah di atas akan mengkategorikan file di /path/to/directory, lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory).
Output yang dikeluarkan adalah seperti ini :
```
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```

`c.` Selain menerima opsi-opsi di atas, program ini menerima opsi *, contohnya ada di bawah ini:
```
$ ./soal3 \*
```
Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut

`d.` Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.

`e.` Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.

Namun karena Ayub adalah orang yang hanya bisa memberi ide saja, tidak mau bantuin buat bikin programnya, Ayub meminta bantuanmu untuk membuatkan programnya. Bantulah agar program dapat berjalan!

**Note:**
- Kategori folder tidak dibuat secara manual, harus melalui program C
- Program ini tidak case sensitive. Contoh: JPG dan jpg adalah sama
- Jika ekstensi lebih dari satu (contoh “.tar.gz”) maka akan masuk ke folder dengan titik terdepan (contoh “tar.gz”)
- Dilarang juga menggunakan fork-exec dan system()
- Bagian b dan c berlaku **rekursif**

# Jawaban No. 3
```C
char *getExtention(char *filename){
    char *dot = strchr(filename, '.');
    if (!dot || dot == filename)  
	return "";
    return dot;
}
```
membuat fungsi `getExtention` untuk mendapatkan nama extention dari suatu file yang akan dikategorikan

### **A. Mengkategorikan file sesuai argumen yang diinputkan dengan opsi -f**
```C
    case F:
        for (i = 2; i < argc; i++) {
            char *arr = argv[i];
            iret[i - 2] = pthread_create(&threads[i - 2], NULL, fungsiF, (void *)arr);
            if (iret[i - 2]){
                fprintf(stderr, "Error - pthread_create() return code: %d\n", iret[i - 2]);
                exit(EXIT_FAILURE);
            }
            pthread_join(threads[i - 2], NULL);
        }
break;
```
pada argumen `-f` dilakukan pengkategorian setiap file yang diinputkan dengan membuat sebuah thread pada setiap file, kemudian masuk ke fungsi `fungsiF` 
```C
void *fungsiF(void *ptr){
    int flag = 0;
    char *param = (char *)ptr;
    char *extension_temp = getExtention(param);
    if (strchr(extension_temp, ' ')){
        extension_temp = strrchr(extension_temp, ' ');
        extension_temp = strchr(extension_temp, '.');
    }
    if (strchr(extension_temp, '_')){
        extension_temp = strrchr(extension_temp, '_');
        extension_temp = strchr(extension_temp, '.');
    }
    if (strchr(extension_temp, '-')){
        extension_temp = strrchr(extension_temp, '-');
        extension_temp = strchr(extension_temp, '.');
    }
    int ch, i, length = strlen(extension_temp);

    char extension[100];
    char *filename = basename(param);
    char new_path[1000], temp[1000];
    if (filename[0] == '.'){
        if (!cekDirectory("Hidden"))
            mkdir("Hidden", 0777);
        strcpy(new_path, "Hidden/");
        strcat(new_path, filename);
        moveFile(param, new_path);

        if (flag == 0){
            printf("File %d: Berhasil Dikategorikan\n", countfile);
            countfile++;
        }
    }
    else {
        memset(new_path, 0, sizeof(new_path));
        memset(extension, 0, sizeof(extension));
        for (i = 1; i < length; i++){
            extension[i - 1] = tolower(extension_temp[i]);
        }

        if (!strlen(extension_temp)){
            if (!cekDirectory("Unknown"))
                mkdir("Unknown", 0777);
            strcpy(new_path, "Unknown/");
            strcat(new_path, filename);
        }
        else{
            if (!cekDirectory(extension))
                mkdir(extension, 0777);
            strcpy(new_path, extension);
            strcat(new_path, "/");
            strcat(new_path, filename);
        }
        moveFile(param, new_path);
        if (flag == 0){
            printf("File %d: Berhasil Dikategorikan\n", countfile);
            countfile++;
        }
        else if (flag == 1) {
            printf("File %d: Sad,gagal :(\n", countfile);
            countfile++;
        }
    }
}
```
Dalam `fungsiF` didapatkan extention file terlebih dahulu, kemudian dibuatkan suatu direktori berdasarkan kategori file yang berada dalam direktori yang sama dengan program yang dijalankan. Jika program berhasil mengkategorikan file maka akan mengeluarkan output `File ..: Berhasil Dikategorikan` dan jika gagal maka akan muncul `File ..: Sad,gagal`. Kemudian akan masuk ke fungsi `moveFile`
```C
void moveFile(char *source, char *dest){
    if (!cekDirectory(source)) {
        FILE *fp1, *fp2;
        int ch;

        fp1 = fopen(source, "r");
        fp2 = fopen(dest, "w");

        while ((ch = fgetc(fp1)) != EOF)
            fputc(ch, fp2);

        fclose(fp1);
        fclose(fp2);

        remove(source);
        work++;
    }
}
```
Dalam fungsi `moveFile` file akan dipindahkan dari path sebelumnya ke path ke direktori berdasarkan pengkategoriannya

![image](https://user-images.githubusercontent.com/66562311/119233420-2b6a5280-bb53-11eb-8f9c-5371f5d99844.png)			![image](https://user-images.githubusercontent.com/66562311/119231753-ac721b80-bb4c-11eb-9d1c-36285b2f723b.png)

![image](https://user-images.githubusercontent.com/66562311/119231875-1ee2fb80-bb4d-11eb-8997-78f023389178.png)

![image](https://user-images.githubusercontent.com/66562311/119231942-65385a80-bb4d-11eb-84ba-e8411bc3d612.png)

### **B. Mengkategorikan file-file dalam satu direktori sesuai path argumen yang diinputkan dengan opsi -d**

```C
    case D:
        getcwd(curr_dirr, sizeof(curr_dirr));
        if (argv[2][0] != '/'){
            strcat(curr_dirr, "/");
            strcat(curr_dirr, argv[2]);
            if (argv[2][strlen(argv[2]) - 1] != '/')
                strcat(curr_dirr, "/");
        }
        dp = opendir(argv[2]);
        if (dp != NULL){
            rekursifListFile(argv[2], iret, threads);
            if (work == 0){
                printf("Yah, gagal disimpan :(\n");
            }
            else{
                printf("Direktori Sukses Disimpan!\n");
            }
        }
        break;
```
Pada argumen `-d` file-file dalam satu direktori akan dikategorikan berdasarkan extensinya. Dalam hal ini menggunakan fungsi `rekursifListFile` 
```C
void rekursifListFile(char *basePath, int *iret, pthread_t *threads){

    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir) return;

    while ((dp = readdir(dir)) != NULL){
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
            char *arr = path;
            iret[i] = pthread_create(&threads[i], NULL, fungsiAll, (void *)arr);
            if (iret[i]){
                fprintf(stderr, "Error - pthread_create() return code: %d\n", iret[i]);
                exit(EXIT_FAILURE);
            }
            pthread_join(threads[i], NULL);
            i++;
            rekursifListFile(path, iret, threads);
        }
    }
    closedir(dir);
}
```
Dalam fungsi `rekursifListFile` akan mengkategorikan satu persatu file dalam direktori yang diinputkan. Dalam pengkategoriannya satu file menggunakan satu thread. kemudian masuk ke fungsi `fungsiAll`
```C
void *fungsiAll(void *ptr){
    int flag = 0;
    char *param = (char *)ptr;
    char *extension_temp = getExtention(param);
    if (strchr(extension_temp, ' ')){
        extension_temp = strrchr(extension_temp, ' ');
        extension_temp = strchr(extension_temp, '.');
    }
    if (strchr(extension_temp, '_')){
        extension_temp = strrchr(extension_temp, '_');
        extension_temp = strchr(extension_temp, '.');
    }
    if (strchr(extension_temp, '-')){
        extension_temp = strrchr(extension_temp, '-');
        extension_temp = strchr(extension_temp, '.');
    }

    int ch, i, length = strlen(extension_temp);

    char extension[100];
    char *filename = basename(param);
    char new_path[1000], temp[1000];
    if (filename[0] == '.'){
        if (!cekDirectory("Hidden"))
            mkdir("Hidden", 0777);
        strcpy(new_path, "Hidden/");
        strcat(new_path, filename);
        moveFile(param, new_path);
    }
    else{
        memset(new_path, 0, sizeof(new_path));
        memset(extension, 0, sizeof(extension));
        for (i = 1; i < length; i++){
            extension[i - 1] = tolower(extension_temp[i]);
        }

        if (!strlen(extension_temp)){
            if (!cekDirectory("Unknown") && !cekDirectory(ptr))
                mkdir("Unknown", 0777);
            strcpy(new_path, "Unknown/");
            strcat(new_path, filename);
            flag = 1;
        }
        else{
            if (!cekDirectory(extension) && !cekDirectory(ptr))
                mkdir(extension, 0777);
            strcpy(new_path, extension);
            strcat(new_path, "/");
            strcat(new_path, filename);
        }
        moveFile(param, new_path);
    }
}

```
Pada `fungsiAll` ini sama dengan `fungsiF` namun dalam fungsi ini tidak mengeluarkan output. Untuk argumen `-d` mengeluarkan output jika pengkategorian berhasil atau gagal berada di fungsi `main`. kemudian setelah mendapatkan tempat direktori yang sesuai dengan pengkategoriannya, maka akan masuk ke fungsi `moveFile` untuk pemindahan file dari path atau direktori sebelumnya ke direktori pengkategoriannya.

![image](https://user-images.githubusercontent.com/66562311/119231701-69b04380-bb4c-11eb-8a7c-a161edd93533.png)  ![image](https://user-images.githubusercontent.com/66562311/119232453-9d409d00-bb4f-11eb-9877-7ee1fd08baed.png)

![image](https://user-images.githubusercontent.com/66562311/119232549-cf51ff00-bb4f-11eb-996a-f789e1500b52.png)

### **C. Mengkategorikan file-file dalam satu direktori yang sama dengan program yang berjalan dengan opsi \***

```C
    case STAR:
        if (getcwd(curr_dirr, sizeof(curr_dirr)) != NULL){
            dp = opendir(curr_dirr);
            if (dp != NULL){
                rekursifListFile(curr_dirr, iret, threads);
            }
        }
        break;
    }
```
Dalam input argumen `\*` cara pengkategoriannya sama dengan argumen `-d` karena mengkategorikan semua file-file dalam satu direktori. Sehingga fungsi yang digunakan juga sama. Melakukan pengkategorian satu persatu file dala direktori dengan fungsi `rekursifListFile` kemudian masuk ke fungsi `fungsiAll` untuk melakukan pengkategorian file sesuai dengan kategrinya. kemudian menuju ke fungsi `moveFile` untuk memindahkan file-file ke direktori sesuai dengan pengkategoriannya.

![image](https://user-images.githubusercontent.com/66562311/119231584-d70fa480-bb4b-11eb-82a3-5e9ac4d99c8d.png)

![image](https://user-images.githubusercontent.com/66562311/119231566-b5aeb880-bb4b-11eb-97cb-0e3cfeca5e41.png)

![image](https://user-images.githubusercontent.com/66562311/119233529-d4b14880-bb53-11eb-99cb-cc3046a36e66.png)

### **D. Mengkategorikan file-file yang tidak memiliki ekstensi disimpan dalam folder “Unknown” dan file hidden, disimpan dalam folder “Hidden”**

```C
if (!strlen(extension_temp)){
            if (!cekDirectory("Unknown"))
                mkdir("Unknown", 0777);
            strcpy(new_path, "Unknown/");
            strcat(new_path, filename);
        }
```
Pengkategoriaan file `Unknown` jika file tersebut tidak memiliki ekstensi. Ketika dicari nama ekstensinya dengan fungsi `getExtention` tidak ditemukan nama ekstensinya

```C
 if (filename[0] == '.'){
        if (!cekDirectory("Hidden"))
            mkdir("Hidden", 0777);
        strcpy(new_path, "Hidden/");
        strcat(new_path, filename);
        moveFile(param, new_path);
```
Pengkategoriaan file `Hidden` jika file tersebut tidak memiliki nama file hanya nama ekstensinya saja atau bisa dicari dengan nama filenya hanya `.` 

![image](https://user-images.githubusercontent.com/66562311/119233353-c6aef800-bb52-11eb-9a4d-2bacf57e0df3.png)

### **E Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread**

Pengkategorian file dengan argumen `-f` menggunakan satu thread setiap satu file yang diinputkan di fungsi `main`

Pengkategorian file dengan argumen `-d` dan `\*` menggunakan satu thread setiap satu file dalam satu direktori pada fungsi `rekursifListFile`

