#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int c;
    int lines = 0, words = 0, chars = 0;
    int in_word = 0;

    FILE *fp = stdin;

    if (argc == 2) {
        fp = fopen(argv[1], "r");
        if (!fp) {
            perror("fopen");
            return 1;
        }
    }

    while ((c = fgetc(fp)) != EOF) {
        chars++;

        if (c == '\n')
            lines++;

        if (isspace(c)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            words++;
        }
    }

    if (fp != stdin)
        fclose(fp);

    printf("%d %d %d\n", lines, words, chars);
    return 0;
}

