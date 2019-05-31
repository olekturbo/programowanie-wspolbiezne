#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <signal.h>
#define ILOSC_SLOW 5

typedef struct {
	long typ;
	char dane[256];
} WIADOMOSC;

typedef struct {
	char *slowoPL;
	char *slowoANG;
} SLOWNIK;

int input, output;

void zaladujSlownik(SLOWNIK* slownik) {
	slownik[0].slowoPL = "monitor";
	slownik[0].slowoANG = "screen";
	slownik[1].slowoPL = "bluza";
	slownik[1].slowoANG = "hoodie";
	slownik[2].slowoPL = "noga";
	slownik[2].slowoANG = "leg";
	slownik[3].slowoPL = "woda";
	slownik[3].slowoANG = "water";
	slownik[4].slowoPL = "ucho";
	slownik[4].slowoANG = "ear";
}

int znajdz(SLOWNIK* slownik, const char *slowo) {

	int i;
	
	for(i = 0; i < ILOSC_SLOW; i++) {
		if(strcmp(slownik[i].slowoPL, slowo) == 0) {
			return i;
		}
	}
	return 0;
}

void sighandler() {
	msgctl(input, IPC_RMID, 0);
	msgctl(output, IPC_RMID, 0);
	exit(0);
}

int main() {
    WIADOMOSC wiadomosc;
	SLOWNIK slownik[ILOSC_SLOW];

	zaladujSlownik(slownik);

	int key = 1111;
	int key2 = 2222;

    input = msgget(key, 0777|IPC_CREAT);
	output = msgget(key2, 0777|IPC_CREAT);
	
	signal(SIGUSR1, sighandler);
	
	while(1) {
      	if(msgrcv(input, &wiadomosc, 255, 0, 0) > 0) {
			int wynik = znajdz(slownik, wiadomosc.dane);
			if(wynik > 0){
				strcpy(wiadomosc.dane, slownik[wynik].slowoANG);
        	}
			else {
				strcpy(wiadomosc.dane, "Nie znam takiego slowa!");
			}
			msgsnd(output, &wiadomosc, 255, 0);
		}
	}
	
	
	return 0;
} 
