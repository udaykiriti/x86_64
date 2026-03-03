#include "stats.h"

int calculate_int_summary(const int *arr, int n, IntSummary *out) {
    if (arr == 0 || out == 0 || n <= 0) {
        return 0;
    }

    int min = arr[0];
    int max = arr[0];
    long long sum = arr[0];

    for (int i = 1; i < n; i++) {
        int cur = arr[i];

        if (cur < min) {
            min = cur;
        }
        if (cur > max) {
            max = cur;
        }

        sum += cur;
    }

    out->min = min;
    out->max = max;
    out->sum = sum;
    out->avg = (double)sum / n;

    return 1;
}

int find_first_above_threshold(const int *arr, int n, int limit) {
    if (arr == 0 || n <= 0) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        if (arr[i] > limit) {
            return i;
        }
    }

    return -1;
}
