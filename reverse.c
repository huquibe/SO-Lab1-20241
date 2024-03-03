#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
        exit(1);
    }

    FILE *output_file = fopen(argv[2], "r");
    if (output_file == NULL)
    {
        fprintf(stderr, "reverse: cannot open output file '%s'\n", argv[2]);
        exit(1);
    }

    if (input_file == output_file)
    {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    fclose(input_file);
    fclose(output_file);

    // Funcionalidad para reverse
    return 0;
}