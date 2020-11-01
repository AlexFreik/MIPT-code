/* Problem phy06-2: posix/filesystem/file-list-summ-size
 * 
 * Программе на стандартном потоке ввода передаётся список имён файлов.
 * 
 * Необходимо посчитать суммарный размер в байтах всех файлов, которые являются регулярными.
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

int main(int argc, char *argv[]) {
    char *path = malloc((PATH_MAX + 1) * sizeof(char));
    uint64_t sizes_summ;
    struct stat st;
    while (fgets(path, PATH_MAX, stdin)) {
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
        if (S_ISREG(st.st_mode)) {
            sizes_summ += st.st_size;
        }
    }
    printf("%" PRIu64 "\n", sizes_summ);
    free(path);
    return 0;
}

