#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#define KLIENT "/var/sandbox/klientfifo"
#define BUFOR 256

int main(int argc, char *argv[]) {
	
	// Zabezpieczenie przed niepoprawnym wywołaniem
    if (argc < 2) {
        printf("./klient $HOME\n");
        return 1;
    }
    // Zmienne
    char sciezka[BUFOR] = {}, odpowiedz[BUFOR] = {}, wiadomosc[BUFOR] = {};
    int dlugosc, id, serwer_fifo, klient_fifo;
    
    // Stworzenie sciezki do kolejki fifo serwera
    strcat(sciezka,argv[1]);
    strcat(sciezka,"/serwerfifo");

	// Pobieranie ID
    printf("Podaj ID: ");
    scanf("%i",&id);
    
    // Stworzenie wiadomosci dla serwera
    wiadomosc[0] = strlen(argv[1])+1;
    wiadomosc[1] = id;
    strcat(wiadomosc, argv[1]);

	// Wyslanie wiadomosci do serwera
    mkfifo(KLIENT,0777);
    serwer_fifo = open(sciezka, O_WRONLY);
    write(serwer_fifo, wiadomosc, strlen(wiadomosc));
    close(serwer_fifo);

	// Pobieranie wiadomosci z serwera
    klient_fifo = open(KLIENT, O_RDONLY);
    read(klient_fifo, odpowiedz, 1);
    dlugosc = odpowiedz[0];
    read(klient_fifo, odpowiedz, dlugosc);
    printf("Odpowiedz: %s\n", odpowiedz);
    close(klient_fifo);
    unlink(KLIENT);
    return 0;

}
