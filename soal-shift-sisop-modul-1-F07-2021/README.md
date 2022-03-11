# Penjelasan dan Penyelesaian Soal Shift Sisop Modul 1
Kelompok F07
1. Timotius Wirawan 05111940000161
2. Hana Machmudah 05111940000072
3. Rosa Amalia 05111940000106

## No.1
Membuat laporan pesan log beserta keterangan lainnya dari file *syslog.log*. Pada file tersebut berisikan data sebagai berikut:
```
...
Jan 31 00:44:34 ubuntu.local ticky: ERROR Permission denied while closing ticket (ac)
Jan 31 01:00:50 ubuntu.local ticky: INFO Commented on ticket [#4709] (blossom)
...
```
dengan setiap barisnya memiliki pola:
```
<time> <hostname> <app_name>: <log_type> <log_message> (<username>)
```

### (a)
Membuat regex untuk enampilkan informasi dari file log. Informasi yang dibutuhkan diantaranya: jenis pesan log (INFO/ERROR), pesan log dan username.
```Shell
grep -oE "(INFO.*)|(ERROR.*)" syslog.log
```
- `grep` command untuk mencari file dengan pola yang telah ditentukan
- `-o` option untuk menampilkan bagian yang hanya sesuai dengan pola
- `-E` menerjemahkan pola sebagai extended regular expressions (EREs)
- `"(INFO.*)|(ERROR.*)"` pola yang akan dicari
  - Tanda kurung `()` digunakan untuk mengelompokkan pola dan dapat mereferensikan mereka sebagai satu item
  - Simbol bintang `*` pada `INFO.*` dan `ERROR.*` adalah quantifier untuk mencocokkan pola dimulai dari nol dan seterusnya
  - Operator pipe `|` memiliki arti alternatif sebagai *"OR"*.  Operator ini memungkinkan untuk mencari kemungkinan pola dari `ERROR.*` dan `INFO.*`

Ouput yang dihasilkan:
```
...
ERROR Permission denied while closing ticket (ac)
INFO Commented on ticket [#4709] (blossom)
...
```
![Output 1a](https://user-images.githubusercontent.com/68428942/113433519-8f9d4f80-9409-11eb-9725-755c97e7a2b7.png)

*Gambar hanya menampilkan sebagian output*
 
### (b)
Menampilkan semua pesan error dengan jumlah kemunculannya.
```Shell
grep -oE 'ERROR.*' syslog.log
echo | grep -cE 'ERROR' syslog.log 
```
- `grep -oE 'ERROR.*' syslog.log` command yang akan berjalan pertama kali, menampilkan pesan ERROR
  - `grep` command untuk mencari file dengan pola yang telah ditentukan
  - `-o` option untuk menampilkan bagian yang hanya sesuai dengan pola
  - `-E` menerjemahkan pola sebagai extended regular expressions (EREs)
  - Simbol bintang `*` pada `ERROR.*` adalah quantifier untuk mencocokkan pola dimulai dari nol dan seterusnya
- `echo | grep -cE 'ERROR' syslog.log ` command yang akan berjalan berikutnya, menampilkan jumlah pesan ERROR
  - `echo` untuk menampilkan string yang di-input. String di sini adalah sebuah command, maka `echo` akan menampilkan hasil dari command tersebut
  - `grep` command untuk mencari file dengan pola yang telah ditentukan
  - `-c` menghitung banyaknya line yang sesuai dengan pola
  - `-E` menerjemahkan pola sebagai extended regular expressions (EREs)

Ouput yang akan dihasilkan:

![Output 1b](https://user-images.githubusercontent.com/68428942/113434511-4cdc7700-940b-11eb-9d23-b1e13eb7cbbf.png)

*Gambar hanya menampilkan sebagian output*

### (c)
Menampilkan jumlah kemunculan log ERROR dan INFO untuk setiap *user*-nya.
```Shell
name=`grep -oE "(\([a-zA-Z]+\))" syslog.log | sort | uniq | grep -oP "(?<=\().*(?=\))"`
echo "$name" |
while read -r lines
do
    total=`grep -c "$lines" syslog.log`
    lines+=','$total
    echo $lines
done
```
- Variabel `name` berisi command untuk mendapatkan username
  - `grep -oE "(\([a-zA-Z]+\))" syslog.log | sort | uniq | grep -oP "(?<=\().*(?=\))"` command untuk mendapatkan username
    - `grep -oE "(\([a-zA-Z]+\))" syslog.log`
      - `grep` command untuk mencari file dengan pola yang telah ditentukan
      - `-o` option untuk menampilkan bagian yang hanya sesuai dengan pola
      - `-E` menerjemahkan pola sebagai extended regular expressions (EREs)
      - `([a-zA-Z]` pola untuk menampilkan baris apapun yang memiliki setidaknya satu huruf, dengan awalan huruf kecil atau pun huruf kapital dan dimulai dari tanda buka kurung `(`
      - Simbol tambah `+` pada `[a-zA-Z]+` adalah quantifier untuk mencocokkan pola dimulai dari satu dan seterusnya
    - `sort` mengurutkan output dari `grep -oE "(\([a-zA-Z]+\))" syslog.log` sesuai dengan abjad yang dioper melalui operator pipe `|`
    - `uniq` menyaring username sehingga tidak ada yang duplikat
    - `grep -oP "(?<=\().*(?=\))"`
      - `grep` command untuk mencari file dengan pola yang telah ditentukan
      - `-o` option untuk menampilkan bagian yang hanya sesuai dengan pola
      - `-P` menerjemahkan pola sebagai Perl-compatible regular expressions (PCREs)
      - `(?<=\().*(?=\))` menggunakan *lookbehind*. `?<=\()` untuk mencocokkan pola setelah tanda buka kurung `(` dan `.*` mengambil semua string yang setelahnya. `(?=\))` untuk membatasi string yang diambil, yaitu hingga sebelum tanda tutup kurung `)`
- `echo "$name"` menampilkan tiap isi dari variabel name
- Operator pipe `|` untuk mengoper hasil ouput `echo "$name"` ke program perulangan while di bawahnya
  - `while read -r lines` kondisi perulangan while
    - `read` membaca `$name` sebagai `lines`
    - `-r` membuat backslash sebagai simbol, sehingga tidak ada karakter yang hilang
  - Variabel `total` berisi command `grep -c "$lines" syslog.log` untuk menghitung banyak `$lines` yang berisi username
    - `grep` command untuk mencari file dengan pola yang telah ditentukan
    - `-c` menghitung banyaknya line yang sesuai dengan pola `$lines`
  - `lines+=','$total` menambahkan tanda koma dan total di string `lines` yang berisi username
  - `echo $lines` menampilkan `lines` yang berisi username dan jumlah log dan errornya

Output yang dihasilkan:

![Output 1c](https://user-images.githubusercontent.com/68428942/113434715-acd31d80-940b-11eb-8c24-8383ddfc1dcc.png)

### (d)
Informasi yang didapat di poin **(b)** dituliskan ke dalam file `error_message.csv` dengan header **Error, Count** diurutkan berdasarkan *jumlah kemunculan pesan error terbanyak*
```Shell
echo "Error","Count" > error_message.csv
error=`grep -oE "ERROR.*([A-Z][a-z]+)\s(['A-Za-z]+\s){1,5}" syslog.log | sort | uniq`
echo "$error" |
while read -r line
do
    total=`grep -c "$line" syslog.log`
    line+=','$total
    echo $line
done | sort  -rnk 2 -t ',' >> error_message.csv
```
- `echo "Error","Count" > error_message.csv` menambah *Error* dan *Count* sebagai header pada file `error_massage.csv`
- Variabel `error` berisi command untuk mendapatkan ERROR
  - `grep -oE "ERROR.*([A-Z][a-z]+)\s(['A-Za-z]+\s){1,5}" syslog.log | sort | uniq`
    - `grep` command untuk mencari file dengan pola yang telah ditentukan
    - `-o` option untuk menampilkan bagian yang hanya sesuai dengan pola
    - `-E` menerjemahkan pola sebagai extended regular expressions (EREs)
    - `"ERROR.*([A-Z][a-z]+)\s(['A-Za-z]+\s){1,5}"` pola yang akan dicari
      - `ERROR.*` akan mencari dengan pola di awali dengan "ERROR" dan seterusnya
      - `([A-Z][a-z]+)` akan mencari pola dengan huruf pertama kapital dan selanjutnya huruf kecil. Simbol `+` adalah quantifier untuk mencocokkan pola dimulai dari satu dan seterusnya
      - `\s` mencocokkan sesuai dengan spasi
      - `{1,5}` mencocokkan setiap kata dari urutan 1 sampai 5
    - `| sort` output dari command sebelumnya diurutkan
    - `uniq` menyaring ERROR sehingga tidak ada yang duplikat
- `echo "$error" |` menampilkan `$error` dan mengoper output ke perulangan while dibawahnya dengan menggunakan operator pipe `|`
  - `while read -r line` kondisi perulangan while
    - `read` membaca `$error` sebagai `line`
    - `-r` membuat backslash sebagai simbol, sehingga tidak ada karakter yang hilang
  - Variabel `total` berisi command `grep -c "$line" syslog.log` untuk menghitung jumlah ERROR
    - `grep` command untuk mencari file dengan pola yang telah ditentukan
    - `-c` menghitung banyaknya line yang sesuai dengan pola `$line`
  - `line+=','$total` menambahkan tanda koma dan total di string `line` yang berisi pesan ERROR
  - `echo $line` menampilkan `$line` yang berisi pesan ERROR dan jumlah kemunculannya
  - `| sort  -rnk 2 -t ',' >> error_message.csv` memindahkan output dan menambahkannya dibawah ke `error_message.csv`
    - `sort` untuk mengurutkan `$line`
    - `-r` me-reverse urutan sorting
    - `-n` mengurutkan berdasarkan nilai numerik/angka
    - `-k 2` menetapkan urutan berdasarkan field ke-2, yaitu field jumlah ERROR
    - `-t ','` mengenali separator field, yaitu tanda koma
    - `>> error_massage.csv` pesan ERROR yang telah diurutkan dimasukkan tanpa menghilangkan isi `error_message.csv` yang sudah ada
 
 Output yang dihasilkan:
 
 ![Ountput 1d](https://user-images.githubusercontent.com/68428942/113434838-e015ac80-940b-11eb-9e1b-9108647d722f.png)
 
 *Ouput pada file error_message.csv*

### (e)
Semua informasi yang didapatkan pada poin **(c)** dituliskan ke dalam file `user_statistic.csv` dengan header **Username,INFO,ERROR** diurutkan berdasarkan *username* secara *ascending*
```Shell
echo "Username", "INFO", "ERROR" > user_statistics.csv
name=`grep -oE "(\([a-zA-Z]+\))" syslog.log | sort | uniq | grep -oP "(?<=\().*(?=\))"`
for i in $name
do
    echo "$i, $(grep -cP "INFO.*($i)" syslog.log), $(grep -cP "ERROR.*($i)" syslog.log)"
done | sort >> user_statistics.csv
```
- `echo "Username", "INFO", "ERROR" > user_statistics.csv` menambah *Username*, *INFO* dan *ERROR* sebagai header pada file `user_statistics.csv`
- Variabel `name` berisi command untuk mendapatkan username
  - `grep -oE "(\([a-zA-Z]+\))" syslog.log | sort | uniq | grep -oP "(?<=\().*(?=\))"` command untuk mendapatkan username
    - `grep -oE "(\([a-zA-Z]+\))" syslog.log`
      - `grep` command untuk mencari file dengan pola yang telah ditentukan
      - `-o` option untuk menampilkan bagian yang hanya sesuai dengan pola
      - `-E` menerjemahkan pola sebagai extended regular expressions (EREs)
      - `([a-zA-Z]` pola untuk menampilkan baris apapun yang memiliki setidaknya satu huruf, dengan awalan huruf kecil atau pun huruf kapital dan dimulai dari tanda buka kurung `(`
      - Simbol tambah `+` pada `[a-zA-Z]+` adalah quantifier untuk mencocokkan pola dimulai dari satu dan seterusnya
- `for i in $name` perulangan for dengan `$name` sebagai `i`
- `echo "$i, $(grep -cP "INFO.*($i)" syslog.log), $(grep -cP "ERROR.*($i)" syslog.log)"` menampilkan jumlah INFO dan ERROR
  - `$i` username
  - `$(grep -cP "INFO.*($i)" syslog.log)` jumlah INFO sesuai dengan username
    - `grep` command untuk mencari file dengan pola yang telah ditentukan
    - `-c` menghitung banyaknya line yang sesuai dengan pola `INFO.*($i)`
    - `-P` menerjemahkan pola sebagai Perl-compatible regular expressions (PCREs)
    - `INFO.*($i)` pola yang dicari. Diawali dengan "INFO" dan diikuti dengan kata setelahnya `.*` sampai username `($i)`
  - `$(grep -cP "ERROR.*($i)" syslog.log)` jumlah ERROR sesuai dengan username
    - Sama dengan command di atas, bedanya hanya pola dimulai dari "ERROR"
- `sort >> user_statistics.csv` memindahkan output dan menambahkannya di bawah `user_statistics.csv`
  - `sort` mengurutkan input

Ouput yang dihasilkan:

![Ouput 1d](https://user-images.githubusercontent.com/68428942/113435120-64682f80-940c-11eb-9030-c00be65afbaa.png)

*Output pada file user_statistics.csv*
  
## No.2
Mencari kesimpulan dari data penjualan `Laporan-TokoShiSop.tsv`
```Shell
#!/usr/bin/bash

awk -F"\t" '
#soal a
BEGIN{ max=0; home=0; consumer=0; corporate=0; central=0; east=0; south=0; west=0 }
{ pp=$21/($18-$21)*100; if (pp>=max) { max=pp; id=$1 } }

#soal b
{ if ((match($3, "-17")) && $10=="Albuquerque") { nama[$7] } }

#soal c
{ if ($8=="Home Office") { home++ } else if ($8=="Consumer") { consumer++ } else if ($8=="Corporate") { corporate++ } }

#soal d
{ if ($13=="Central") { central+=$21 } else if ($13=="East") { east+=$21 } else if ($13=="South") { south+=$21 } else if ($13=="West") { west+=$21 } }

END {
#soal e
printf "Transaksi terakhir dengan profit percentage terbesar yaitu %d dengan presentase %d%%.\n\n", id, max
printf "Daftar nama customer di Albuquerque pada tahun 2017 antara lain:\n"

for (i in nama)
	printf "%s\n", i
  
if (home<=consumer && home<=corporate)
	printf "\nTipe segmen customer yang penjualannya paling sedikit adalah Home Office dengan %d transaksi.\n", home
else if (consumer<=home && consumer<=corporate)
	printf "\nTipe segmen customer yang penjualannya paling sedikit adalah Consumer dengan %d transaksi.\n", comsumer
else
	printf "\nTipe segmen customer yang penjualannya paling sedikit adalah Corporate dengan %d transaksi.\n", corporate
  
if (central<=east && central<=south && central<=west)
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah Central dengan total keuntungan %f.\n", central
else if (east<=central && east<=south && east<=west)
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah East dengan total keuntungan %f.\n", east
else if (south<=central && south<=east && south<=west)
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah South dengan total keuntungan %f.\n", south
else
	printf "\nWilayah bagian (region) yang memiliki total keuntungan (profit) yang paling sedikit adalah West dengan total keuntungan %f.\n", west
}
' Laporan-TokoShiSop.tsv > hasil.txt
```

- `-F"\t"` untuk mengubah field separator menjadi tab per kolom
- `BEGIN{ max=0; home=0; consumer=0; corporate=0; central=0; east=0; south=0; west=0 }` mendeklarasikan variabel di blok BEGIN

### (a)
Menampilkan ID dan profit percentage terbesar menggunakan formula: `profit percentage = (profit/cost price) x 100` dengan *cost price* didapatkan dari pengurangan Sales dengan Profit.
```Shell
{ pp=$21/($18-$21)*100; if (pp>=max) { max=pp; id=$1 } }
```
- `pp=$21/($18-$21)*100` deklarasi nilai profit percentage, akan berubah di sesuai baris input
  - `$21` kolom **Profit**
  - `$18` kolom **Sales**
- `id=$1` variabel id menyimpan id dari kolom **Row ID**

### (b)
Menampilkan nama *customer* pada transaksi tahun 2017 di Albuquerque
```Shell
{ if ((match($3, "-17")) && $10=="Albuquerque") { nama[$7] } }
```
- `if ((match($3, "-17")) && $10=="Albuquerque")` mencari kondisi yang sesuai
  - `match($3, "-17"` menggunakan fungsi match() untuk mencari record yang mengandung **"-17"** (tahun 2017) di kolom `$3` **Order Date**
  - `$10=="Albuquerque"` mencari record yang persis sama dengan **"Albuquerque"** di kolom `$10` **City**
- `{ nama[$7] }` memasukkan nama di kolom `$7` **Customer Name** ke array nama

### (c)
Menampilkan segment customer dan jumlah transaksinya yang paling sedikit
```Shell
{ if ($8=="Home Office") { home++ } else if ($8=="Consumer") { consumer++ } else if ($8=="Corporate") { corporate++ } }
```
- Mencari kondisi yang sesuai di kolom **Segment**
  - `$8` Kolom **Segment** 
- Setiap kondisi yang sesuai ditemukan, variabel untuk menghitung masing-masing segment akan bertambah

### (d)
Menampilkan wilayah bagian (region) yang memiliki total keuntungan (profit) paling sedikit dan total keuntungan wilayah tersebut
```Shell
{ if ($13=="Central") { central+=$21 } else if ($13=="East") { east+=$21 } else if ($13=="South") { south+=$21 } else if ($13=="West") { west+=$21 } }
```
- Mencari kondisi yang sesuai di kolom **Region**
  - `$13` kolom **Region**
- Menambahkan profit dari kolom **Profit** ke variabel untuk menghitung jumlah profit masing-masing region
  - `$21` kolom **Profit**

### (e)
Membuat script yang menghasilkan file **hasil.txt** dengan format yang sudah ditentukan.
Pada blok END akan mencetak setiap soal yang telah dibuat dengan format yang sudah ditentukan.
```Shell
Laporan-TokoShiSop.tsv > hasil.txt
```
- `Laporan-TokoShiSop.tsv` sumber file yang menjadi input
- `> hasil.txt` hasil output akan dikirim ke file **hasil.txt**

output yang dihasilkan:
![Output no2](https://user-images.githubusercontent.com/66562311/113478896-e9f3ea00-94b5-11eb-9259-00ea7fe25230.PNG)


## No.3
Mengunduh gambar dari website tertentu
### (a)
Mengunduh 23 gambar dari `https://loremflickr.com/320/240/kitten` dan menyimpan log-nya ke file `Foto.log`, kemudian menyimpannya dengan nama `Koleksi_XX`
```Shell
host="https://loremflickr.com"
to=$(curl -s -i "https://loremflickr.com/320/240/kitten" | grep "location" | awk -F ': ' '{print $2}' | tr -d $'\r')
download="$host$to"
```
Disini kami menggunakan url untuk membandingkan foto yang sama dimana variabel to merupakan variabel penting untuk membandingkan tiap gambarnya. Untuk variabel download digunakan untuk disimpan pada Foto.log

```Shell
if [ $(cat Foto.log | grep -c "${download}") -gt 1 ]
    then
        rm $name
else
if [ 1 -eq $(echo "${number} <10" | bc) ]
        then
            number="0${number}"
        fi

    filename="Koleksi_${number}.jpg"
    mv $name "$filename"
```
Sebelumnya telah dijelaskan bahwa untuk mendeteksi gambar yang sama dengan membandingkan url pada tiap gambar. Disini url telah disimpan di Foto.log maka akan dihitung dari log tersebut. Jika url yang sama sudah lebih dari 1, maka foto tersebut akan dihapus dan jika sebaliknya akan di ganti nama filenya.

Saat program dijalankan, akan ada proses mengunduh seperti ini untuk tiap gambar:

![Proses Download](https://user-images.githubusercontent.com/68428942/113464052-1c6cfb00-9454-11eb-896b-19e56d8950b6.png)

Dengan Foto.log menyimpan variabel download:

![Foto.log](https://user-images.githubusercontent.com/68428942/113464355-1aa43700-9456-11eb-9023-600376825cea.png)

Dan gambar yang akan terunduh:

![Gambar](https://user-images.githubusercontent.com/68428942/113464268-8c2fb580-9455-11eb-9267-a41c6b6f579b.jpg)


### (b)
Menjalankan script dengan jadwal sehari sekali pada jam 8 malam untuk tanggal-tanggal tertentu setiap bulan, yaitu dari tanggal 1 tujuh hari sekali (1,8,...), serta dari tanggal 2 empat hari sekali(2,6,...). Kemudian dipindahkan ke folder dengan nama sesuai tanggal unduhannya
```Shell
date=$(date +'%d-%m-%Y')

if [ ! -d "$date" ]
then
    mkdir "$date"
fi
```
Untuk membuat folder dibutuhkan tanggal terlebih dahulu, maka fungsi dari variabel date adalah untuk menyimpan tanggal. Jika folder tanggal hari ini telah dibuat maka akan datanya akan disimpan di folder yang telah dibuat. Untuk script selebihnya sama dengan soal3a hanya ada perbedaan pada directory file saja.
### Cron3b.tab

```
0 20 1-31/7,2-31/4 * * /home/Downloads/Downloads/soal-shift-sisop-modul-1-F07-2021/soal3b.sh
```
Crontab diatas menjalankan file yang akan dieksekusi pada sehari sekali pada jam 20.00 pada tanggal-tanggal tertentu setiap bulannya. Untuk tanggal 1, tujuh hari sekali misalkan 1, 8, 15,..., serta dari tanggal 2, empat hari sekali misalkan 2, 6, 10,.., dimana file yang akan dieksekusi berada pada local user atau untuk kasus diatas berada pada
Saat program dijalankan, akan ada proses mengunduh seperti ini untuk tiap gambar:

![Proses Download](https://user-images.githubusercontent.com/68428942/113464395-89819000-9456-11eb-8734-790d67a03b6d.png)

Folder dengan nama sesuai tanggal unduhannya:

![Forder](https://user-images.githubusercontent.com/68428942/113464544-a4083900-9457-11eb-9fc8-06f16dc14f18.jpg)

Dengan isi folder:

![Isi Folder](https://user-images.githubusercontent.com/68428942/113464563-c69a5200-9457-11eb-9832-56f58cc2d0a6.png)


### (c)
Mengunduh gambar dari `https://loremflickr.com/320/240/bunny`, kemudian diunduh secara bergantian dengan sumber di poin **(a)**. Kemudian disimpan ke folder dengan nama yang sesuai dengan jenisnya ("Kucing_" atau "Kelinci_")
```Shell
currentDate=$(date +'%d-%m-%Y')
yesterdayDate=$(date -d "yesterday" +'%d-%m-%Y')

if [ $(ls -l | grep "$yesterdayDate" | grep -c "Kucing") -eq 0 ]
then
    if [ ! -d "Kucing_$currentDate" ]
    then
        mkdir "Kucing_$currentDate"
    fi
    kucing
else
    if [ ! -d "Kelinci_$currentDate" ]
    then
        mkdir "Kelinci_$currentDate"
    fi
    kelinci
fi
```
Untuk fungsi dari kucing dan kelinci sama seperti fungsi 3a hanya diubah pada directory file saja dan untuk kelinci diubah pada url. Disini saya menggunakan kucing sebagai awalan, untuk cara kerjanya sendiri yaitu mengecek folder kucing dan tanggal dengan tanggal kemarin, jika tidak ada maka akan dibuat folder kucing terlebih dahulu lalu menjalankan fungsi kucing. Jika tidak maka akan membuat folder kelinci dan menjalankan fungsi kelinci.

Saat program dijalankan, akan ada proses mengunduh seperti ini untuk tiap gambar:

![Proses Download](https://user-images.githubusercontent.com/68428942/113464669-bf277880-9458-11eb-865d-aea9b82e2a95.png)

Folder yang akan terbuat:

![Folder](https://user-images.githubusercontent.com/68428942/113464840-ee8ab500-9459-11eb-8ad3-7334d0ff00e8.jpg)

Isi dari folder:

![Isi Folder](https://user-images.githubusercontent.com/68428942/113464862-1417be80-945a-11eb-8a6f-8770d80f4d0f.png)


### (d)
Memindahkan seluruh folder ke zip dan menguncinya dengan password

```
pass=`date +'%m%d%Y'`

for i in */ ; do
    echo "$i"
    zip -r -P $pass Koleksi.zip . -i "$i*"
    rm -rf "$i"
done
```
Membuat variabel `pass` yang digunakan untuk menyimpan password folder yang akan di zip. Pada perulangan for, variabel `i` mengambil semua folder melalui `*/`. Tiap `i` dimasukkan ke dalam `Koleksi.zip` dengan command `zip`.
- `-r` akan mengecek tiap folder yang akan di zip dengan rekursif
- `-P` untuk menggunakan password pada folder zip
- `-i` untuk hanya memasukkan folder yang memiliki nama yang sama dengan `$i*`
- `rm` untuk menghapus folder `$i`
  - `-r` menghapus secara rekursif
  - `-f` mengabaikan file dan argumen yang tidak ada (force)

Proses memindahkan folder ke zip:

![Proses Memindahkan Folder](https://user-images.githubusercontent.com/68428942/113464903-635def00-945a-11eb-93d1-a2e9a3a11a95.png)

Folder zip yang terbuat:

![Folder zip](https://user-images.githubusercontent.com/68428942/113465039-57266180-945b-11eb-9834-9e8c28f6b266.jpg)


### (e) cron3e.tab
Membuat koleksi sebelumnya ter-**zip** di waktu: setiap hari kecuali sabtu dan minggu, dari jam 7 pagi sampai 6 sore. Selain di jadwal tersebut, file ter-**unzip**
Setiap hari kecuali sabtu dan minggu, dari jam 7 pagi sampai 6 sore, ia memintamu untuk membuat koleksinya ter-zip.

```
#zip
0 7 * * 1-5 /home/Downloads/Downloads/soal-shift-sisop-modul-1-F07-2021/soal3d.sh

#unzip
0 18 * * 1-5 pass=$(date +'%m%d%Y'); unzip -P "$pass" Koleksi.zip; rm Koleksi.zip
```
Crontab diatas menjelaskan bahwa pada jam 07.00 sampai 18.00 dari hari senin sampai jumat untuk menjalankan script untuk membuat koleksi zipnya pada script soal3d.sh.
Selain dari waktu yang disebutkan, ia ingin koleksinya ter-unzip dan tidak ada file zip sama sekali

## Kendala dalam pengerjaan
Saat mengerjakan nomor 2, output yang dihasilkan tiap-tiap anggota berbeda padahal file yang dijalankan sama sehingga menimbulkan kebingungan. Kendala ini ternyata disebabkan oleh pengaturan tiap linux anggota berbeda.

![kendala no2](https://user-images.githubusercontent.com/68428942/113478905-f4ae7f00-94b5-11eb-9053-a9767788fd09.png)

*Hasil ouput yang tidak sesuai dengan yang seharusnya*

![Output no2](https://user-images.githubusercontent.com/66562311/113478896-e9f3ea00-94b5-11eb-9259-00ea7fe25230.PNG)

*Hasil ouput yang seharusnya*
