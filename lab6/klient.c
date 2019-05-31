#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

typedef struct {
      	long typ;
     	char dane[256];
} WIADOMOSC;

int main(int argc, char * argv[]) {
	if(argc > 1) {
      	WIADOMOSC wiadomosc;
      	int input, output;
      	long pid = getpid();
		int key = 1111;
		int key2 = 2222;
      
      	input = msgget(key, 0777);
		output = msgget(key2, 0777);

      	strcpy(wiadomosc.dane, argv[1]);
		wiadomosc.typ = pid;
		
		msgsnd(input, &wiadomosc, 255, 0);
		printf("PID: (%ld) - %s\n", pid, wiadomosc.dane);
		msgrcv(output, &wiadomosc, 255, wiadomosc.typ, 0);
		printf("PID: (%ld) - %s\n", pid, wiadomosc.dane);
		
		return 0;
	}
	else {
		printf("Brak parametru slowo.");
		return 1;
	}
} 
