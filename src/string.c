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
    if (str == 0 || str[0] == '\0' || str[1] == '\0') {
        return;
    }

    int length = str_len(str);
    int start = 0;
    int end = length - 1;

    while (start < end) {
        char temp_char = str[start];
        str[start] = str[end];
        str[end] = temp_char;

        start++;
        end--;
    }
}
