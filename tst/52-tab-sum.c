#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"

#define ARRAY_SIZE 1000000  // Taille du grand tableau
#define NUM_THREADS 8       // Nombre de threads

// Structure pour passer les arguments aux threads
typedef struct {
    int *array;
    long start;
    long end;
    long long partial_sum;
} ThreadData;

// Fonction de chaque thread pour calculer la somme partielle
void *calculate_partial_sum(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    data->partial_sum = 0;
    for (long i = data->start; i < data->end; i++) {
        data->partial_sum += data->array[i];
    }
    return NULL;
}

int main() {
    int *array = (int *)malloc(ARRAY_SIZE * sizeof(int));
    thread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // Initialisation du tableau à 1 pour chaque case
    for (long i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 1;
    }

    // Déterminer les segments de travail pour chaque thread
    long segment_size = ARRAY_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].array = array;
        thread_data[i].start = i * segment_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * segment_size;
        thread_data[i].partial_sum = 0;
        thread_create(&threads[i], calculate_partial_sum, &thread_data[i]);
    }

    // Attendre la fin de tous les threads et calculer la somme totale
    long long total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_join(threads[i], NULL);
        total_sum += thread_data[i].partial_sum;
    }

    // Affichage de la somme totale
    printf("La somme totale est : %lld\n", total_sum);

    // Libération de la mémoire
    free(array);
    assert(total_sum==ARRAY_SIZE);

    return 0;
}
