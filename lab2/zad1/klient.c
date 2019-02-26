#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void wyczyscBufor() {
	int wyniki = open("wyniki.txt", O_RDWR, O_CREAT);
	char czyszczenie[1] = "";
	write(wyniki, czyszczenie, 1);
	close(wyniki);
}

void czytaj() {
	int dane = open("dane.txt", O_RDWR, O_CREAT);
	char odpowiedz[256] = "";
	read(dane,odpowiedz,256);
	close(dane);
	printf("%s", odpowiedz);
}

void pisz() {
	int wyniki = open("wyniki.txt", O_RDWR, O_CREAT);
	char pytanie[256] = "";
	int i = 0;
	char c;
	printf("Zadaj pytanie:\n");
	while((c = getchar())  != 27) {
		pytanie[i] = c;
		i++;
	}
	write(wyniki, pytanie, 256);
	close(wyniki);
}

void sprawdzDostepnosc() {
	while (open("lockfile",O_CREAT|O_EXCL,0)==-1) {
      printf("Serwer zajety, prosze czekac\n");
      sleep(1);
   }
}

int main(int argc, char *argv[]) {
	wyczyscBufor();
	pisz();
	sprawdzDostepnosc();
	czytaj();
	
}

