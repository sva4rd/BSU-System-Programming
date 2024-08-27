#!/bin/bash
count=0
trap "exit" SIGINT
while true; do
    echo $count
    sleep 2
    ((count++))
    if [ $count -eq 5 ]; then
        count=0
    fi
done

