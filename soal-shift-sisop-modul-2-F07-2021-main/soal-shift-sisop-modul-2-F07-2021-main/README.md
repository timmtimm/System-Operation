# soal-shift-sisop-modul-2-F07-2021
Kelompok F07
1. Timotius Wirawan 05111940000161
2. Hana Machmudah 05111940000072
3. Rosa Amalia 05111940000106

# Soal No.1
Pada suatu masa, hiduplah seorang Steven yang hidupnya pas-pasan. Steven punya pacar, namun sudah putus sebelum pacaran. Ketika dia galau memikirkan mantan, ia selalu menonton https://www.youtube.com/watch?v=568DH_9CMKI untuk menghilangkan kesedihannya.

Di lain hal Steven anak yang tidak amat sangat super membenci matkul sisop, beberapa jam setelah diputus oleh pacarnya dia menemukan wanita lain bernama Stevany, namun Stevany berkebalikan dengan Steven karena menyukai sisop. Steven ingin terlihat jago matkul sisop demi menarik perhatian Stevany.

Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi.

`(a)` Dikarenakan Stevany sangat menyukai huruf Y, Steven ingin nama folder-foldernya adalah Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg `(b)`untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). `(c)` Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu meng-extract-nya setelah didownload serta `(d)` memindahkannya ke dalam folder yang telah dibuat (hanya file yang dimasukkan).`(e)` Untuk memudahkan Steven, ia ingin semua hal di atas berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany).`(f)` Setelah itu pada waktu ulang tahunnya Stevany, semua folder akan di zip dengan nama Lopyu_Stevany.zip dan semua folder akan di delete(sehingga hanya menyisakan .zip).

Kemudian Steven meminta bantuanmu yang memang sudah jago sisop untuk membantunya mendapatkan hati Stevany. Bantu Woy!!

**Note:**
- Ulang Tahun Stevany : 09 April Pukul 22.22 WIB
- Semua poin dijalankan oleh 1 script di latar belakang, termasuk mendownload file zip-nya. Jadi cukup jalankan script 1x serta ubah time dan date untuk check hasilnya.
- Tidak boleh menggunakan fungsi system(), mkdir(), dan rename() (Yang di bahasa C) .... FORBIDDENNN!!
- Tidak boleh pake cron !!!
- Menggunakan fork dan exec.
- Link
    ```
    Foto   : https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD/view
    Musik  : https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J/view
    Film   : https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp/view
    ```

# Jawaban No. 1

### **A. Membuat folder Musyik untuk mp3, Fylm untuk mp4, dan Pyoto untuk jpg**
```C
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
```
#### Penjelasan A
Fungsi ini melakukan fork yang digunakan untuk membuat folder sesuai permintaan.


### **B. Mendownloadnya zip dari link yang telah disediakan**
```C
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
```
#### Penjelasan B
Fungsi ini melakukan fork sesuai banyaknya zip yang akan didownload lalu digunakan untuk mendownload zip sesuai permintaan.


###  **C. Mengextract zip yang telah didownload**
```C
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
```
#### Penjelasan C
Fungsi ini melakukan fork sesuai banyaknya zip yang akan didownload lalu digunakan untuk mengextract zip yang telah didownload.


### **D. Memindahkan file yang telah diextract ke dalam folder yang telah dibuat**
```C
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
```
#### Penjelasan D
Fungsi ini melakukan looping sebanyak folder yang ada lalu looping pada banyakny file pada setiap folderlalu dipindahkan ke directory yang diinginkan.


### **E. Jawaban soal A-D berjalan otomatis 6 jam sebelum waktu ulang tahun Stevany**
```C
void timeBefore6Hours(pid_t pid, int *status, char *folderStevany[], char *linkDownload[], char *saveName[], char *folderExtract[], char *copyTo[])
{
  makeFolder(pid, status, folderStevany);
  downloadZip(pid, status, linkDownload, saveName);
  unZip(pid, status, saveName);
  copyFolder(pid, status, folderExtract, copyTo);
}
```
#### Penjelasan E
Fungsi ini digunakan untuk menyatukan jawaban dari A-D untuk dilaksanakan nanti pada saat 6 jam sebelum ulang tahun Stevany.


### **F. Mengzip file yang telah dibuat pada soal A dan folder extract lalu dihapus**
```C
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

void zipForStevany(pid_t pid, int *status, char *folderStevany[])
{
  int i = 0;
  pid = fork();

  if(pid == 0)
  {
    char *argv[] = {"zip", "-r", "Lopyu_Stevany.zip", folderStevany[0], folderStevany[1], folderStevany[2], NULL};
    execv("/bin/zip", argv);
  }

  while(wait(status)>0);
  
}
```
#### Penjelasan F
- Fungsi `removeFolder` digunakan untuk menghapus folder yang ada dengan melakukan fork.
- Fungsi `zipForStevany` digunakan untuk mengzip folder yang telah dibuat pada poin A.


### **Fungsi untuk membandingkan waktu ulang tahun Stevany dan untuk menjalankan fungsi pada poin E dan F**
```C
void runTime(int *status, char *folderStevany[], char *linkDownload[], char *saveName[], char *folderExtract[], char *copyTo[], char *folderExtracted[])
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
        timeBefore6Hours(pid, status, folderStevany, linkDownload, saveName, folderExtract, copyTo);
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
```
#### Penjelasan
Fungsi ini sebagai pembanding waktu ulang tahun Stevany dengan waktu sekarang dan menjalankan fungsi yang diminta sesuai waktu yang ditentukan.

### **Program utama dan Daemon**
```C
int main()
{
  char *folderStevany[]   = {"Fylm", "Pyoto", "Musyik"};
  char *linkDownload[]    = {"https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download", 
                              "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download",
                              "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download"};
  char *saveName[]        = {"Film_for_Stevany.zip", "Foto_for_Stevany.zip", "Musik_for_Stevany.zip"};
  char *folderExtract[]   = {"/home/timtim/Desktop/SisOp/Praktikum 2/soal1/FILM",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/FOTO",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/MUSIK"};
  char *copyTo[]          = {"/home/timtim/Desktop/SisOp/Praktikum 2/soal1/Fylm",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/Pyoto",
                              "/home/timtim/Desktop/SisOp/Praktikum 2/soal1/Musyik"};
  char *folderExtracted[] = {"FILM", "FOTO", "MUSIK"};
  char *folder            = {"/home/timtim/Desktop/SisOp/Praktikum 2/soal1"};

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
  
  runTime(&status, folderStevany, linkDownload, saveName, folderExtract, copyTo, folderExtracted);

  return 0;
}
```
#### Penjelasan
Program utama ini digunakan sebagai penyimpan string yang akan digunakan pada setiap fungsi dan inisialisasi program agar dapat berjalan di belakang layar.

### **Demo Program**
Demo program bisa dilihat pada link berikut
https://youtu.be/NOtwsf5BvRw

### **Kendala dalam pengerjaan**
- Ada error mungkin dari compiler komputer saya, dimana saat itu program sudah berjalan dengan baik padahal seharusnya belum bisa. Untuk kasus ini waktu itu di program mainnya belum di fork tetapi berjalan.

# Soal No. 2
Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

`a.` Pertama-tama program perlu mengextract zip yang diberikan ke dalam folder “/home/[user]/modul2/petshop”. Karena bos Loba teledor, dalam zip tersebut bisa berisi folder-folder yang tidak penting, maka program harus bisa membedakan file dan folder sehingga dapat memproses file yang seharusnya dikerjakan dan menghapus folder-folder yang tidak dibutuhkan.

`b.` Foto peliharaan perlu dikategorikan sesuai jenis peliharaan, maka kamu harus membuat folder untuk setiap jenis peliharaan yang ada dalam zip. Karena kamu tidak mungkin memeriksa satu-persatu, maka program harus membuatkan folder-folder yang dibutuhkan sesuai dengan isi zip.
Contoh: Jenis peliharaan kucing akan disimpan dalam `“/petshop/cat”` , jenis peliharaan kura-kura akan disimpan dalam `“/petshop/turtle”`.

`c.` Setelah folder kategori berhasil dibuat, programmu akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.
Contoh: `“/petshop/cat/joni.jpg”`.

`d.` Karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai. Contoh: foto dengan nama `“dog;baro;1_cat;joni;2.jpg”` dipindah ke folder `“/petshop/cat/joni.jpg”` dan `“/petshop/dog/baro.jpg”`.

`e.` Di setiap folder buatlah sebuah file "keterangan.txt" yang berisi nama dan umur semua peliharaan dalam folder tersebut. Format harus sesuai contoh.

```
nama : joni
umur  : 3 tahun

nama : miko
umur  : 2 tahun
```

Loba sangat mengapresiasi bantuanmu, minggu depan ia akan mentraktir makan malam!

**Note:**
- Setiap data peliharaan disimpan sebagai nama foto dengan format [jenis peliharaan]:[nama peliharaan]:[umur peliharaan dalam tahun]. Jika terdapat lebih dari satu peliharaan, data dipisahkan menggunakan underscore(_).
- Tidak boleh menggunakan fungsi system(), mkdir(), dan rename().
- Menggunakan fork dan exec.

# Jawaban No. 2

### **A. Mengekstrak folder pet.zip ke folder petshop, kemudian folder di dalamnya dihapus**
```C
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
```

Bagian fungsi `managefile()`
```C
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
			
		..........
```
##### Penjelasan A
Pada fungsi `main()` melakukan fork untuk melakukan pembuatan folder `petshop` dan mengekstrak folder `pet.zip` ke folder `petshop`. Kemudian, isi folder `petshop` disortir untuk mengapus folder-folder di dalamnya di fungsi `manageFile()`. Tiap file di folder diperiksa tipe filenya melalui anggota struct `d_type`, jika bernilai 4 akan dihapus karena itu tandanya adalah folder.


### Jawaban B - E
File yang akan dikategorikan, ekstensi file dihapus dari nama file menggunakan fungsi `hapussubstr()`.
```C
void hapussubstr (char *str, char *substr) {
    char *match;
    int len_sub = strlen(substr);
    
    while ((match = strstr(str, substr))) {
      	*match = '\0';
      	strcat(str, match+len_sub);
    }
}
```
Fungsi `hapussubstr()` dipanggil di dalam fungsi `managefile()`.
```C
void managefile() {
    DIR *dp;
    struct dirent *ep;
    char path[] = "/home/rosaam/modul2/petshop/";

    dp = opendir(path);

    if (dp != NULL)
    {
		..........
		
                	char path[] = "/home/rosaam/modul2/petshop/";
                 	char file[100];
                 	strcpy(file, ep->d_name);

                 	hapussubstr(file, ".jpg");
                 	char jenis[15];
                 	char nama[15];
                 	char umur[15];

                ..........
    }

    (void) closedir (dp);
    } else perror ("Couldn't open the directory");

    return;
}
```
Setelah string nama file sudah tidak memiliki ekstensi file-nya lagi, nama file dipisah untuk mendapatkan jenis hewan peliharaan, nama hewan dan umur hewan tersebut, dengan menggunakan fungsi `strtok` di perulangan `while`.
```C
void managefile() {
    DIR *dp;
    struct dirent *ep;
    char path[] = "/home/rosaam/modul2/petshop/";

    dp = opendir(path);

    if (dp != NULL)
    {
		.........

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

		..........
    }

    (void) closedir (dp);
    } else perror ("Couldn't open the directory");

    return;
}
```
Pada perulangan `while` terdapat counter `i`. Karena maksimal data hewan pada nama file ada 2, dapat disimpulkan `while` akan melakukan perulangan maksimal 6 kali, dengan tiap perulangan akan mengambil string berupa:
- Tiap perulangan ke-0 dan ke-3 akan mendapatkan string jenis peliharaan
  - Panggil fungsi `buatfolder()` untuk membuat folder dengan nama yang sesuai dengan jenis peliharaan
  ```C
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
  ```
- Tiap perulangan ke-1 dan ke-4 akan mendapatkan string nama peliharaan
  - Panggil fungsi `pindahfoto()` untuk mengcopy foto asli ke folder yang telah dibuat sesuai dengan jenis peliharaan yang telah didapatkan. Dilakukan copy file jika ternyata pada nama memiliki dua jenis hewan, dapat dimasukkan ke dua folder yang berbeda. File foto diganti namanya sesuai dengan nama peliharaan yang sudah diambil, dengan menempelkan string nama dan ekstensi file ke path file.
  ```C
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
  ```
- Tiap perulangan ke-3 dan ke-5 akan mendapatkan string umur peliharaan
  - Panggil fungsi `keterangan()` untuk memasukkan nama hewan dan umur hewan ke file `keterangan.txt`
  ```C
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
  ```

Kemudian, file asli yang masih berada di folde dihapus. Sehingga hanya menyisakan folder-folder yang berisi file foto yang sesuai serta file `keterangan.txt`.
```C
void managefile() {
    DIR *dp;
    struct dirent *ep;
    char path[] = "/home/rosaam/modul2/petshop/";

    dp = opendir(path);

    if (dp != NULL)
    {
		..........

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
            ..........
    }

    (void) closedir (dp);
    } else perror ("Couldn't open the directory");

    return;
}
```
### Output Program
Program akan berjalan seperti gambar di bawah ini:
![image](https://user-images.githubusercontent.com/68428942/115954913-c1dc3180-a51d-11eb-8b27-f3c28682754d.png)

Setelah program selesai berjalan, isi folder `petshop` adalah folder-folder dengan nama jenis peliharaan.
![image](https://user-images.githubusercontent.com/68428942/115954975-1a133380-a51e-11eb-88f1-9bcc56c79f29.png)

Dan di dalam folder, akan berisi file foto yang sesuai dengan jenisnya dan file `keterangan.txt`. Berikut isi salah satu folder:
![image](https://user-images.githubusercontent.com/68428942/115955039-74ac8f80-a51e-11eb-9bb3-e068642c103e.png)

dan isi file .txt
![image](https://user-images.githubusercontent.com/68428942/115955053-93ab2180-a51e-11eb-9b63-2cb8a797c74d.png)

# Soal No. 3
Ranora adalah mahasiswa Teknik Informatika yang saat ini sedang menjalani magang di perusahan ternama yang bernama “FakeKos Corp.”, perusahaan yang bergerak dibidang keamanan data. Karena Ranora masih magang, maka beban tugasnya tidak sebesar beban tugas pekerja tetap perusahaan. Di hari pertama Ranora bekerja, pembimbing magang Ranora memberi tugas pertamanya untuk membuat sebuah program.

`a.` Ranora harus membuat sebuah program C yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss].

`b.` Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

`c.` Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Caesar Cipher adalah Teknik enkripsi sederhana yang dimana dapat melakukan enkripsi string sesuai dengan shift/key yang kita tentukan. Misal huruf “A” akan dienkripsi dengan shift 4 maka akan menjadi “E”. Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip saja.

`d.` Untuk mempermudah pengendalian program, pembimbing magang Ranora ingin program tersebut akan men-generate sebuah program “Killer” yang executable, dimana program tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan. Karena Ranora menyukai sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat nantinya harus merupakan program bash.

`e.` Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete direktori).

Ranora meminta bantuanmu untuk membantunya dalam membuat program tersebut. Karena kamu anak baik dan rajin menabung, bantulah Ranora dalam membuat program tersebut!

**Note:**
- Tidak boleh menggunakan system() dan mkdir()
- Program utama merupakan SEBUAH PROGRAM C
- Wajib memuat algoritma Caesar Cipher pada program utama yang dibuat

# Jawaban No. 3
### **A. Membuat sebuah direktori dengan nama sesuai timestamp `[YYYY-mm-dd_HH:ii:ss]` setiap 40 detik **

#### Penjelasan A
untuk mebuat direktori, kami menggunakan fungsi `makeFolder'
```C
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
```
agar direktori dapat dibuat setiap selama 40 detik maka menggunakan `sleep(40)` di fungsi main dan menamai direktori dengan `"%Y-%m-%d_%H:%M:%S"`
```C
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
```
![image](https://user-images.githubusercontent.com/66562311/115957394-52ba0980-a52c-11eb-8cba-a33dc223fdff.png)

### **B. Mendownload 10 gambar dari `https://picsum.photos/` di direktor yang sudah dibuat, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp `[YYYY-mm-dd_HH:ii:ss]` dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel **

#### Penjelasan B
untuk melakukan download, pada fungsi `download` sebanyak 10 gambar dengan melakukan looping dan setiap 5 detik sekali dengan memaikai fungsi `sleep(5)`
```C
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
```

![image](https://user-images.githubusercontent.com/66562311/115957240-526d3e80-a52b-11eb-91d4-5dfa923535b5.png)

### **C. Membuat file `status.txt` di tiap folder, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5 **

#### Penjelasan C
Membuat file `status.txt` pada fungsi `succecStat` dan melakukan enkripsi Caesar Chiper dengan me-looping pada tiap-tiap kata "Download Success"
```C
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
```

file `status.txt` akan terbuat setelah 10 gambar telah terdownload dalam satu direktori

![image](https://user-images.githubusercontent.com/66562311/115957357-171f3f80-a52c-11eb-805f-39dc0d9b0808.png)

hasilnya akan seperti ini di file `status.txt`

![image](https://user-images.githubusercontent.com/66562311/115952081-59d21f00-a50e-11eb-9e63-40e72f7c60c2.png)

### **D. Membuat program `Killer.sh` **

#### Penjelasan D
Membuat program `Killer.sh` pada fungsi `fileKill`
```C
void fileKill(const char *argv[],int sid)
{
    FILE *fptr;
    fptr = fopen("killer.sh","w");
    fputs("#!/bin/bash\n\n",fptr);
    char program[100];
    ;
    ;
}  
```

![image](https://user-images.githubusercontent.com/66562311/115957307-ad069a80-a52b-11eb-88a5-5076a1891598.png)

### **E. Membuat program berjalan dengan argumen `-z` dan `-x`  dan mengubah zip pada direktori yang sudah selesai mendownload **

#### Penjelasan E
pada fungsi `main` menginisialisaikan program bisa berjalan dengan argumen `-z` dan `-x` 
```C
int main(int argc, const char **argv)
{
    pid_t parentPID;
    parentPID = getpid();

    if(!strcmp(argv[1],"-z") || !strcmp(argv[1],"-x"))
    {
        ;
        ;
    }
}
```
kemudian pada fungsi `fileKill` jika argumen `-z` digunakan `killall` untuk melakukan kill pada parent sehingga program utama akan langsung menghentikan semua operasinya, sedangkan untuk argumen `-x` menggunakan `kill` maka program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai
```C
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
```
![image](https://user-images.githubusercontent.com/66562311/115957269-7d579280-a52b-11eb-8379-1ce15f1d3a67.png)

kemudian untuk melakukan zip pada direktori yang telah selesai dengan menggunkan fungsi `zipFile`
```C
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
```
![image](https://user-images.githubusercontent.com/66562311/115957258-6add5900-a52b-11eb-945a-e939ecf7fb94.png)

