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