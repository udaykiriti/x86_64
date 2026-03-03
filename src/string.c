#include "string.h"

int str_len(const char *str) {
    if (str == 0) {
        return 0;
    }

    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void str_rev(char *str) {
    if (str == 0 || str[0] == '\0') {
        return;
    }

    char *start = str;
    char *end = str;

    while (*end != '\0') {
        end++;
    }
    end--;

    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }
}
