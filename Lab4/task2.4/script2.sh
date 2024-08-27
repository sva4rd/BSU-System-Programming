#!/bin/bash

filename="$1"
permission_mask="$2"

if [[ -z "$filename" ]]; then
    echo "$0: имя файла не может быть пустым."
    exit 1
fi

if ! [[ "$permission_mask" =~ ^[0-7]{3}$ ]]; then
    echo "$0: некорректное разрешение, введите разрешение в формате трех цифр от 0 до 7"
    exit 1
fi

umask_orig=$(umask)
umask "$permission_mask"
touch "$filename"
umask "$umask_orig"

