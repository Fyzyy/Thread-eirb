#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

#define ARRAY_SIZE 1000000
#define THREAD_COUNT 4

typedef struct {
    int* array;
    int low;
    int high;
} ThreadData;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* array, int low, int high) {
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);
    return i + 1;
}

void quicksort(int* array, int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quicksort(array, low, pi - 1);
        quicksort(array, pi + 1, high);
    }
}

void* quicksort_thread(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    quicksort(data->array, data->low, data->high);
    return NULL;
}

void merge(int* array, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = (int*) malloc(n1 * sizeof(int));
    int* R = (int*) malloc(n2 * sizeof(int));

    for (int i = 0; i < n1; i++)
        L[i] = array[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = array[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        } else {
            array[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergesort(int* array, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergesort(array, left, mid);
        mergesort(array, mid + 1, right);
        merge(array, left, mid, right);
    }
}

void* mergesort_thread(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    mergesort(data->array, data->low, data->high);
    return NULL;
}

int main() {
    int* array = (int*) malloc(ARRAY_SIZE * sizeof(int));
    if (!array) {
        fprintf(stderr, "Erreur lors de l'allocation de la mémoire\n");
        return 1;
    }

    // Initialisation du tableau
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % 10000;
    }

    // Quick sort
    thread_t threads[THREAD_COUNT];
    ThreadData thread_data[THREAD_COUNT];
    int segment_size = ARRAY_SIZE / THREAD_COUNT;

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_data[i].array = array;
        thread_data[i].low = i * segment_size;
        thread_data[i].high = (i == THREAD_COUNT - 1) ? (ARRAY_SIZE - 1) : ((i + 1) * segment_size - 1);
        thread_create(&threads[i], quicksort_thread, &thread_data[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        thread_join(threads[i], NULL);
    }

    // Merge des segments
    for (int size = segment_size; size < ARRAY_SIZE; size *= 2) {
        for (int left = 0; left < ARRAY_SIZE - 1; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < ARRAY_SIZE - 1) ? (left + 2 * size - 1) : (ARRAY_SIZE - 1);
            merge(array, left, mid, right);
        }
    }

    // Vérification de l'état du tableau (trié ou non)
    for (int i = 1; i < ARRAY_SIZE; i++) {
        if (array[i - 1] > array[i]) {
            fprintf(stderr, "Tableau non trié à l'index: %d\n", i);
            free(array);
            return 1;
        }
    }

    printf("Tableau trié.\n");
    free(array);
    return 0;
}
