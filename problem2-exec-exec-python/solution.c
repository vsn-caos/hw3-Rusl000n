#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Программе на стандартный поток ввода задается арифметическое выражение
// в синтаксисе языка python3. Необходимо вычислить это выражение и вывести результат.
// Использовать дополнительные процессы запрещено — нужно использовать exec.

int main(void) {
    // TODO: прочитайте выражение из stdin,
    //       затем вызовите execvp/execlp для запуска python3,
    //       который вычислит и выведет результат.
    //       Подсказка: python3 -c "print(<выражение>)"

    char expression[4096];
    if (!fgets(expression, sizeof(expression), stdin)) {
        fprintf(stderr, "No input provided\n");
        return 1;
    }
    size_t len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') {
        expression[len - 1] = '\0';
    }
    char python_code[4200];
    snprintf(python_code, sizeof(python_code), "print(%s)", expression);
    execlp("python3", "python3", "-c", python_code, (char*)NULL);
    perror("execlp: python3");
    return 1;
}
