#!/bin/bash

fifo_serwer="serwer"
fifo_klient="$1/klient"

if [[ ! -p $fifo_klient ]]; then
    mkfifo $fifo_klient
fi

if [[ ! -p $fifo_serwer ]]; then
    echo "Brak polaczenia z serwerem"
    exit 1
fi

echo $fifo_klient $2 > $fifo_serwer
read odpowiedz < $fifo_klient
echo $odpowiedz


