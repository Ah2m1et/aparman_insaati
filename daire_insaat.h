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