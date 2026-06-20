#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

// Программе передаются два аргумента: имя файла и строка для поиска.
// Необходимо найти все вхождения строки в текстовом файле,
// используя отображение на память с помощью системного вызова mmap.
// На стандартный поток вывода вывести список всех позиций (с нуля),
// где встречается искомая строка, по одной на строку.

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <search_string>\n", argv[0]);
        return 1;
    }

    // TODO: откройте файл, получите его размер через fstat,
    //       отобразите в память через mmap,
    //       найдите все вхождения argv[2] и выведите их позиции

    const char *filename = argv[1];
    const char *needle = argv[2];
    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return 0;
    }

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    struct stat st;
    if (fstat(fd, &st) != 0) {
        perror("fstat");
        close(fd);
        return 1;
    }
    off_t file_size = st.st_size;
    if (file_size == 0) {
        close(fd);
        return 0;
    }
    char *mapped = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    close(fd);

    const char *ptr = mapped;
    size_t offset = 0;
    while (offset + needle_len <= (size_t)file_size) {
        const char *found = memmem(ptr, file_size - offset, needle, needle_len);
        if (!found) break;
        size_t pos = found - mapped;
        printf("%zu\n", pos);
        offset = pos + 1;
        ptr = mapped + offset;
    }
    munmap(mapped, file_size);
    return 0;
}
