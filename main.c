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