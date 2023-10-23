#define _CRT_SECURE_NO_WARNINGS
#define _XOPEN_SOURCE 600
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
pthread_barrier_t barrier;
pthread_mutex_t mutex;

char input_file[100];
char output_file[100];
int threads;

int no_dimensions;
int rows;
int cols;
int steps;
int **matrix;
int **newMatrix;

void getInitialParams(char *in_file, char *out_file, int no_threads)
{
    memset(input_file, 0, 100);
    strcpy(input_file, in_file);

    memset(output_file, 0, 100);
    strcpy(output_file, out_file);

    threads = no_threads;
}

void getFileParams(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        printf("Error opening the file.\n");
        exit(-1);
    }

    int read_values = fscanf(f, "%d %d %d %d", &no_dimensions, &rows, &cols, &steps);
    if (read_values != 4)
    {
        printf("Error reading values from the file.\n");
        fclose(f);
        exit(-1);
    }
    int aux = rows;
    rows = cols;
    cols = aux;

    matrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            read_values = fscanf(f, "%d", &matrix[i][j]);
            if (read_values != 1)
            {
                printf("Error reading values from the file.\n");
                fclose(f);
                exit(-1);
            }
        }
    }

    newMatrix = (int **)malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        newMatrix[i] = (int *)malloc(cols * sizeof(int));
        memset(newMatrix[i], 0, cols * sizeof(int));
    }

    fclose(f);
}
void myCheck()
{
    printf("matrix:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
    }

    printf("\n");
}
void *threadRoutine(void *var)
{
    int thread_id = *(int *)var;

    double ownElems = (double)cols / (double)threads;
    int start = (thread_id * (double)ceil(ownElems));
    int end = min(cols, (thread_id + 1) * (double)ceil(ownElems));

    // printf("Thread %d start: %d end: %d\n", thread_id, start, end);

    for (int k = 0; k < steps; k++)
    {
        pthread_barrier_wait(&barrier);
        for (int i = 0; i < rows; i++)
        {
            for (int j = start; j < end; j++)
            {
                if (matrix[i][j] == 1)
                {
                    newMatrix[i][j] = 2;
                    continue;
                }
                if (matrix[i][j] == 2)
                {
                    newMatrix[i][j] = 0;
                    continue;
                }

                int on_neighbors = 0;
                for (int x = -1; x <= 1; ++x)
                {
                    for (int y = -1; y <= 1; ++y)
                    {
                        if (x == 0 && y == 0)
                        {
                            continue;
                        }
                        int ni = i + x;
                        int nj = j + y;

                        if ((ni < 0 || ni >= rows || nj < 0 || nj >= cols))
                        {
                            ;
                        }
                        else if (matrix[ni][nj] == 1)
                        {
                            on_neighbors++;
                        }
                    }
                }

                if (matrix[i][j] == 0 && on_neighbors == 2)
                {
                    newMatrix[i][j] = 1;
                }
            }
        }
        pthread_barrier_wait(&barrier);

        for (int i = 0; i < rows; i++)
        {
            for (int j = start; j < end; j++)
            {
                matrix[i][j] = newMatrix[i][j];
            }
        }
    }
    pthread_exit(NULL);
}

void writeInFile(const char *filename)
{
    FILE *f = fopen(filename, "w");
    if (f == NULL)
    {
        printf("Error opening the file for writing.\n");
        exit(-1);
    }

    fprintf(f, "%d %d %d\n", no_dimensions, cols, rows);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            fprintf(f, "%d ", matrix[i][j]);
        }
    }

    fclose(f);
}

int main(int argc, char *argv[])
{
    getInitialParams(argv[1], argv[2], atoi(argv[3]));

    getFileParams(input_file);

    pthread_t tid[threads];
    int thread_id[threads];
    pthread_barrier_init(&barrier, NULL, threads);

    for (int i = 0; i < threads; i++)
    {
        thread_id[i] = i;
    }
    for (int i = 0; i < threads; i++)
    {
        pthread_create(&tid[i], NULL, threadRoutine, &thread_id[i]);
    }
    for (int i = 0; i < threads; i++)
    {
        pthread_join(tid[i], NULL);
    }

    writeInFile(output_file);
    // myCheck();

    return 0;
}