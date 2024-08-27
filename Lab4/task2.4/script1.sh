#!/bin/bash

while true; do
    read -p "Введите имя пользователя: " username
    if id -u "$username" >/dev/null 2>&1; then
        uid=$(id -u "$username")
        groups=$(id -Gn "$username")
        primary_group=$(id -gn "$username")
        echo "UID пользователя $username: $uid"
        echo "Группы пользователя $username: $groups"
        echo "Основная группа пользователя $username: $primary_group"
        break
    else
        echo "$0: пользователь $username не зарегистрирован в системе." >&2
    fi
done

