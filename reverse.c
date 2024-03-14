#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LINE_LENGTH 256 // Ajuste según sea necesario para las longitudes de línea esperadas

// Estructura para representar un nodo de pila
typedef struct stack_node
{
    char line[MAX_LINE_LENGTH];
    struct stack_node *next;
} stack_node_t;

// Prototipos de funciones
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
    fclose(input_file);

    FILE *output_file = fopen(argv[2], "r");
    if (output_file == NULL)
    {
        fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
        exit(1);
    }
    fclose(output_file);

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

// Función para verificar si dos archivos están enlazados en duro
int areFilesHardlinked(const char *file1, const char *file2)
{
    struct stat stat1, stat2;
    if (stat(file1, &stat1) != 0 || stat(file2, &stat2) != 0)
    {
        return -1; // Ocurrió error
    }
    return (stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino);
}

// Función para empujar una línea a la pila
int push(stack_node_t **top, const char *line)
{
    stack_node_t *new_node = malloc(sizeof(stack_node_t)); // Reserva memoria para un nuevo nodo
    if (new_node == NULL)
    {
        fprintf(stderr, "Error: Falló la asignación de memoria.\n");
        return 1; // Indicar error
    }
    strcpy(new_node->line, line); // Copia la línea proporcionada en el campo line del nuevo nodo
    new_node->next = *top;        // Actualiza el puntero next del nuevo nodo para que apunte al nodo que está en el tope de la pila
    *top = new_node;              // Actualiza el puntero top para que apunte al nuevo nodo, convirtiéndolo en el nuevo tope de la pila
    return 0;                     // Éxito
}

// Función para sacar una línea de la pila
char *pop(stack_node_t **top)
{
    if (*top == NULL)
    {
        return NULL; // La pila está vacía
    }
    stack_node_t *temp = *top;                   // Se crea un nuevo puntero temp que apunta al nodo en la parte superior de la pila
    char *line = malloc(strlen(temp->line) + 1); // Se asigna memoria para una nueva cadena de caracteres
    if (line == NULL)
    {
        fprintf(stderr, "Error: Falló la asignación de memoria.\n");
        return NULL; // Indicar error
    }
    strcpy(line, temp->line); // Se copia la línea almacenada en el nodo en la parte superior de la pila en la nueva cadena
    *top = temp->next;        // Se actualiza el puntero top para que apunte al siguiente nodo en la pila
    free(temp);               // Se libera la memoria del nodo que fue eliminado de la pila
    return line;              // Se devuelve la nueva cadena que contiene la línea eliminada de la pila
}

// Función para leer líneas de un archivo, invertirlas usando una pila y escribirlas en otro archivo
int read_and_write_lines(const char *input_filename, const char *output_filename)
{
    FILE *input_file = fopen(input_filename, "r");

    FILE *output_file = fopen(output_filename, "w");

    char line[MAX_LINE_LENGTH];
    stack_node_t *stack = NULL; // Inicializar una pila vacía

    // Leer líneas del archivo de entrada y ponerlas en la pila
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL)
    {
        // Eliminar el carácter de nueva línea si está presente
        size_t length = strlen(line);
        if (length > 0 && line[length - 1] == '\n')
            line[length - 1] = '\0';

        if (push(&stack, line) != 0)
        {
            fclose(input_file);
            fclose(output_file);
            return 1; // Indicar error
        }
    }

    fclose(input_file);

    // Sacar líneas de la pila y escribirlas en el archivo de salida
    while (stack != NULL)
    {
        char *line = pop(&stack);
        if (line == NULL)
        {
            fprintf(stderr, "Error: Failed to pop from stack.\n");
            fclose(output_file);
            return 1; // Indicar error
        }

        fprintf(output_file, "%s\n", line);
        free(line); // Liberar la memoria asignada
    }

    fclose(output_file);

    return 0; // Salir
}