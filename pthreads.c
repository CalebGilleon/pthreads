#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_M 1000
#define MAX_N 4
#define MAX_VALUE 79

int **integerArray;

int generateRandomInteger() {
    return 1 + rand() % MAX_VALUE;
}

void convertModulo19(int *element) {
    *element = *element % 19;
}

typedef struct {
    int threadId;
    int numRows;
    int numColumns;
} ThreadArgs;

int m, n;

void *updateArray(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int threadId = args->threadId;
    int numRows = args->numRows;
    int numColumns = args->numColumns;
    int startRow = threadId * numRows;
    int endRow = (threadId == n - 1) ? 5 : startRow + numRows;

    for (int i = startRow; i < endRow; i++) {
        for (int j = 0; j < numColumns; j++) {
            convertModulo19(&integerArray[i][j]);
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <m> <n>\n", argv[0]);
        return 1;
    }

    m = atoi(argv[1]);
    n = atoi(argv[2]);

    srand(time(NULL));

    integerArray = (int **)malloc(5 * sizeof(int *));
    for (int i = 0; i < 5; i++) {
        integerArray[i] = (int *)malloc(m * sizeof(int));
    }

    printf("Using %d threads.\n", n);
    printf("Original array:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < m; j++) {
            integerArray[i][j] = generateRandomInteger();
            printf("%d", integerArray[i][j]);
            if (j < m - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    pthread_t threads[n];
    ThreadArgs threadArgs[n];

    for (int i = 0; i < n; i++) {
        threadArgs[i].threadId = i;
        threadArgs[i].numRows = 5 / n;
        threadArgs[i].numColumns = m;
        pthread_create(&threads[i], NULL, updateArray, &threadArgs[i]);
    }

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Updated array:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d", integerArray[i][j]);
            if (j < m - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < 5; i++) {
        free(integerArray[i]);
    }
    free(integerArray);

    return 0;
}