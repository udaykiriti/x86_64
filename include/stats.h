#ifndef STATS_H
#define STATS_H

typedef struct {
    int min;
    int max;
    long long sum;
    double avg;
} IntSummary;

/* Returns 1 on success, 0 for invalid input. */
int calculate_int_summary(const int *arr, int n, IntSummary *out);

/* Returns index of first element > threshold, or -1 if not found/invalid input. */
int find_first_above_threshold(const int *arr, int n, int limit);

#endif
