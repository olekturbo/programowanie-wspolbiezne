#include <fcntl.h> 
#include <unistd.h> 
#include <stdio.h>
#define DLUGOSC_WIADOMOSCI 256
#define UPRAWNIENIA 0711

void wyczyscBufor() {
		unlink("lockfile");
		unlink("dane.txt");
}

void czytaj() {
   int dane = 0;
   char pytanie[DLUGOSC_WIADOMOSCI] = "";
   char uzytkownik[64];
   getlogin_r(uzytkownik, sizeof(uzytkownik)-1);
 
   while((dane = open("dane.txt", O_RDWR, UPRAWNIENIA)) == -1) {}
	
   if(dane != 0)
   {
		while(read(dane,pytanie,DLUGOSC_WIADOMOSCI) == 0) {}	
		printf("NOWA WIADOMOSC!\n");
		printf("Klient (%s): %s" , uzytkownik, pytanie);
	}
}

void pisz() {
	int wyniki;
	char odpowiedz[DLUGOSC_WIADOMOSCI] = "";
	char c;
	int i;
	
	while(1) {
		wyniki = open("wyniki.txt", O_RDWR|O_CREAT, UPRAWNIENIA);
		if(wyniki != 0) {
			i = 0;
			printf("\nOdpowiedz serwera:\n");
				
			while((c = getchar()) != 27) {
				odpowiedz[i] = c;
				i++;
			}
				
			write(wyniki,odpowiedz,i);
			close(wyniki);
			break;
		}
	}
}

int main() {	
	
	printf("### START SERWERA ###\n");
	while(1) {
		czytaj();
		pisz();
		wyczyscBufor();
	}
		
	return 0;
}

