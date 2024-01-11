#include "lab1_IO.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

int threadnum, n;

int **A;
int **B;
int **result;

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

int main(int argc, char *argv[])
{
    // check number of argument, valid thread number, and if thread number is a square number
    if (argc != 2)
    {
        printf("Incorrect Arguments: Usage ./main <number of threads>\n");
        return 1;
    }

    threadnum = atoi(argv[1]);

    if (threadnum < 0)
    {
        printf("Incorrect Thread number: Please input thread number that is greater than 0.\n");
        return 1;
    }

    if (sqrt(threadnum) != floor(sqrt(threadnum)))
    {
        printf("Incorrect Thread number: Please input thread number that is a square number.\n");
        return 1;
    }

    Lab1_loadinput(&A, &B, &n);

    // check if n^2 is divisble by threadsnum
    if (((n * n) % threadnum) != 0)
    {
        int j = n * n;
        printf("Incorrect Thread number: Please input thread number that is %d is divisible by\n", j);
        return 1;
    }

    result = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        result[i] = malloc(n * sizeof(int));
    }

    pthread_t workers[threadnum];

    for (int i = 0; i < threadnum; i++)
    {
        if (pthread_create(&workers[i], NULL, threadfunc, (void *)(intptr_t)i) < 0)
        {
            perror("Thread Create failed");
        }
    }

    for (int i = 0; i < threadnum; i++)
    {
        pthread_join(workers[i], NULL);
    }

    // TODO actually time this
    Lab1_saveoutput(result, &n, 4.99);

    // Free memory
    for (int i = 0; i < n; i++)
    {
        free(A[i]);
        free(B[i]);
        free(result[i]);
    }
    free(A);
    free(B);
    free(result);

    // Serial Version: TODO compare with parallel version
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //     {
    //         result[i][j] = 0;

    //         for (int k = 0; k < n; k++)
    //         {
    //             result[i][j] += A[i][k] * B[k][j];
    //         }
    //         printf("%d\t", result[i][j]);
    //     }
    //     printf("\n");
    // }

    return 0;
}