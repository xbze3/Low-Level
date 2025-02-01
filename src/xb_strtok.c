#include <stddef.h>
char *xb_strtok(char *str, const char *delim)
{
    static char *static_ptr = NULL;

    if (str != NULL)
    {
        static_ptr = str;
    }

    while (*static_ptr && xb_strchr_helper(*static_ptr, delim))
    {
        static_ptr++;
    }

    if (*static_ptr == '\0')
    {
        return NULL;
    }

    char *start = static_ptr;

    while (*static_ptr && !xb_strchr_helper(*static_ptr, delim))
    {
        static_ptr++;
    }

    if (*static_ptr == '\0')
    {
        return start;
    }

    *static_ptr = '\0';

    static_ptr++;

    return start;
}

char *xb_strchr_helper(char ch, const char *src)
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