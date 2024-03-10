#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256 // Adjust as needed for your expected line lengths

int areFilesHardlinked(const char *file1, const char *file2)
{
    struct stat stat1, stat2;
    if (stat(file1, &stat1) != 0 || stat(file2, &stat2) != 0)
        return -1; // Error occurred

    return (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino);
}

int read_and_print_lines(const char *filename)
{
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    size_t bytes_read;

    while ((bytes_read = fread(line, sizeof(char), MAX_LINE_LENGTH - 1, input_file)) > 0)
    {
        // Ensure proper null termination
        line[bytes_read] = '\0';

        // Print the line, removing any trailing newline if present
        if (line[bytes_read - 1] == '\n')
        {
            line[bytes_read - 1] = '\0'; // Overwrite newline
        }
        printf("%s\n", line);
    }

    if (ferror(input_file))
    {
        fprintf(stderr, "Error: An error occurred while reading the file.\n");
        fclose(input_file);
        return 1; // Indicate error
    }

    fclose(input_file);
    return 0; // Success
}

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

    if (strcmp(argv[1], argv[2]) == 0)
    {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    if (areFilesHardlinked(argv[1], argv[2]))
    {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    return read_and_print_lines(argv[1]);

    fclose(input_file);
    fclose(output_file);

    
    return 0;
}