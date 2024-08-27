#!/bin/bash

min_size="$1"
max_size="$2"
directory="$3"

if [[ -z "$min_size" || -z "$max_size" ]]; then
    echo "$0: значения min_size и max_size должны быть указаны."
    exit 1
fi

if ! [[ "$min_size" =~ ^[0-9]+$ && "$max_size" =~ ^[0-9]+$ ]]; then
    echo "$0: значения min_size и max_size должны быть неотрицательными числами."
    exit 1
fi

if [ "$min_size" -lt 0 ]; then
    echo "$0: значение min_size должно быть больше или равно нулю."
    exit 1
fi


if [ "$max_size" -lt "$min_size" ]; then
    echo "$0: значение max_size должно быть не меньше значения min_size."
    exit 1
fi

if [ ! -d "$directory" ]; then
    echo "$0: директория '$directory' не существует."
    exit 1
fi

find "$directory" -type f -size +"$min_size"c -size -"$max_size"c
