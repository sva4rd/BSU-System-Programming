# Функция для удаления дубликатов команд
function remove_duplicates() {
    command = $2
    if (!(command in commands)) {
        print $0 >> "history-copy"
        commands[command]
    } else {
        counter++
    }
}

# Функция для вывода количества удаленных строк
function print_removed() {
    print "Удалено строк: " counter
}

# Главная функция
function main() {
    print "" > "history-copy"
    while ((getline < "history") > 0) {
        remove_duplicates()
    }
    close("history")
    print_removed()
}

BEGIN {
    main()
}
