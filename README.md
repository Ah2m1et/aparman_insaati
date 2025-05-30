Bu projede, 10 katlı ve her katında 4 daire bulunan bir apartmanın inşaat sürecini modelleyerek işletim sistemlerinin temel kavramları olan process (süreç), thread (iş parçacığı) ve senkronizasyon mekanizmalarını pratik olarak uygulamak amaçlanmıştır.

Tamamdır, apartman inşaatı projesini senin için adım adım, olabildiğince basit ve ayrı .c dosyaları kullanarak modelleyelim! 🏗️

Senkronizasyon ve temel kavramları, daha önce konuştuğumuz gibi basit tutmaya çalışacağım.
Proje Dosya Yapısı

Şöyle bir dosya yapımız olacak:

    main.c: Ana program. İnşaatın genel akışını (temel atma, kat process'lerini başlatma) yönetecek.
    kat_insaat.h: Kat inşaatıyla ilgili fonksiyonların ve yapıların bildirimleri.
    kat_insaat.c: Bir katın inşaatını yürüten process'in ana mantığı. Daire thread'lerini burada oluşturacağız.
    daire_insaat.h: Daire inşaatıyla ilgili fonksiyonların ve yapıların bildirimleri.
    daire_insaat.c: Bir dairenin inşaatını yürüten thread'in ana mantığı. Ortak tesisat gibi işler için senkronizasyon burada olacak.
    Makefile (isteğe bağlı ama önerilir): Derlemeyi kolaylaştırmak için. Şimdilik komutları doğrudan vereceğim.

1. daire_insaat.h (Daire İnşaatı Başlık Dosyası)

Bu dosya, daire inşaatı thread'inin kullanacağı veri yapısını ve fonksiyon bildirimini içerecek.
C

// daire_insaat.h
#ifndef DAIRE_INSAAT_H
#define DAIRE_INSAAT_H

#include <pthread.h> // Mutex için

// Daire thread'ine göndereceğimiz bilgileri içeren yapı
typedef struct {
    int kat_no;
    int daire_no;
    pthread_mutex_t *tesisat_kilidi; // Katın ortak tesisat kilidi
} daire_bilgisi_t;

// Daire inşaatını başlatan thread fonksiyonu
void *daire_yap(void *arg);

#endif // DAIRE_INSAAT_H

2. daire_insaat.c (Daire İnşaatı Kaynak Dosyası)

Bir dairenin inşaat aşamalarını simüle edeceğiz. "Ortak tesisat" işi için mutex kullanacağız.
C

// daire_insaat.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep için
#include "daire_insaat.h"

void *daire_yap(void *arg) {
    daire_bilgisi_t *bilgi = (daire_bilgisi_t *)arg;

    printf("Kat %d, Daire %d: Kaba inşaat başlıyor.\n", bilgi->kat_no, bilgi->daire_no);
    sleep(2); // Kaba inşaat sürüyormuş gibi yap
    printf("Kat %d, Daire %d: Kaba inşaat bitti.\n", bilgi->kat_no, bilgi->daire_no);

    // --- Ortak Tesisat Bölümü (Senkronizasyon Gerekli) ---
    printf("Kat %d, Daire %d: Ortak tesisat (elektrik/su) için kilit bekleniyor...\n", bilgi->kat_no, bilgi->daire_no);
    pthread_mutex_lock(bilgi->tesisat_kilidi); // Tesisat için kilidi al

    printf("Kat %d, Daire %d: ORTAK TESİSAT yapılıyor (KİLİTLİ BÖLGE).\n", bilgi->kat_no, bilgi->daire_no);
    sleep(3); // Ortak tesisat işi sürüyormuş gibi yap

    pthread_mutex_unlock(bilgi->tesisat_kilidi); // Kilidi bırak
    printf("Kat %d, Daire %d: Ortak tesisat bitti, kilit bırakıldı.\n", bilgi->kat_no, bilgi->daire_no);
    // --- Ortak Tesisat Bölümü Sonu ---

    printf("Kat %d, Daire %d: İç tasarım ve boya başlıyor.\n", bilgi->kat_no, bilgi->daire_no);
    sleep(2); // İç işler sürüyormuş gibi yap
    printf("Kat %d, Daire %d: İNŞAAT TAMAMLANDI.\n", bilgi->kat_no, bilgi->daire_no);

    free(bilgi); // Ana fonksiyonda (kat_insaat.c) ayrılan belleği serbest bırak
    pthread_exit(NULL);
}

3. kat_insaat.h (Kat İnşaatı Başlık Dosyası)
C

// kat_insaat.h
#ifndef KAT_INSAAT_H
#define KAT_INSAAT_H

// Bir katın inşaatını başlatan fonksiyon (process tarafından çağrılacak)
void kat_yap(int kat_numarasi);

#endif // KAT_INSAAT_H

4. kat_insaat.c (Kat İnşaatı Kaynak Dosyası)

Bu dosya, bir katın inşaatını yöneten process'in mantığını içerir. 4 daire için thread'ler burada oluşturulur ve kat için ortak olan tesisat kilidi yönetilir.
C

// kat_insaat.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // exit için
#include "kat_insaat.h"
#include "daire_insaat.h" // daire_bilgisi_t ve daire_yap için

#define DAIRE_SAYISI 4

void kat_yap(int kat_numarasi) {
    printf("---------------------------------------------\n");
    printf(">>>> Kat %d İNŞAATI BAŞLIYOR (Process ID: %d)\n", kat_numarasi, getpid());
    printf("---------------------------------------------\n");

    pthread_t daire_threadleri[DAIRE_SAYISI];
    pthread_mutex_t kat_tesisat_kilidi; // Bu katın daireleri için ortak kilit

    // Kat için ortak tesisat kilidini başlat
    if (pthread_mutex_init(&kat_tesisat_kilidi, NULL) != 0) {
        perror("Kat için mutex başlatılamadı");
        exit(EXIT_FAILURE);
    }

    // Her daire için bir thread oluştur
    for (int i = 0; i < DAIRE_SAYISI; i++) {
        // Her thread'e kendi bilgilerini içeren bir yapı gönderiyoruz
        // Bu yapıyı her thread için ayrı ayrı heap'te oluşturmalıyız
        daire_bilgisi_t *bilgi = (daire_bilgisi_t *)malloc(sizeof(daire_bilgisi_t));
        if (bilgi == NULL) {
            perror("Daire bilgisi için bellek ayrılamadı");
            // Önceki mutex'i destroy et ve çık
            pthread_mutex_destroy(&kat_tesisat_kilidi);
            exit(EXIT_FAILURE);
        }
        bilgi->kat_no = kat_numarasi;
        bilgi->daire_no = i + 1;
        bilgi->tesisat_kilidi = &kat_tesisat_kilidi;

        if (pthread_create(&daire_threadleri[i], NULL, daire_yap, (void *)bilgi) != 0) {
            perror("Daire thread'i oluşturulamadı");
            free(bilgi); // Belleği serbest bırak
            // Burada diğer thread'leri iptal etmek ve mutex'i yok etmek daha doğru olurdu
            // ama basitlik için şimdilik sadece çıkıyoruz.
        } else {
            printf("Kat %d, Daire %d için thread oluşturuldu.\n", kat_numarasi, i + 1);
        }
    }

    // Tüm daire thread'lerinin bitmesini bekle
    for (int i = 0; i < DAIRE_SAYISI; i++) {
        if (daire_threadleri[i] != 0) { // Eğer thread başarıyla oluşturulduysa
             pthread_join(daire_threadleri[i], NULL);
        }
    }

    // Kat için ortak tesisat kilidini yok et
    pthread_mutex_destroy(&kat_tesisat_kilidi);

    printf("---------------------------------------------\n");
    printf("<<<< Kat %d İNŞAATI TAMAMLANDI.\n", kat_numarasi);
    printf("---------------------------------------------\n");
    exit(EXIT_SUCCESS); // Kat process'i başarıyla sonlanır
}

Not: daire_bilgisi_t için malloc ile bellek ayırıp free ile serbest bırakıyoruz. Bu, her thread'in kendi argüman kopyasına sahip olmasını ve ana stack'teki değişkenlerin thread ömründen önce kaybolmamasını sağlar.
5. main.c (Ana Program)

Bu dosya, tüm inşaat sürecini başlatır. Her kat için bir process oluşturur ve katlar arasında senkronizasyonu (bir kat bitmeden diğerinin başlamaması) sağlar.
C

// main.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // fork, getpid için
#include <sys/wait.h> // waitpid için
#include "kat_insaat.h" // kat_yap fonksiyonu için

#define KAT_SAYISI 10

int main() {
    printf("🏢 APARTMAN İNŞAATI BAŞLIYOR (Ana Yüklenici Process ID: %d) 🏢\n\n", getpid());

    printf("🛠️  Temel atılıyor...\n");
    sleep(2); // Temel atma sürüyormuş gibi yap
    printf("✅ Temel tamamlandı.\n\n");

    // Her kat için bir process oluştur
    for (int i = 0; i < KAT_SAYISI; i++) {
        int kat_no = i + 1;
        pid_t kat_pid = fork(); // Yeni bir process (kat inşaatı) yarat

        if (kat_pid < 0) {
            // Hata durumu
            perror("Kat process'i (fork) oluşturulamadı");
            exit(EXIT_FAILURE);
        } else if (kat_pid == 0) {
            // --- Bu blok ÇOCUK PROCESS (Kat İnşaatı) tarafından çalıştırılır ---
            kat_yap(kat_no); // kat_insaat.c içindeki fonksiyonu çağır
            // kat_yap fonksiyonu içinde exit() olduğu için buraya gelinmez.
        } else {
            // --- Bu blok ANA PROCESS (Ana Yüklenici) tarafından çalıştırılır ---
            printf("🏗️ Ana Yüklenici: Kat %d inşaatı için process (PID: %d) başlatıldı.\n", kat_no, kat_pid);
            printf("⏳ Ana Yüklenici: Kat %d inşaatının bitmesi bekleniyor...\n", kat_no);

            int status;
            waitpid(kat_pid, &status, 0); // Oluşturulan kat process'inin bitmesini bekle

            if (WIFEXITED(status)) {
                if (WEXITSTATUS(status) == EXIT_SUCCESS) {
                    printf("✅ Ana Yüklenici: Kat %d inşaatı başarıyla tamamlandı.\n\n");
                } else {
                    fprintf(stderr, "❌ Ana Yüklenici: Kat %d inşaatı bir hatayla sonlandı (çıkış kodu: %d).\n\n", kat_no, WEXITSTATUS(status));
                    // Burada inşaatı durdurma kararı alınabilir. Basitlik için devam ediyoruz.
                }
            } else {
                fprintf(stderr, "❌ Ana Yüklenici: Kat %d inşaatı anormal bir şekilde sonlandı.\n\n", kat_no);
            }
        }
    }

    printf("🎉🎉🎉 TÜM APARTMAN İNŞAATI BAŞARIYLA TAMAMLANDI! 🎉🎉🎉\n");
    return 0;
}

Derleme ve Çalıştırma

Bu dosyaları aynı dizine kaydettikten sonra, bir terminalde aşağıdaki komutlarla derleyip çalıştırabilirsin (Linux/macOS):

    Kaynak Dosyaları Derle (.o object dosyaları oluştur):
    Bash

gcc -c daire_insaat.c -o daire_insaat.o -pthread
gcc -c kat_insaat.c -o kat_insaat.o -pthread
gcc -c main.c -o main.o

    -c: Sadece derle, linkleme yapma.
    -pthread: Thread kütüphanesiyle ilgili kodlar olduğu için daire_insaat.c ve kat_insaat.c derlenirken bu bayrak gerekir.

Object Dosyalarını Linkle (Çalıştırılabilir Dosya Oluştur):
Bash

gcc main.o kat_insaat.o daire_insaat.o -o apartman_insaat -pthread

    -o apartman_insaat: Çıktı dosyasının adını apartman_insaat yap.
    -pthread: Thread kütüphanesini linkle.

Çalıştır:
Bash

./apartman_insaat

