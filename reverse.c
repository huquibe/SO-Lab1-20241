#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256 // Adjust as needed for your expected line lengths

// Structure to represent a stack node
typedef struct stack_node
{
    char line[MAX_LINE_LENGTH];
    struct stack_node *next;
} stack_node_t;

// Function prototypes
int areFilesHardlinked(const char *file1, const char *file2);
int push(stack_node_t **top, const char *line);
char *pop(stack_node_t **top);
int read_and_write_lines(const char *input_filename, const char *output_filename);

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
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
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

    return read_and_write_lines(argv[1], argv[2]);
}

// Function to check if two files are hardlinked
int areFilesHardlinked(const char *file1, const char *file2)
{
    struct stat stat1, stat2;
    if (stat(file1, &stat1) != 0 || stat(file2, &stat2) != 0)
    {
        return -1; // Error occurred
    }
    return (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino);
}

// Function to push a line onto the stack
int push(stack_node_t **top, const char *line)
{
    stack_node_t *new_node = malloc(sizeof(stack_node_t)); // Reserva memoria para un nuevo nodo
    if (new_node == NULL)
    {
        fprintf(stderr, "Error: Falló la asignación de memoria.\n");
        return 1; // Indicar error
    }
    strcpy(new_node->line, line); // Copia la línea proporcionada en el campo line del nuevo nodo
    new_node->next = *top; // Actualiza el puntero next del nuevo nodo para que apunte al nodo que está en el tope de la pila
    *top = new_node; // Actualiza el puntero top para que apunte al nuevo nodo, convirtiéndolo en el nuevo tope de la pila
    return 0; // Éxito
}

// Function to pop a line from the stack
char *pop(stack_node_t **top)
{
    if (*top == NULL)
    {
        return NULL; // La pila está vacía
    }
    stack_node_t *temp = *top; // Se crea un nuevo puntero temp que apunta al nodo en la parte superior de la pila
    char *line = malloc(strlen(temp->line) + 1); // Se asigna memoria para una nueva cadena de caracteres
    if (line == NULL)
    {
        fprintf(stderr, "Error: Falló la asignación de memoria.\n");
        return NULL; // Indicar error
    }
    strcpy(line, temp->line); // Se copia la línea almacenada en el nodo en la parte superior de la pila en la nueva cadena
    *top = temp->next; // Se actualiza el puntero top para que apunte al siguiente nodo en la pila
    free(temp); // Se libera la memoria del nodo que fue eliminado de la pila
    return line; // Se devuelve la nueva cadena que contiene la línea eliminada de la pila
}

// Function to read lines from a file, reverse them using a stack, and write them to another file
// Function to read lines from a file, reverse them using a stack, and write them to another file
int read_and_write_lines(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");
    if (input_file == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", input_filename);
        exit(1);
    }

    FILE *output_file = fopen(output_filename, "w"); // Always open for writing
    if (output_file == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", output_filename);
        fclose(input_file);
        exit(1);
    }

    if (strcmp(input_filename, output_filename) == 0)
    {
        fprintf(stderr, "reverse: input and output file must differ\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    stack_node_t *stack = NULL; // Initialize an empty stack

    // Read lines from input file and push them onto the stack
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL)
    {
        // Strip newline character if present
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n')
            line[length - 1] = '\0';

        if (push(&stack, line) != 0)
        {
            fclose(input_file);
            fclose(output_file);
            return 1; // Indicate error
        }
    }

    fclose(input_file);

    // Pop lines from the stack and write them to the output file
    while (stack != NULL)
    {
        char *line = pop(&stack);
        if (line == NULL)
        {
            fprintf(stderr, "Error: Failed to pop from stack.\n");
            fclose(output_file);
            return 1; // Indicate error
        }

        fprintf(output_file, "%s\n", line);
        free(line); // Free allocated memory
    }

    fclose(output_file);

    return 0; // Exit
}