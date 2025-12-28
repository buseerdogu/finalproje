#include <stdio.h>
#include <string.h>

#define MAX 50

/* ----- STRUCT TANIMLARI ----- */

struct Uye {
    int id;
    char ad[30];
};

struct Kitap {
    int id;
    char ad[30];
    char yazar[30];
    char isbn[20];
    int durum;        // 0 = Musait, 1 = Oduncte
    int alanUyeId;
};

/* ------ GLOBAL DIZILER ------ */

struct Uye uyeler[MAX];
struct Kitap kitaplar[MAX];

int uye_sayisi = 0;
int kitap_sayisi = 0;

/* ------ FONKSIYON PROTOTIPLERI ----- */

void menu();
void uyeEkle();
void kitapEkle();
void kitapOduncVer();
void kitapIadeAl();
void kitapListele();
void uyeListele();
void dosyadanOku();
void dosyayaYaz();

/* ------ ANA PROGRAM ------- */

int main() {
    int secim;

    dosyadanOku();

    while (1) {
        menu();
        scanf("%d", &secim);
        getchar(); 

        switch (secim) {
            case 1: uyeEkle(); break;
            case 2: kitapEkle(); break;
            case 3: kitapOduncVer(); break;
            case 4: kitapIadeAl(); break;
            case 5: kitapListele(); break;
            case 6: uyeListele(); break;
            case 7:
                dosyayaYaz();
                printf("Veriler kaydedildi. Program sonlandiriliyor...\n");
                return 0;
            default:
                printf("Hatali secim!\n");
        }
    }
}

/* ----- MENU ------ */

void menu() {
    printf("\n===== KUTUPHANE OTOMASYONU =====\n");
    printf("1 - Uye Ekle\n");
    printf("2 - Kitap Ekle\n");
    printf("3 - Kitap Odunc Ver\n");
    printf("4 - Kitap Iade Al\n");
    printf("5 - Kitaplari Listele\n");
    printf("6 - Uyeleri Listele\n");
    printf("7 - Cikis\n");
    printf("Seciminiz: ");
}

/* ------ DOSYADAN OKU -------- */

void dosyadanOku() {
    FILE *fuye = fopen("uyeler.txt", "r");
    FILE *fkitap = fopen("kitaplar.txt", "r");
    char satir[200];

    if (fuye) {
        while (fgets(satir, sizeof(satir), fuye)) {
            sscanf(satir, "%d|%[^\n]",
                   &uyeler[uye_sayisi].id,
                   uyeler[uye_sayisi].ad);
            uye_sayisi++;
        }
        fclose(fuye);
    }

    if (fkitap) {
        while (fgets(satir, sizeof(satir), fkitap)) {
            sscanf(satir, "%d|%[^|]|%[^|]|%[^|]|%d|%d",
                   &kitaplar[kitap_sayisi].id,
                   kitaplar[kitap_sayisi].ad,
                   kitaplar[kitap_sayisi].yazar,
                   kitaplar[kitap_sayisi].isbn,
                   &kitaplar[kitap_sayisi].durum,
                   &kitaplar[kitap_sayisi].alanUyeId);
            kitap_sayisi++;
        }
        fclose(fkitap);
    }
}

/* ------ DOSYAYA YAZ ------- */

void dosyayaYaz() {
    FILE *fuye = fopen("uyeler.txt", "w");
    FILE *fkitap = fopen("kitaplar.txt", "w");
    int i;

    for (i = 0; i < uye_sayisi; i++) {
        fprintf(fuye, "%d|%s\n", uyeler[i].id, uyeler[i].ad);
    }

    for (i = 0; i < kitap_sayisi; i++) {
        fprintf(fkitap, "%d|%s|%s|%s|%d|%d\n",
                kitaplar[i].id,
                kitaplar[i].ad,
                kitaplar[i].yazar,
                kitaplar[i].isbn,
                kitaplar[i].durum,
                kitaplar[i].alanUyeId);
    }

    fclose(fuye);
    fclose(fkitap);
}

/* ------1. UYE EKLE ------ */

void uyeEkle() {
    struct Uye u;
    u.id = uye_sayisi + 1;

    printf("Uye adi: ");
    fgets(u.ad, sizeof(u.ad), stdin);
    u.ad[strcspn(u.ad, "\n")] = '\0';

    uyeler[uye_sayisi++] = u;
    printf("Uye eklendi. ID: %d\n", u.id);
}

/* ------2. KITAP EKLE ------- */

void kitapEkle() {
    struct Kitap k;
    k.id = kitap_sayisi + 1;
    k.durum = 0;
    k.alanUyeId = 0;

    printf("Kitap adi: ");
    fgets(k.ad, sizeof(k.ad), stdin);
    k.ad[strcspn(k.ad, "\n")] = '\0';

    printf("Yazar adi: ");
    fgets(k.yazar, sizeof(k.yazar), stdin);
    k.yazar[strcspn(k.yazar, "\n")] = '\0';

    printf("ISBN: ");
    fgets(k.isbn, sizeof(k.isbn), stdin);
    k.isbn[strcspn(k.isbn, "\n")] = '\0';

    kitaplar[kitap_sayisi++] = k;
    printf("Kitap basariyla eklendi. ID: %d\n", k.id);
}

/* -----3. KITAP ODUNC ------ */

void kitapOduncVer() {
    int kitapId, uyeId, i;

    printf("Kitap ID: ");
    scanf("%d", &kitapId);
    printf("Uye ID: ");
    scanf("%d", &uyeId);
    getchar();

    for (i = 0; i < kitap_sayisi; i++) {
        if (kitaplar[i].id == kitapId) {
            if (kitaplar[i].durum == 0) {
                kitaplar[i].durum = 1;
                kitaplar[i].alanUyeId = uyeId;
                printf("Kitap odunc verildi.\n");
            } else {
                printf("Kitap zaten oduncte!\n");
            }
            return;
        }
    }
    printf("Kitap bulunamadi!\n");
}

/* ------4. KITAP IADE AL ------ */

void kitapIadeAl() {
    int kitapId, i;

    printf("Iade edilecek kitap ID: ");
    scanf("%d", &kitapId);
    getchar();

    for (i = 0; i < kitap_sayisi; i++) {
        if (kitaplar[i].id == kitapId) {
            if (kitaplar[i].durum == 1) {
                kitaplar[i].durum = 0;
                kitaplar[i].alanUyeId = 0;
                printf("Kitap iade alindi.\n");
            } else {
                printf("Kitap zaten musait.\n");
            }
            return;
        }
    }
    printf("Kitap bulunamadi!\n");
}

/* -----5. KITAP LISTELE ------- */

void kitapListele() {
    int i;
    printf("\n--- KITAPLAR ---\n");

    for (i = 0; i < kitap_sayisi; i++) {
        printf("ID:%d | %s | %s | ISBN:%s | %s",
               kitaplar[i].id,
               kitaplar[i].ad,
               kitaplar[i].yazar,
               kitaplar[i].isbn,
               kitaplar[i].durum ? "Oduncte" : "Musait");

        if (kitaplar[i].durum)
            printf(" | Alan Uye ID: %d", kitaplar[i].alanUyeId);

        printf("\n");
    }
}

/* ------6- UYE LISTELE ------ */

void uyeListele() {
    int i;
    printf("\n--- UYELER ---\n");

    for (i = 0; i < uye_sayisi; i++) {
        printf("ID:%d | %s\n", uyeler[i].id, uyeler[i].ad);
    }
}

