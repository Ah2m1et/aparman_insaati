Bu projede, 10 katlı ve her katında 4 daire bulunan bir apartmanın inşaat sürecini modelleyerek işletim sistemlerinin temel kavramları olan process (süreç), thread (iş parçacığı) ve senkronizasyon mekanizmalarını pratik olarak uygulamak amaçlanmıştır.

Derleme ve Çalıştırma:

Kaynak Dosyaları Derle (.o object dosyaları oluştur):

    
    gcc -c daire_insaat.c -o daire_insaat.o -pthread
    gcc -c kat_insaat.c -o kat_insaat.o -pthread
    gcc -c main.c -o main.o


-c: Sadece derle, linkleme yapma.
-pthread: Thread kütüphanesiyle ilgili kodlar olduğu için daire_insaat.c ve kat_insaat.c derlenirken bu bayrak gerekir.

Object Dosyalarını Linkle (Çalıştırılabilir Dosya Oluştur):
    
    gcc main.o kat_insaat.o daire_insaat.o -o apartman_insaat -pthread
    
-o apartman_insaat: Çıktı dosyasının adını apartman_insaat yap.
-pthread: Thread kütüphanesini linkle.

Çalıştır:
Bash

    ./apartman_insaat

