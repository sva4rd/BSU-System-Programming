# Файл Makefile.am для генерации Makefile

AUTOMAKE_OPTIONS = foreign

# Установка переменных
bin_PROGRAMS = prog # Имя исходного файла

# Указание исходных файлов
prog_SOURCES = src/main.c src/libreplacing.c

# Указание директории для заголовочных файлов
AM_CPPFLAGS = -Iinclude

# Цель для сборки динамической библиотеки
lib_LTLIBRARIES = libreplacing.la

# Указание исходных файлов для библиотеки
libreplacing_la_SOURCES = src/libreplacing.c 

# Установка флагов компилятора и опций линковки для библиотеки
libreplacing_la_CFLAGS = -fPIC -Iinclude
libreplacing_la_LDFLAGS = -shared -Lsrc
