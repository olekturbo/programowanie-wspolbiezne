#!/bin/bash

fifo_serwer="serwer"

if [[ "$1" ]]; then
        if [[ ! -p $1 ]]; then
                echo "Brak polaczenia z klientem"
        else
                echo "$(($2 + 5))" > $1
        fi
else
        trap "" SIGTERM
        trap "" SIGHUP
        trap "exit 0" SIGUSR1

        if [[ ! -p $fifo_serwer ]]; then
                mkfifo $fifo_serwer
        fi

        while true
        do
                if read zapytanie < $fifo_serwer; then
                        $0 $zapytanie &
                fi
        done
fi





