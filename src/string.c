#include "string.h"

int str_len(const char *str) {
    if (str == 0) {
        return 0;
    }

    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
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
        char temp_char = *start;
        *start = *end;
        *end = temp_char;
        start++;
        end--;
    }
}
