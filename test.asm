; Программа для демонстрации разбора строк
DATA:                               ; Секция данных
    message: DB 'Hello, World!'     ; Объявление строки
CODE:                               ; Секция кода
BEGIN:                              ; Начало программы
            mov    ah,9h
            mov    dx,message
            int    21h
            ret
END: CODE
END: BEGIN