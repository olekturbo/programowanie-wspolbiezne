#!/bin/bash

while true; do
	dane=$(cat dane.txt)

	if [ $dane ]; then
		wynik=$(((dane * dane)+dane))
		echo $wynik > wynik.txt
		echo "" > dane.txt
	fi
done
