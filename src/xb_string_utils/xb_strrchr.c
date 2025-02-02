#include <stddef.h>
char *xb_strrchr(const char *src, const char ch)
{
    char *current = NULL;
    while (*src)
    {
        if (*src == ch)
        {
            current = (char *)src;
        }
        src++;
    }

    if (ch == '\0')
    {
        current = (char *)src;
    }

    return current;
}