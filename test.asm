; Программа для демонстрации разбора строк

SECTION .data                       ; Секция данных
    message DB 'Hello, World!', 0   ; Объявление строки

SECTION .text                       ; Секция кода
GLOBAL BEGIN                       ; Глобальная метка _start

BEGIN:                             ; Начало программы
    ; Вывод строки
    mov     eax, 4                  ; Код системного вызова для вывода
    mov     ebx, 1                  ; Файловый дескриптор (stdout)
    mov     ecx, message            ; Указатель на сообщение
    mov     edx, 13                 ; Длина сообщения
    int     0x80                    ; Вызов ядра

    ; Завершение программы
    mov     eax, 1                  ; Код системного вызова для выхода
    xor     ebx, ebx                ; Код завершения
    int     0x80                    ; Вызов ядра
