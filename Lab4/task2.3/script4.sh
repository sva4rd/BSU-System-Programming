#!/bin/bash

dir=$1
files=()

for file in $(ls $dir); do
    if [ -f "$dir/$file" ]; then
        if [ $(stat -c '%a' "$dir/$file") -eq 755 ]; then
            files+=("$file")
        fi
    fi
done

echo "Файлы с правами доступа 755:"
for file in "${files[@]}"; do
    echo "$file"
done
