#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct xb_array
{
    unsigned capacity;
    int length;
    char **contain;
} xb_array;

xb_array *init_xb_array()
{
    xb_array *Array = malloc(sizeof(xb_array));

    if (Array == NULL)
        return NULL;

    Array->capacity = 10;
    Array->length = 0;
    Array->contain = malloc(sizeof(char *) * Array->capacity);

    if (Array->contain == NULL)
    {
        free(Array);
        return NULL;
    }

    return Array;
}

int xb_append(xb_array *array, const char *value)
{
    if (array->length == array->capacity)
    {
        char **tmp = malloc(sizeof(char *) * (array->capacity * 2));
        if (tmp == NULL)
            return -1;

        memcpy(tmp, array->contain, sizeof(char *) * array->length);
        free(array->contain);
        array->contain = tmp;
        array->capacity *= 2;
    }

    array->contain[array->length] = malloc(strlen(value) + 1);
    if (array->contain[array->length] == NULL)
        return -1;

    strcpy(array->contain[array->length], value);
    array->length++;

    return 0;
}

int xb_printList(xb_array *array)
{
    if (!array)
        return -1;

    for (int i = 0; i < array->length; i++)
    {
        printf("Element: %s\n", array->contain[i]);
    }

    return array->length;
}

void xb_free(xb_array *array)
{
    if (array)
    {
        for (int i = 0; i < array->length; i++)
        {
            free(array->contain[i]);
        }
        free(array->contain);
        free(array);
    }
}

int main()
{
    xb_array *xb_nums = init_xb_array();

    if (xb_nums == NULL)
    {
        printf("Failed to initialize xb_array.\n");
        return -1;
    }

    printf("Initial xb_array pointer: %p\n\n", xb_nums);

    xb_append(xb_nums, "Hello");
    xb_append(xb_nums, "World");

    xb_printList(xb_nums);

    printf("\nLength: %d\n", xb_nums->length);
    printf("Capacity: %d\n", xb_nums->capacity);

    xb_free(xb_nums);

    return 0;
}
