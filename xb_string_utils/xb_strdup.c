#include <stddef.h>
char *xb_strdup(const char *src)
{
    int count = 0;
    const char *tmp_src = src;
    while (*tmp_src)
    {
        count++;
        tmp_src++;
    }

    count += 1;

    char *dest = malloc(sizeof(char) * count);

    if (dest == NULL)
        return NULL;

    char *start = dest;

    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';

    return start;
}