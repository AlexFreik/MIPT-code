/* Problem phy06-3: posix/filesystem/manage-symlinks
 * 
 * Программе на стандартном потоке ввода передаётся список имён файлов.
 * 
 * Если файл является символической ссылкой, то нужно вывести абсолютное имя того файла, на который эта ссылка указывает.
 * 
 * Если файл является регулярным, то необходимо создать символическую ссылку в текущем каталоге, приписав в названии файла префикс link_to_.
 * 
 */

#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char *path = malloc((PATH_MAX + 1) * sizeof(char));
    char *new_path = malloc((PATH_MAX + 1) * sizeof(char));
    struct stat st;
    while (NULL != fgets(path, PATH_MAX, stdin)) {
        for (size_t cnt = 0; '\0' != *(path + cnt); ++cnt) {
            if ('\n' == *(path + cnt)) {
                *(path + cnt) = '\0';
                while (cnt > 0 && ' ' == *(path + cnt - 1))  {
                    *(path + --cnt) = '\0';
                }
                break;
            }
        }
        if (-1 == lstat(path, &st)) {
            continue;
        }
        if (S_ISLNK(st.st_mode)) {
            if (NULL != realpath(path, new_path)) {
                printf("%s\n", new_path);
            }
        } else {
            strcpy(new_path, "link_to_");
            char *name = path;
            for (char *cp = path; '\0' != *cp; ++cp) {
                if ('/' == *cp) {
                     name = cp + sizeof(char);
                }
            }
            strcat(new_path, name);
            symlink(path, new_path);
        }
    }
    free(path);
    free(new_path);
    return 0;
}

