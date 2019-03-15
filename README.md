# SoalShift_Modul2_B05
## Jawaban Soal Shift Modul 2

1. Elen mempunyai pekerjaan pada studio sebagai fotografer. Suatu hari ada seorang klien yang bernama Kusuma yang meminta untuk mengubah nama file yang memiliki ekstensi .png menjadi “[namafile]_grey.png”. Karena jumlah file yang diberikan Kusuma tidak manusiawi, maka Elen meminta bantuan kalian untuk membuat suatu program C yang dapat mengubah nama secara otomatis dan diletakkan pada direktori /home/[user]/modul2/gambar. Catatan : Tidak boleh menggunakan crontab.<br>
### [Source code](https://github.com/fandykun/SoalShift_modul2_B05/blob/master/soal1/soal1.c)
### Jawaban
- Untuk mendapatkan semuafile dalam direktori, maka perlu memanfaatkan library `dirent.h`
- Untuk mengubah namafile, kita perlu tahu dulu filenamenya, dengan memisahkan `.` dan menambahkan `_grey` kedalam file yang berekstensi `.png`
- Karena diminta untuk memindahkan kedalam direktori folder `gambar`, maka perlu membuat `mkdir`
- Dengan memanfaatkan fungsi `rename` file, maka dapat file dapat dipindahkan ke direktori dan disimpan dengan nama yang berbeda

2. Pada suatu hari Kusuma dicampakkan oleh Elen karena Elen dimenangkan oleh orang lain. Semua kenangan tentang Elen berada pada file bernama “elen.ku” pada direktori “hatiku”. Karena sedih berkepanjangan, tugas kalian sebagai teman Kusuma adalah membantunya untuk menghapus semua kenangan tentang Elen dengan membuat program C yang bisa mendeteksi owner dan group dan menghapus file “elen.ku” setiap 3 detik dengan syarat ketika owner dan grupnya menjadi “www-data”. Ternyata kamu memiliki kendala karena permission pada file “elen.ku”. Jadi, ubahlah permissionnya menjadi 777. Setelah kenangan tentang Elen terhapus, maka Kusuma bisa move on. Catatan: Tidak boleh menggunakan crontab.

### Jawaban
Karena tidak diperbolehkan menggunakan ```crontab```, dan proses menghapus file tersebut berlangsung secara terus - menerus, maka gunakan daemon process.
```bash
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
include <time.h>
 
#include <pwd.h>
#include <grp.h>
 
int main()
{
  pid_t pid, sid;
  pid = fork();

  // If pid < 0, fork failed
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }

  /* 
  If pid > 0, it's the parent.
  In order to make a daemon, terminate the parent process
  */
  if (pid > 0)
  {
    exit(EXIT_SUCCESS);
  }

  /*
  Set file mode so it can be written and read properly
  */
  umask(0);
 
  // Create an unique SID
  sid = setsid();
  if (sid < 0)
  {
    exit(EXIT_FAILURE);
  }
 
  /*
  Change directory
  Must be an active and always exists directory
  */
  if ((chdir("/home/abraham")) < 0)
  {
    exit(EXIT_FAILURE);
  }
 
  // Close the standard file descriptor
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
 
  // Main program
    
  int count = 1;
  uid_t userID;
  gid_t groupID;
    
  while(1)
  {
    struct stat attrib;
    struct group *grup;
    struct passwd *pwd;
    
    stat("hatiku/elen.ku", &attrib);
    chmod("hatiku/elen.ku", 777);	
    userID = attrib.st_uid;
    groupID = attrib.st_gid;
    
    grup = getgrgid(groupID);
    pwd = getpwuid(userID);
    
    if (!(strcmp(pwd->pw_name, "www-data")) && !(strcmp(grup->gr_name, "www-data")))
    {
    remove("hatiku/elen.ku");
    }
    sleep(3);
  }

  exit(EXIT_SUCCESS);
}
```
#### Penjelasan
- ```#include <pwd.h>``` dan ```#include <grp.h>``` merupakan library dari perintah yang digunakan untuk mengecek **owner** dan **group**.
- Deklarasikan variabel bertipe data uid_t bernama userID, dan tipe data gid_t bernama groupID. userID dan groupID akan digunakan untuk menangkap nilai userID dan groupID dari file elen.ku.
- Untuk mengecek status / atribut dari sebuah direktori / file, gunakan ```struct stat attrib```. Fungsi penggunaan struct ini adalah untuk mendapatkan atribut dari sebuah direktori yang ditunjuk, seperti yang dijelaskan <a href="https://linux.die.net/man/2/stat">di sini</a>. 
- Setelah mendapatkan atribut dari elen.ku, ubah permission menjadi 777 dengan ```chmod("hatiku/elen.ku", 777);```.
- Ambil nilai ID user dan ID grup dari elen.ku dari **attrib** seperti dengan struct biasa. Untuk ID user disimpan dengan nama ***st_uid***, dan untuk ID grup disimpan dengan nama ***st_gid***. 
- Untuk mengecek owner dan group dari sebuah file, terlebih dahulu deklarasikan ```struct passwd *pwd``` dan ```struct group *grup```. Cari nama owner dan grup dengan fungsi dari **pwd** dan **grup** dengan fungsi ```getpwuid(userID)``` dan ```getgrgid(groupID)```.
- Bandingkan jika nama user (```pwd->pw_name```) dan nama grup (```grup->gr_name```) sama dengan "www-data". Jika benar sama, maka hapus file elen.ku dengan ```remove("hatiku/elen.ku");```.

3. Diberikan file campur2.zip. Di dalam file tersebut terdapat folder “campur2”. Buatlah program C yang dapat : <br>
	i)  Mengekstrak file zip tersebut. <br>
	ii) Menyimpan daftar file dari folder “campur2” yang memiliki ekstensi .txt ke dalam file daftar.txt.<br> 
Catatan:  
	- Gunakan fork dan exec.
	- Gunakan minimal 3 proses yang diakhiri dengan exec.
	- Gunakan pipe.
	- Pastikan file daftar.txt dapat diakses dari text editor.

4. Dalam direktori /home/[user]/Documents/makanan terdapat file makan_enak.txt yang berisikan daftar makanan terkenal di Surabaya. Elen sedang melakukan diet dan seringkali tergiur untuk membaca isi makan_enak.txt karena ngidam makanan enak. Sebagai teman yang baik, Anda membantu Elen dengan membuat program C yang berjalan setiap 5 detik untuk memeriksa apakah file makan_enak.txt pernah dibuka setidaknya 30 detik yang lalu (rentang 0 - 30 detik). Jika file itu pernah dibuka, program Anda akan membuat 1 file makan_sehat#.txt di direktori /home/[user]/Documents/makanan dengan '#' berisi bilangan bulat dari 1 sampai tak hingga untuk mengingatkan Elen agar berdiet.<br>
Contoh:
	- File makan_enak.txt terakhir dibuka pada detik ke-1.
	- Pada detik ke-10 terdapat file makan_sehat1.txt dan makan_sehat2.txt.
	
	Catatan:
	- Dilarang menggunakan crontab
	- Contoh nama file : makan_sehat1.txt, makan_sehat2.txt, dst

### Penjelasan


5. Kerjakan poin a dan b di bawah:<br>
Buatlah program c untuk mencatat log setiap menit dari file log pada syslog ke /home/[user]/log/[dd:MM:yyyy-hh:mm]/log#.log<br>
Ket:
- Per 30 menit membuat folder /[dd:MM:yyyy-hh:mm]
- Per menit memasukkan log#.log ke dalam folder tersebut
- ‘#’ : increment per menit. Mulai dari 1
Buatlah program c untuk menghentikan program di atas.<br>
NB: Dilarang menggunakan crontab dan tidak memakai argumen ketika menjalankan program.

