#include "lab1_IO.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "lab1_IO.h"

int threadnum;
int **A;
int **B;
int **result;
int n;

void *threadfunc(void *argp)
{
    int rank = (int)argp;
    int x = floor(rank / sqrt(threadnum));
    int y = rank % (int)(sqrt(threadnum) * sqrt(threadnum));
    int rowUpperBound = (n / sqrt(threadnum)) * x;
    int rowLowerBound = ((n / sqrt(threadnum)) * (x + 1)) - 1;
    int collumUpperBound = (n / sqrt(threadnum)) * y;
    int collumLowerBound = ((n / sqrt(threadnum)) * (y + 1)) - 1;

    for (int i = rowLowerBound; i < rowUpperBound; i++)
    {
        for (int j = collumLowerBound; j < collumUpperBound; j++)
        {
            result[i][j] = 0;

            for (int k = 0; k < n; k++)
            {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[])
{

    threadnum = atoi(argv[1]);

    // check number of argument, valid thread number, and if thread number is a square number
    if (argc != 2)
    {
        printf("Incorrect Arguments: Usage ./main <number of threads>\n");
        return 1;
    }

    if (threadnum < 0)
    {
        printf("Incorrect Thread number: Please input thread number that is greater than 0 and is a square number\n");
        return 1;
    }

    if (sqrt(threadnum) != floor(sqrt(threadnum)))
    {
        printf("Incorrect Thread number: Please input thread number that is a square number\n");
        return 1;
    }

    Lab1_loadinput(&A, &B, &n);

    // check if n^2 is divisble bby threadsnum
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
        if (pthread_create(&workers[i], NULL, threadfunc, (void *)i) < 0)
        {
            perror("Thread Create failed");
        }
    }

    for (int i = 0; i < threadnum; i++)
    {
        pthread_join(workers[i], NULL);
    }

    Lab1_saveoutput(result, &n, 4.99);

    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         result[i][j] = 0;

    //         for (int k = 0; k < n; k++) {
    //             result[i][j] += A[i][k] * B[k][j];
    //         }
    //         printf("%d\t", result[i][j]);

    //     }
    //     printf("\n");
    // }

    return 0;
}