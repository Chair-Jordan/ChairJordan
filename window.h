#include <stdint.h>

int32_t window_avg(int32_t *win, int size);
void window_insert(int32_t *win, int size, int32_t value);
void window_clear(int32_t *win, int size);
