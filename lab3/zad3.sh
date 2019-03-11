#!/bin/bash
ilosc=0
procesy=""
bar="-------------------------------------------------------------"

# CZYSZCZENIE KONSOLI
if [ -z "$3" ]; then
	clear
	echo "WYSZUKIWANIE PLIKU $2 W PODDRZEWIE KATALOGÓW O KORZENIU $1"
	echo "$bar"
	echo "WYSTĄPIENIA"
	echo "$bar"
fi

# WALIDACJA POPRAWNOŚCI WYWOŁANIA SKRYPTU
if [ "$#" -lt 2 ]; then
    echo "Prawidłowe wykonanie: $0 katalog plik"
	exit
else
	if [ ! -d "$1" ]; then
		echo "Brak katalogu o nazwie $1"
		exit
	fi
fi

# ZAPAMIĘTANIE PID-ów, WYWOŁANIE PROCESÓW POTOMNYCH ORAZ WYPISANIE ZNALEZIONYCH PLIKÓW
for item in $1/*; do
	if [ -d "$item" ]; then
		"$0" "$item" "$2" "potomny" &
		procesy+=("$!")
	elif [ -f "$item" ]; then
		if [ $(basename $item) == "$2" ]; then
			((ilosc++))
			echo "$1/$2"
		fi
	fi
done

# ZLICZENIE ILOŚCI PLIKÓW NA PODSTAWIE EXIT PROCESÓW POTOMNYCH
for pid in "${procesy[@]}"; do
	if [ "$pid" ]; then
		wait "$pid"
		((ilosc+=("$?")))
	fi
done

# WYPISANIE ILOŚCI PLIKÓW
if [ -z "$3" ]; then
	if [ "$ilosc" -eq 0 ]; then
		echo "Brak pliku $2 w katalogu $1"
	else
		echo "$bar"
		echo "$ilosc wystapien pliku $2 w katalogu $1"
	fi
	echo "$bar"
	echo "PORÓWNANIE Z FUNKCJĄ find"
	echo "$bar"
	find $1 -name "$2"
	ilosc_find=($(find $1 -name "$2" | wc -l))
	echo "$bar"
	echo "$ilosc_find wystąpień pliku $2 w katalogu $1 wedlug funkcji find"
	echo "$bar"
fi

exit "$ilosc"
