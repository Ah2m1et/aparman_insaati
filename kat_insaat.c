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