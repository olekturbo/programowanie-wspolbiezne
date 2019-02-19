
#!/bin/bash

echo "Podaj liczbe:"
read liczba
regex='^[0-9]+$'

if ! [[ $liczba =~ $regex ]] ; then
   echo "To nie jest liczba"
   exit 1
fi

echo $liczba > dane.txt

while true; do
	wynik=$(cat wynik.txt 2> /dev/null)
	if [ $wynik ]; then
		echo "" > wynik.txt
		echo "Wynik: $wynik"
		break
	fi
done
