#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura para un nodo de la lista enlazada
struct Node
{
    char *line;
    struct Node *next;
};

// Función para crear un nuevo nodo
struct Node *createNode(char *line)
{
    struct Node *newNode = malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        perror("Error al asignar memoria para un nuevo nodo");
        exit(1);
    }
    newNode->line = strdup(line);
    newNode->next = NULL;
    return newNode;
}

// Función para insertar al final de la lista enlazada
void insertEnd(struct Node **head, char *line)
{
    struct Node *newNode = createNode(line);
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        struct Node *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s <archivo_de_entrada>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error abriendo archivo");
        return 1;
    }

    struct Node *head = NULL;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
    {
        insertEnd(&head, line);
    }

    // Imprimir y liberar la lista enlazada
    struct Node *temp = head;
    while (temp != NULL)
    {
        printf("%s", temp->line);
        struct Node *prev = temp;
        temp = temp->next;
        free(prev->line); // Liberar la cadena duplicada
        free(prev);       // Liberar el nodo
    }

    free(line);
    fclose(file);

    return 0;
}
