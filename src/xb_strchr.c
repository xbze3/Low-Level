#include <stddef.h>
char *xb_strchr(char ch, const char *src)
{
    while (*src)
    {
        if (*src == ch)
        {
            return src;
        }
        src++;
    }

    return NULL;
}