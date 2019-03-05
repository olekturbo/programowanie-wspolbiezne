#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#define DLUGOSC_WIADOMOSCI 256
#define UPRAWNIENIA 0777

void sprawdzDostepnosc() {	
	while (open("lockfile",O_CREAT|O_EXCL,0)==-1) {
       printf("Serwer zajety, prosze czekac\n\n");	
       sleep(1);
    }
}

void wyczyscBufor() {
	unlink("wyniki.txt");
}

void czytaj() {
   int wyniki = 0;
   char odpowiedz[DLUGOSC_WIADOMOSCI] = "";
 
   while((wyniki = open("wyniki.txt", O_RDWR, UPRAWNIENIA)) == -1) {}
	
   if(wyniki != 0) {
		while(read(wyniki,odpowiedz,DLUGOSC_WIADOMOSCI) == 0) {}	
		printf("ODPOWIEDZ SERWERA!\n");
		printf("Serwer: %s" , odpowiedz);
   }
}

void pisz(char *uzytkownik) {
	int dane;
	char pytanie[DLUGOSC_WIADOMOSCI] = "";
	char c;
	int i;
	
	while(1) {
		dane = open("dane.txt", O_RDWR|O_CREAT, UPRAWNIENIA);
		if(dane != 0) {
			i = 0;
			printf("\nPytanie klienta:\n");
				
			while((c = getchar()) != 27) {
				pytanie[i] = c;
				i++;
			}
			pytanie[i] = '-';
			i++;
			
			write(dane,pytanie,i);
			write(dane,uzytkownik, DLUGOSC_WIADOMOSCI);
			close(dane);
			break;
		}
	}
}

int main( int argc, char *argv[] ) {
	sprawdzDostepnosc();
	pisz(argv[1]);
	czytaj();
	wyczyscBufor();

    return 0;
}
