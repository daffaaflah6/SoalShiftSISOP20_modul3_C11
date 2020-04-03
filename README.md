# SoalShiftSISOP20_modul3_C11
Marsha Nabilah Wibowo (05111840000027) || Muhammmad Daffa' Aflah Syarif (05111840000030)

# 1. Poke*ZONE
Source Code :
- [soal1-pokezone.c]()
- [soal1-traizone.c]()

# 2. Tap Tap Game
Source Code :
- [soal2-client.c](https://github.com/daffaaflah6/SoalShiftSISOP20_modul3_C11/blob/master/soal2/soal2-client.c)
- [soal2-server.c](https://github.com/daffaaflah6/SoalShiftSISOP20_modul3_C11/blob/master/soal2/soal2-server.c)

Dalam soal ini, diminta untuk membuat program dalam c untuk game tap-tap berbasis console dengan menggunakan Socket dan Thread.

# 3. Program Mengkategorikan File
Source Code : 
[soal3.c](https://github.com/daffaaflah6/SoalShiftSISOP20_modul3_C11/blob/master/soal3/3.c)

Buatlah sebuah program dari C untuk mengkategorikan file. Program ini akan memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah sama) ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.
```c
for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            for (int k = 0; k < c1; ++k) {
                hasil = hasil + matA[i][k] * matB[k][j];
            }

            mult[i][j] = hasil;
            hasil = 0;
        }
    }

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            *p = mult[i][j];
            sleep(1);
            printf("%d\n", *p);
        }
    }
}
```
- Perkalian matriks kemudian hasilnya di-assign ke *p.

# 4. Teka Teki Norland
Source Code :
- [soal4a.c](https://github.com/daffaaflah6/SoalShiftSISOP20_modul3_C11/blob/master/soal4/4a.c)
- [soal4b.c](https://github.com/daffaaflah6/SoalShiftSISOP20_modul3_C11/blob/master/soal4/4b.c)
- [soal4c.c](https://github.com/daffaaflah6/SoalShiftSISOP20_modul3_C11/blob/master/soal4/4c.c)

a. Buatlah program C dengan nama "4a.c", yang berisi program untuk melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka). Tampilkan matriks hasil perkalian tadi ke layar.

- Menggunakan perkalian matriks biasa setelah itu di-assign ke pointer agar dapat melakukan shared memory dengan soal 4b

b. Buatlah program C kedua dengan nama "4b.c". Program ini akan mengambil variabel hasil pertambahan matriks dari program "4a.c" (program sebelumnya), dan tampilkan hasil matriks tersebut ke layar.

- Melakukan shared memory kemudian melakukan penambahan matriks dengan thread.

c. Pada program ini, Norland diminta mengetahui jumlah file dan folder di direktori saat ini dengan command "ls | wc -l". Karena sudah belajar IPC, Norland mengerjakannya dengan semangat.
