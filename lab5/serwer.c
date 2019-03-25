#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define SERWER "/var/sandbox/serwerfifo"
#define BUFOR 256

typedef struct {
    int ID;
    char nazwisko[20];
} osoba;

char odpowiedz[BUFOR] = {};

void czysc() {
    int i;
    for (i=0;i<BUFOR;i++) odpowiedz[i] = '\0';
}

int main() {
	// Zmienne
    osoba baza[20] = {};
    int i = 0;
    char pytanie[BUFOR] = {}, sciezka[BUFOR] = {};
    char nazwisko[20];
    int id, dlugosc, znaleziono, serwer_fifo, klient_fifo;
    
    // Odczyt bazy z pliku
	FILE *dane = fopen("dane.txt", "r");
    while(fscanf(dane,"%d %s\n",&baza[i].ID, baza[i].nazwisko) != EOF) { i++; }
    fclose(dane);

	// Stworzenie serwerowej kolejki fifo
    mkfifo(SERWER, 0777);

    printf("--------------------\n");
	printf("| ID | SCIEZKA\n");
    while(1) {
		// Odczyt dlugosci, id i sciezki klienta z pliku
        serwer_fifo = open(SERWER, O_RDONLY);
        read(serwer_fifo, pytanie, 2);
        dlugosc = pytanie[0];
        id = pytanie[1];
        read(serwer_fifo, pytanie, dlugosc);
        printf("| %i | %s\n", id, pytanie);
        
        // Wyszukiwanie czy dane ID znajduje się w bazie
        znaleziono = 0;
        for (i = 0; i < 20; i++) {
            if (baza[i].ID == id) {
                strcpy(nazwisko,baza[i].nazwisko);
                znaleziono = 1;
                break;
            }
        }
        if (!znaleziono) {
            strcpy(nazwisko,"Nie ma");
        }
        
        // Czyszczenie odpowiedzi
        czysc();
        
        // Odpowiedz dla klienta
        odpowiedz[0] = strlen(nazwisko);
        strcat(odpowiedz, nazwisko);
        strcpy(sciezka,"");
        strcat(sciezka, pytanie);
        strcat(sciezka, "/klientfifo");
        klient_fifo = open(sciezka, O_WRONLY);
        write(klient_fifo, odpowiedz, strlen(odpowiedz));
        close(klient_fifo);
    }

    return 0;
}
