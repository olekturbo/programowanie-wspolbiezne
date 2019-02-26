#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(){

  int wyniki = open("wyniki.txt", O_RDWR, O_CREAT);
  int dane = open("dane.txt", O_RDWR, O_CREAT);
  char pytanie[256] = "";
  char odpowiedz[256] = "";
  char czyszczenie[1] = "";
  
  while(1) {
	  if(read(wyniki,pytanie,256) != NULL) {
		  printf("Pytanie: %s\n", pytanie);
		  printf("Podaj odpowiedz:\n");
		  scanf("%s", odpowiedz);
		  write(dane, "Odpowiedz: ", 11);
		  write(dane, odpowiedz, 256);
		  printf("\nLockfile usuniety\n");
		  unlink("lockfile"); 
		  write(wyniki,czyszczenie,1);
		  close(wyniki);
	  }
  }
  close(wyniki);
}
