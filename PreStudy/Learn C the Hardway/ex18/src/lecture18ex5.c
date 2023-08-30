#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Our old friend die from ex17
void die(const char *message) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

// A typedef creates a fake type, in this
// case for a function pointer
typedef int (*compare_cb)(int a, int b);

typedef int *(*custom_sort)(int *numbers, int count, compare_cb cmp);

// A classic bubble sort function that uses the
// compare_cb to do the sorting
int *bubble_sort(int *numbers, int count, compare_cb cmp) {
  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if (!target) {
    die("Memory error.");
  }

  memcpy(target, numbers, count * sizeof(int));

  for (i = 0; i < count; i++) {
    for (j = 0; j < count - 1; j++) {
      if (cmp(target[j], target[j + 1]) > 0) {
        temp = target[j + 1];
        target[j + 1] = target[j];
        target[j] = temp;
      }
    }
  }

  return target;
}

void quick_sort_internal(int *target, int low, int high, compare_cb cmp);
int quick_sort_partition(int *target, int low, int high, compare_cb cmp);

int *quick_sort(int *numbers, int count, compare_cb cmp) {
  int *target = malloc(count * sizeof(int));

  if (!target) {
    die("Memory error.");
  }

  memcpy(target, numbers, count * sizeof(int));

  quick_sort_internal(target, 0, count - 1, cmp);

  return target;
}

void quick_sort_internal(int *target, int low, int high, compare_cb cmp) {
  if (low < high) {
    int pivot = quick_sort_partition(target, low, high, cmp);

    quick_sort_internal(target, low, pivot - 1, cmp);
    quick_sort_internal(target, pivot + 1, high, cmp);
  }
}

int quick_sort_partition(int *target, int low, int high, compare_cb cmp) {
  int temp = 0;
  int pivot_value = target[(low + high) / 2];

  while (low < high) {
    while (cmp(target[low], pivot_value) < 0) {
      low++;
    }

    while (cmp(target[high], pivot_value) > 0) {
      high--;
    }

    if (low < high) {
      temp = target[low];
      target[low] = target[high];
      target[high] = temp;
    }
  }

  return high;
}

int sorted_order(int a, int b) { return a - b; }

int reverse_order(int a, int b) { return b - a; }

int strange_order(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  } else {
    return a % b;
  }
}

// Used to test that we are sorting things correctly
// by doing the sort and printing it out.
void test_sorting(int *numbers, int count, compare_cb cmp, custom_sort sort) {
  int i = 0;
  int *sorted = sort(numbers, count, cmp);

  if (!sorted) {
    die("Failed to sort as requested.");
  }

  for (i = 0; i < count; i++) {
    printf("%d ", sorted[i]);
  }

  printf("\n");

  free(sorted);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    die("USAGE: ex18 4 3 1 5 6");
  }

  int count = argc - 1;
  int i = 0;
  char **inputs = argv + 1;

  int *numbers = malloc(count * sizeof(int));
  if (!numbers) {
    die("Memory error.");
  }

  for (i = 0; i < count; i++) {
    numbers[i] = atoi(inputs[i]);
  }

  printf("Bubble sort:\n");

  test_sorting(numbers, count, sorted_order, bubble_sort);
  test_sorting(numbers, count, reverse_order, bubble_sort);
  test_sorting(numbers, count, strange_order, bubble_sort);

  printf("Quick sort:\n");
  test_sorting(numbers, count, sorted_order, quick_sort);
  test_sorting(numbers, count, reverse_order, quick_sort);
  test_sorting(numbers, count, strange_order, quick_sort);

  free(numbers);

  return 0;
}
