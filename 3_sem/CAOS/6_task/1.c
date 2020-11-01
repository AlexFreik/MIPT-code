/* Problem phy06-1: files-io/print-list-posix
 * 
 * Программе в аргументе командной строки передается имя файла с бинарными данными в Little-Endian.
 * 
 * Файл хранит внутри себя односвязный список элементов:
 * 
 * struct Item {
 *   int value;
 *   uint32_t next_pointer;
 * };
 * Поле value храние значение элемента списка, поле next_pointer - позицию в файле (в байтах), указывающую на следующий элемент. Признаком последнего элемента является значение next_pointer, равное 0.
 * 
 * Расположение первого элемента списка (если он существует) - строго в нулевой позиции в файле, расположение остальных - случайным образом.
 * 
 * Выведите на экран значения элементов в списке в текстовом представлении.
 * 
 * Для работы с файлом использовать только низкоуровневый ввод-вывод POSIX.
 * 
 */

#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

typedef struct Item {
  int value;
  uint32_t next_pointer;
} Item;

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);
    off_t end = lseek(fd, 0, SEEK_END);
    off_t last_np = lseek(fd, 0, SEEK_SET);
    if (last_np == end) {
        goto finaly;
    }
    last_np = 1;
    Item item;
    while (last_np) {
        if (-1 == read(fd, &item, sizeof(item))) {
            break;
        }
        printf("%d ", item.value);
        last_np = lseek(fd, item.next_pointer, SEEK_SET);
    }
    printf("\n");

finaly:
    return 0;
}
