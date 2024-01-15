#include "lab1_IO.h"
#include "timer.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

int threadnum, n;
int **A, **B, **result, **serResult;

void *threadfunc(void *argp)
{
    intptr_t rank = (intptr_t)argp;
    int x = floor(rank / sqrt(threadnum));
    int y = rank % (int)(sqrt(threadnum));
    int rowLowerBound = (n / sqrt(threadnum)) * x;
    int rowUpperBound = ((n / sqrt(threadnum)) * (x + 1)) - 1;
    int colLowerBound = (n / sqrt(threadnum)) * y;
    int colUpperBound = ((n / sqrt(threadnum)) * (y + 1)) - 1;

    for (int i = rowLowerBound; i <= rowUpperBound; i++)
    {
        for (int j = colLowerBound; j <= colUpperBound; j++)
        {
            result[i][j] = 0;

            for (int k = 0; k < n; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return NULL;
}

void serialMatrixMultiply()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {

            serResult[i][j] = 0;

            for (int k = 0; k < n; k++)
            {
                serResult[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void cleanup()
{
    for (int i = 0; i < n; i++)
    {
        free(A[i]);
        free(B[i]);
        free(result[i]);
        free(serResult[i]);
    }

    free(A);
    free(B);
    free(result);
    free(serResult);
}

int main(int argc, char *argv[])
{
    // check number of argument, valid thread number, and if thread number is a square number
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <number of threads>\n", argv[0]);
        return 1;
    }

    threadnum = atoi(argv[1]);

    if (threadnum <= 0 || sqrt(threadnum) != floor(sqrt(threadnum)))
    {
        fprintf(stderr, "Invalid thread number. Please input a positive square number.\n");
        return 1;
    }

    // Load input matrices
    Lab1_loadinput(&A, &B, &n);

    // check if n^2 is divisble by threadsnum
    if (((n * n) % threadnum) != 0)
    {
        fprintf(stderr, "Invalid thread number. Please input a number that divides evenly into n^2.\n");
        return 1;
    }

    result = malloc(n * sizeof(int *));
    serResult = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        result[i] = malloc(n * sizeof(int));
        serResult[i] = malloc(n * sizeof(int));
    }

    // Parallel matrix multiplication
    double start, end;
    GET_TIME(start);

    pthread_t* workers;
    workers = malloc(threadnum * sizeof(pthread_t));
    
    for (long i = 0; i < threadnum; i++)
    {
        if (pthread_create(&workers[i], NULL, threadfunc, (void *)(intptr_t)i) < 0)
        {
            perror("Thread Create failed");
            cleanup();
            return 1;
        }
    }

    for (long i = 0; i < threadnum; i++)
    {
        pthread_join(workers[i], NULL);
    }

    GET_TIME(end);

    // Save output and display timings
    Lab1_saveoutput(result, &n, end - start);
    printf("Parallel Time: %f\n", end - start);

    // Serial matrix multiplication
    GET_TIME(start);
    serialMatrixMultiply();
    GET_TIME(end);

    printf("Serial Time: %f\n", end - start);

    // Compare results
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; ++j)
        {
            if (result[i][j] != serResult[i][j])
            {
                printf("Error: Result mismatch at row %d column %d\n, %d != %d", i, j, result[i][j], serResult[i][j]);
                return 1;
            }
        }
    }
    printf("Parallel and serial results match\n");

    // Free memory
    cleanup();

    return 0;
}
