#-------------------------------------------------------------------
'''A. Пингвины

Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Напишите программу, которая по данному числу N от 1 до 9 выводит на экран N пингвинов. Изображение одного пингвина имеет размер 5×9 символов, между двумя соседними пингвинами также имеется пустой (из пробелов) столбец. Разрешается вывести пустой столбец после последнего пингвина. Для упрощения рисования скопируйте пингвина из примера в среду разработки.
Формат ввода

Вводится натуральное число.
Формат вывода

Выведите ответ на задачу.
Пример 1

Ввод	Вывод
3
   _~_       _~_       _~_    
  (o o)     (o o)     (o o)   
 /  V  \   /  V  \   /  V  \  
/(  _  )\ /(  _  )\ /(  _  )\ 
  ^^ ^^     ^^ ^^     ^^ ^^   
Пример 2

Ввод	Вывод
1
   _~_    
  (o o)   
 /  V  \  
/(  _  )\ 
  ^^ ^^   
Примечания

Учтите, что вывод данных на экран производится построчно, а не попингвинно.
Не забудьте, что для вывода бекслеша надо написать два бекслеша подряд.
'''

def draw_snowman(number):
    snowman_details = list(['   _~_   ', '  (o o)  ', ' /  V  \\ ',
                            '/(  _  )\\', '  ^^ ^^  '])
    for detail in snowman_details:
        for i in range(number):
            print(detail, end=' ')
        print()


draw_snowman(int(input()))

#-------------------------------------------------------------------
'''
B. Палиндромы

Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Вася продолжает изучать палиндромы и искать их вокруг себя в повседневной жизни,
вот только нигде их не находит. Тогда он решает, что не стоит искать палиндромы,
когда их можно делать! Вася задался вопросом: какое минимальное количество
символов нужно приписать к строке слева или справа так, чтобы она стала палиндромом.
Формат ввода

Строка, состоящая из строчных латинских символов, длина строки не превосходит 1000.
Формат вывода

Выведите минимальное количество символов, которое необходимо дописать.
Пример 1

Ввод	Вывод
abc
2
Пример 2

Ввод	Вывод
aba
0
'''

def size_of_palindrome_extension(str):

    if len(str) % 2 == 0:
        ans1 = for_even_words(str)
    else:
        ans1 = for_odd_words(str)
    str = str[::-1]
    if len(str) % 2 == 0:
        ans2 = for_even_words(str)
    else:
        ans2 = for_odd_words(str)
    return min(ans1, ans2)


def for_odd_words(str):
    assert len(str) % 2 == 1
    cursor = len(str) >> 1
    if for_odd_if_right_part_pal(str, cursor):
        return 0
    cursor += 1
    for i in range(len(str) - cursor):
        if for_even_if_right_part_pal(str, cursor):
            return i * 2 + 1
        if for_odd_if_right_part_pal(str, cursor):
            return i * 2 + 2
        cursor += 1


def for_even_words(str):
    assert len(str) % 2 == 0
    cursor = len(str) >> 1
    for i in range(len(str) - cursor):
        if for_even_if_right_part_pal(str, cursor):
            return i * 2
        if for_odd_if_right_part_pal(str, cursor):
            return i * 2 + 1
        cursor += 1
    return 0


def for_odd_if_right_part_pal(str, cursor):
    assert cursor * 2 - len(str) + 1 >= 0
    for j in range(1, len(str) - cursor):
        if str[cursor - j] != str[cursor + j]:
            return False
    return True


def for_even_if_right_part_pal(str, cursor):
    assert 2 * cursor - len(str) >= 0
    for j in range(len(str) - cursor):
        if str[cursor + j] != str[cursor - j - 1]:
            return False
    return True


print(size_of_palindrome_extension(input()))


#-------------------------------------------------------------------
'''
C. git log

Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Напишите программу для переформатирования вывода команды git log.
Формат ввода

На вход программе подаётся лог с описанием коммитов, 
каждая строка которого отвечает формату: <sha-1>\t<date>\t<author>\t<email>\t<message>
Формат вывода

На выход нужно подать строки лога в формате: <первые 7 символов sha-1>...<message>. 
Длина строки должна быть 80 символов. Предполагается, что вход всегда корректный.
Пример

Ввод	Вывод
0cd8619f18d8ecad1e5d2303f95ed206c2d6f92b	Fri Sep 23 10:59:32 2016 -0700	
Brett Cannon	brettcannon@users.noreply.github.com	Update PEP 512 (#107)

94dbee096b92f10ab83bbcf88102c6acdc3d76d1	Thu Sep 22 12:39:58 2016 +0100	
Thomas Kluyver	takowl@gmail.com	Update PEP 517 to use pyproject.toml from PEP 518 (#51)


0cd8619....................................................Update PEP 512 (#107)
94dbee0..................Update PEP 517 to use pyproject.toml from PEP 518 (#51)
Примечания

Указание: используйте строковый метод format https://docs.python.org/3.5/library/string.html#formatstrings
'''

def funk(line):
    massage = line.split('\t')[-1]
    return '{}{:.>73}'.format(line[:7], massage)


while True:
    try:
        print(funk(input()))
    except (ValueError, EOFError):
        break

#-------------------------------------------------------------------
'''
D. Сила пароля

Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Студент Петя в совершенстве отточил навык придумывания новых паролей и теперь
объясняет студентке Ане какие пароли можно использовать, а какие нет. 
Помогите ему написать программку для оценки силы аниного пароля.
Правила простые: сильным может считаться только тот пароль, 
в котором есть буквы в разных регистрах и цифры. Пароли длины менее 8,
пароли состоящие менее чем из 4 уникальных символов,
и пароли в которых встречается слово anna (в любом регистре букв) считаются слабыми.
Формат ввода

Непустая строка с паролем
Формат вывода

строка 'weak' или 'strong'
Пример

Ввод	Вывод
Anna12345
weak
'''

import re


def password_strength(str):
    fff = 0
    if check_letters_in_diff_registers(str) \
            and check_digits(str) \
            and check_length(str) \
            and check_unique_symbols_number(str)\
            and check_anna_in_pass(str):
        return 'strong'
    else:
        return 'weak'


def check_letters_in_diff_registers(password):
    lower_reg = re.findall('[a-z]', password)
    upper_reg = re.findall('[A-Z]', password)
    return lower_reg and upper_reg


def check_digits(password):
    return bool(re.findall('[0-9]', password))


def check_length(password):
    return len(password) >= 8


def check_unique_symbols_number(password):
    return len(set(password)) >= 4


def check_anna_in_pass(password):
    return 'anna' not in password.lower()


print(password_strength(input()))


#-------------------------------------------------------------------
'''
E. Непонятная задача

Ограничение времени	1 секунда
Ограничение памяти	64Mb
Ввод	стандартный ввод или input.txt
Вывод	стандартный вывод или output.txt
Вам дан неизвестный файл import_me.py. Он лежит в одной директории с вашим решением. Решите задачу, импортировав его.
Решение должно иметь имя solution.py.
Подсказка: функция help() недоступна, хотя она пригодилась бы. Чем бы её можно заменить?
И загляните в лог компиляции, когда ваше решение будет падать, там может оказаться что-то полезное :)
В этой задаче выбирайте компилятор Make, в остальных – Python.
Ещё подсказка: в успешном решении не должно быть вызовов print(), мы всё сделаем за вас.


stdout:
'/temp/compiling/solution.py' cannot be extracted via extract ()
/usr/bin/python3.4 -S executor.py
Поздравляю. Тут лежит условие задачи. :)

В этом Python-модуле объявлено несколько переменных и функций.
Больше ничего нет.

Нужно написать функцию get_all_functions(), которая будет
возвращать список названий всех _функций_, объявленных в этом файле.
Вложенные функции учитывать не надо. Порядок имён функций не важен.

Например, для файла:
    universal_answer = 42
    def ask(question):
        return universal_answer
    def help():
        return ask('Help')
ответом будет считаться ['ask', 'help'].

Подсказка: getattr(module, function_or_var_name) возвращает
объект функции (или переменной).
А ещё вам пригодится встроенная функция callable.

4


stderr:
'''

import import_me


def get_all_functions():
    return [name for name in dir(import_me) if callable(getattr(import_me, name))]

