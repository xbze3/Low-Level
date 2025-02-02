#include <stdio.h>
#include <string.h>

typedef struct xb_array
{
    unsigned capacity;
    int length;
    int *contain;
} xb_array;

xb_array *init_xb_array()
{
    xb_array *Array = malloc(sizeof(xb_array));

    if (Array == NULL)
        return NULL;

    Array->capacity = 10;
    Array->length = 0;
    Array->contain = malloc(sizeof(int) * Array->capacity);

    if (Array->contain == NULL)
    {
        free(Array);
        return NULL;
    }

    return Array;
}

int xb_append(xb_array *array, int value)
{
    if (array->length == array->capacity)
    {
        int *tmp = malloc(sizeof(int) * (array->capacity * 2));
        if (tmp == NULL)
            return -1;

        memcpy(tmp, array->contain, sizeof(int) * array->length);
        free(array->contain);
        array->contain = tmp;
        array->capacity *= 2;
    }

    array->contain[array->length++] = value;

    return 0;
}

int xb_printList(xb_array *array)
{
    if (!array)
        return -1;

    for (int i = 0; i < array->length; i++)
    {
        printf("Element: %d\n", array->contain[i]);
    }

    return array->length;
}

void xb_free(xb_array *array)
{
    if (array)
    {
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

    xb_append(xb_nums, 10);
    xb_append(xb_nums, 20);
    xb_append(xb_nums, 30);
    xb_append(xb_nums, 40);
    xb_append(xb_nums, 50);
    xb_append(xb_nums, 60);
    xb_append(xb_nums, 70);
    xb_append(xb_nums, 80);
    xb_append(xb_nums, 90);
    xb_append(xb_nums, 100);
    xb_append(xb_nums, 110);

    xb_printList(xb_nums);

    printf("\nLength: %d\n", xb_nums->length);
    printf("Capacity: %d\n", xb_nums->capacity);

    xb_free(xb_nums);

    return 0;
}
