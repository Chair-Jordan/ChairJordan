#include "window.h"

int32_t window_avg(int32_t *win, int size) {
  int32_t sum = 0;
  for (int i = 0; i < size; i++)
    sum += win[i];
  return sum/size;
}

void window_insert(int32_t *win, int size, int32_t value) {
  for (int i = 1; i < size; i++)
    win[i - 1] = win[i];
  win[size - 1] = value;
}

void window_clear(int32_t *win, int size) {
  for (int i = 0; i < size; i++)
    win[i] = 0;
}

