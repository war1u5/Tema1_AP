void myCheck()
{
    printf("____CLI PARAMS____\n");
    printf("in file: %s\n", input_file);
    printf("out file: %s\n", output_file);
    printf("threads: %d\n", threads);
    printf("____FILE PARAMS____\n");
    printf("no dimensions %d\n", no_dimensions);
    printf("rows: %d\n", rows);
    printf("cols: %d\n", cols);
    printf("steps: %d\n", steps);

    printf("matrix:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");

    printf("new matrix:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%d ", newMatrix[i][j]);
        }
        printf("\n");
    }
}